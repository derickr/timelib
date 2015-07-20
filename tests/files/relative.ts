Expected:      | String to parse:    | Hole filler:             | Timezone:              | Comments:
TS: 1116457202 | +2 sec              | 2005-05-18 23:00 GMT     | GMT
TS: 1116457198 | 2 secs ago          | 2005-05-18 23:00 GMT     | GMT

TS: 1116630000 | +2 days             | 2005-05-18 23:00 GMT     | GMT                    |  \
TS: 1116630000 |                     | 2005-05-20 23:00 GMT     | GMT                    |   \ should be the same

TS: 1116284400 | +2 days ago         | 2005-05-18 23:00 GMT     | GMT

TS: 1112828400 | -3 forthnight       | 2005-05-18 23:00 GMT     | GMT
TS: 1112828400 |                     | 2005-04-06 23:00 GMT     | GMT

TS: 1123714800 | +12 weeks           | 2005-05-18 23:00 GMT     | GMT
TS: 1123714800 |                     | 2005-08-10 23:00 GMT     | GMT

TS: 1128938400 | 0 secs              | 2005-10-10 12:00         | Europe/Amsterdam
TS: 1128942000 | 0 secs              | 2005-10-10 12:00 CET     | Europe/Amsterdam
TS: 1128938400 | 0 secs              | 2005-10-10 12:00 CEST    | Europe/Amsterdam
TS: 1131620400 | 0 secs              | 2005-11-10 12:00         | Europe/Amsterdam
TS: 1131620400 | 0 secs              | 2005-11-10 12:00 CET     | Europe/Amsterdam
TS: 1131616800 | 0 secs              | 2005-11-10 12:00 CEST    | Europe/Amsterdam
TS: 1131620400 | +31 days            | 2005-10-10 12:00         | Europe/Amsterdam

TS: 1099648800 | 6 month 2004-05-05 12:00:00 CEST |             |
TS: 1099648800 | 2004-11-05 12:00:00 CEST         |             |
TS: 1099648800 | 2004-05-05 12:00:00 CEST 6 months |            |

TS: 1099648800 | 6 month 2004-05-05 12:00:00 CEST |             | Europe/Amsterdam
TS: 1099648800 | 2004-11-05 12:00:00 CEST |                     | Europe/Amsterdam
TS: 1099648800 | 2004-05-05 12:00:00 CEST 6 months |            | Europe/Amsterdam

TS: 1099652400 | 6 month 2004-05-05 12:00:00 |                  | Europe/Amsterdam
TS: 1099652400 | 2004-11-05 12:00:00 |                          | Europe/Amsterdam
TS: 1099652400 | 2004-05-05 12:00:00 6 months |                 | Europe/Amsterdam
TS: 1083751200 | 2004-05-05 12:00:00 |                          | Europe/Amsterdam

TS: 1068027323 | 2003-11-05 12:15:23 CEST |                     |
TS: 1068027323 | 2004-05-05 12:15:23 CEST 6 months ago |        |
TS: 1068372923 | 2003-11-09 12:15:23 CEST |                     |
TS: 1068372923 | 2004-05-05 12:15:23 CEST 6 months ago 4 days | |

TS: 1145570400 | 21-04-2006      |            | Europe/Amsterdam
TS: 1145570400 | this weekday    | 21-04-2006 | Europe/Amsterdam 
TS: 1145484000 | last weekday    | 21-04-2006 | Europe/Amsterdam
TS: 1145570400 | last weekday    | 22-04-2006 | Europe/Amsterdam
TS: 1145570400 | last weekday    | 23-04-2006 | Europe/Amsterdam
TS: 1145397600 | 13 weekdays ago | 07-05-2006 | Europe/Amsterdam

TS: 1145570400 | 21-04-2006      |            | Europe/Amsterdam
TS: 1145570400 | this weekday    | 21-04-2006 | Europe/Amsterdam
TS: 1145829600 | 24-04-2006      |            | Europe/Amsterdam
TS: 1145829600 | this weekday    | 22-04-2006 | Europe/Amsterdam
TS: 1145829600 | this weekday    | 23-04-2006 | Europe/Amsterdam

TS: 1145829600 | 24-04-2006    |            | Europe/Amsterdam
TS: 1145829600 | first weekday | 21-04-2006 | Europe/Amsterdam
TS: 1145829600 | first weekday | 22-04-2006 | Europe/Amsterdam
TS: 1145829600 | first weekday | 23-04-2006 | Europe/Amsterdam
TS: 1145916000 | 25-04-2006    |            | Europe/Amsterdam
TS: 1145916000 | first weekday | 24-04-2006 | Europe/Amsterdam
TS: 1146002400 | 26-04-2006    |            | Europe/Amsterdam
TS: 1146002400 | 8 weekday     | 15-04-2006 | Europe/Amsterdam
TS: 1146002400 | eight weekday | 15-04-2006 | Europe/Amsterdam
TS: 1149700004 | Mon, 08 May 2006 13:06:44 -0400 +30 days | | Europe/Amsterdam
