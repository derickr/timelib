/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 MongoDB, Inc.
 * Copyright (c) 2015 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "timelib.h"

#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>

/* Filter out some non-tzdata files and the posix/right databases, if
 * present. */
static int index_filter(const struct dirent *ent)
{
	return strcmp(ent->d_name, ".") != 0
		&& strcmp(ent->d_name, "..") != 0
		&& strcmp(ent->d_name, "posix") != 0
		&& strcmp(ent->d_name, "posixrules") != 0
		&& strcmp(ent->d_name, "right") != 0
		&& strcmp(ent->d_name, "Etc") != 0
		&& strcmp(ent->d_name, "localtime") != 0
		&& strstr(ent->d_name, ".list") == NULL
		&& strstr(ent->d_name, ".tab") == NULL;
}

static int sysdbcmp(const void *first, const void *second)
{
	const timelib_tzdb_index_entry *alpha = first, *beta = second;

	return strcasecmp(alpha->id, beta->id);
}

/* Returns true if the passed-in stat structure describes a
 * probably-valid timezone file. */
static int is_valid_tzfile(const struct stat *st, int fd)
{
	if (fd) {
		char buf[20];
		if (read(fd, buf, 20) != 20) {
			return 0;
		}
		lseek(fd, SEEK_SET, 0);
		if (memcmp(buf, "TZif", 4)) {
			return 0;
		}
	}
	return S_ISREG(st->st_mode) && st->st_size > 20;
}

/* Return the mmap()ed tzfile if found, else NULL.  On success, the
 * length of the mapped data is placed in *length. */
static char *map_tzfile(const char *directory, const char *timezone, size_t *length)
{
	char fname[PATH_MAX];
	struct stat st;
	char *p;
	int fd;
	
	if (timezone[0] == '\0' || strstr(timezone, "..") != NULL) {
		return NULL;
	}

	snprintf(fname, sizeof(fname), "%s/%s", directory, timezone /* canonical_tzname(timezone) */);

	fd = open(fname, O_RDONLY);
	if (fd == -1) {
		return NULL;
	} else if (fstat(fd, &st) != 0 || !is_valid_tzfile(&st, fd)) {
		close(fd);
		return NULL;
	}

	*length = st.st_size;
	p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	
	return p != MAP_FAILED ? p : NULL;
}

/* Create the zone identifier index by trawling the filesystem. */
static int create_zone_index(const char *directory, timelib_tzdb *db)
{
	size_t dirstack_size,  dirstack_top;
	size_t index_size, index_next;
	timelib_tzdb_index_entry *db_index;
	char **dirstack;
	size_t data_size = 0;
	unsigned char *tmp_data = NULL;

	/* LIFO stack to hold directory entries to scan; each slot is a
	 * directory name relative to the zoneinfo prefix. */
	dirstack_size = 32;
	dirstack = malloc(dirstack_size * sizeof(*dirstack));
	dirstack_top = 1;
	dirstack[0] = strdup("");
	
	/* Index array. */
	index_size = 64;
	db_index = malloc(index_size * sizeof(timelib_tzdb_index_entry));
	index_next = 0;

	do {
		struct dirent **ents;
		char name[PATH_MAX], *top;
		int count;

		/* Pop the top stack entry, and iterate through its contents. */
		top = dirstack[--dirstack_top];
		snprintf(name, sizeof(name), "%s/%s", directory, top);

		count = scandir(name, &ents, index_filter, alphasort);
		if (count == -1) {
			free(dirstack);
			free(db_index);
			return -errno;
		}

		while (count > 0) {
			struct stat st;
			const char *leaf = ents[count - 1]->d_name;

			snprintf(name, sizeof(name), "%s/%s/%s", directory, top, leaf);
			
			if (strlen(name) && stat(name, &st) == 0) {
				/* Name, relative to the zoneinfo prefix. */
				const char *root = top;

				if (root[0] == '/') {
					root++;
				}

				snprintf(name, sizeof(name), "%s%s%s", root, *root ? "/": "", leaf);

				if (S_ISDIR(st.st_mode)) {
					if (dirstack_top == dirstack_size) {
						dirstack_size *= 2;
						dirstack = realloc(dirstack, dirstack_size * sizeof(*dirstack));
					}
					dirstack[dirstack_top++] = strdup(name);
				} else {
					if (index_next == index_size) {
						index_size *= 2;
						db_index = realloc(db_index, index_size * sizeof(timelib_tzdb_index_entry));
					}

					db_index[index_next].id = strdup(name);

					{
						size_t length;
						const char *tzfile_data = map_tzfile(directory, name, &length);

						if (tzfile_data) {
							tmp_data = realloc(tmp_data, data_size + length);
							memcpy(tmp_data + data_size, tzfile_data, length);
							db_index[index_next].pos = data_size;
							data_size += length;
						}
					}

					index_next++;
				}
			}

			free(ents[--count]);
		}
		
		if (count != -1) {
			free(ents);
		}
		free(top);
	} while (dirstack_top);

	qsort(db_index, index_next, sizeof(*db_index), sysdbcmp);

	db->index = db_index;
	db->index_size = index_next;
	db->data = tmp_data;

	free(dirstack);

	return 0;
}

timelib_tzdb *timelib_zoneinfo(char *directory)
{
	timelib_tzdb *tmp = malloc(sizeof(timelib_tzdb));

	tmp->version = "0.system";
	tmp->data = NULL;
	if (create_zone_index(directory, tmp) < 0) {
		free(tmp);
		return NULL;
	}
	return tmp;
}

void timelib_zoneinfo_dtor(timelib_tzdb *tzdb)
{
	int i;

	for (i = 0; i < tzdb->index_size; i++) {
		free(tzdb->index[i].id);
	}
	free((timelib_tzdb_index_entry*) tzdb->index);
	free((char*) tzdb->data);
	free(tzdb);
}
