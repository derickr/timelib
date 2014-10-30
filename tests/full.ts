Expected:      | String to parse:    | Hole filler:             | Timezone:              | Comments:
TS: 1126396800 | 9/11                | 2005-09-11 00:00:00      |                        | We have no timezone at all -> GMT
TS: 1126396800 | 9/11                | 2005-09-11 00:00:00 GMT  |                        | The filler specified a timezone -> GMT
TS: 1126396800 | 9/11                | 2005-09-11 00:00:00      | GMT                    | Global timezone is set -> GMT
TS: 1126396800 | 9/11 GMT            | 2005-09-11 00:00:00      |                        | String to be parsed contains timezone -> GMT

TS: 1126393200 | 9/11                | 2005-09-11 00:00:00 CET  |                        | Timezone specified -> CET, no DST (GMT+1)
TS: 1126389600 | 9/11                | 2005-09-11 00:00:00 CEST |                        | Timezone specified -> CEST, with DST (GMT+2)

TS: 1126393200 | 9/11 CET            | 2005-09-11 00:00:00      |                        | Timezone specified -> CET, no DST (GMT+1)
TS: 1126389600 | 9/11 CEST           | 2005-09-11 00:00:00      |                        | Timezone specified -> CEST, with DST (GMT+2)

TS: 1126389600 | 9/11                | 2005-09-11 00:00:00      | Europe/Amsterdam       | Zone identifier specified -> use it (CEST, GMT+2)
TS: 1126393200 | 9/11                | 2005-09-11 00:00:00 CET  | Europe/Amsterdam       | Timezone specified (wrong) and Zone ID specified -> adjust (GMT+1)
TS: 1126389600 | 9/11                | 2005-09-11 00:00:00 CEST | Europe/Amsterdam       | Timezone specified and Zone ID specified -> (GMT+2)

TS: 1105401600 | 1/11                | 2005-01-11 00:00:00 GMT  |                        | Timezone specified -> GMT
TS: 1105398000 | 1/11                | 2005-01-11 00:00:00 CET  |                        | Timezone specified -> CET
TS: 1105394400 | 1/11                | 2005-01-11 00:00:00 CEST |                        | Timezone specified -> CEST (doesn't actually exist)

TS: 1105398000 | 1/11                | 2005-01-11 00:00:00      | Europe/Amsterdam       | Zone identifier specified -> use it (CST, GMT+1)
TS: 1105398000 | 1/11                | 2005-01-11 00:00:00 CET  | Europe/Amsterdam       | Timezone specified and Zone ID specified -> adjust (GMT+1)
TS: 1105394400 | 1/11                | 2005-01-11 00:00:00 CEST | Europe/Amsterdam       | Timezone specified (wrong) and Zone ID specified -> (GMT+2)

TS: 283132800  | 1978-12-22          | 00:00:00 GMT             |
TS: 283147200  | 1978-12-22          | 00:00:00 EDT             |
TS: 1113861600 | 2005-04-19          |                          | Europe/Amsterdam
TS: 1113886800 | 2005-04-19          |                          | America/Chicago
TS: 1113883200 | 2005-04-19          |                          | America/New_York
TS: 1113883200 | 2005-04-19          | 00:00:00                 | America/New_York
TS: 1113918120 | 2005-04-19          | 09:42:00                 | America/New_York
