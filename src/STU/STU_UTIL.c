
#define _CRT_SECURE_NO_WARNINGS

/*
asctime .............................. 22
biostime ............................ 57
_bios_timeofday ..................... 58
clock ............................... 82
ctime .............................. 101
difftime ............................ 107
_dos_getftime, _dos_setftime ......... 120
_dos_gettime, _dos_settime .......... 121
? dostounix .......................... 126
ftime .............................. 204
getdate, _dos_getdate, _dos_setdate,
setdate ............................ 219
getftime, setftime ................... 233
gettime, settime ..................... 259
gmtime ............................ 268
localtime ........................... 335
mktime ............................ 366
stime .............................. 522
strftime ............................ 532
_strtime ........................... 545
time ............................... 566
? unixtodos .......................... 578
utime ............................. 581
timezone .......................... 609
tzname ............................ 609

#include <time.h>
struct tm t;
char *asctime(const struct tm *tblock);

time_t t;
t = time (NULL) ;
printf("Today's date and time: %s\n", ctime(&t));

Set the global long variable timezone to the difference in seconds between
GMT and local standard time (in PST, timezone is 8x60x60). The global
variable daylight is nonzero if and only if the standard U.s. daylight saving
time conversion should be applied.

/ * pacific standard & daylight savings time * /
char *tzstr = "TZ=PST8PDT";
time_t t;
struct tm *gmt, *area;
putenv (tzstr) ;
tzset();
t = time (NULL) ;
area = localtime(&t);
printf("Local time is: %s", asctime(area));
gmt = gmtime(&t);
printf("GMT is: %s", asctime(gmt));

#define SECONDS_PER_DAY 86400L / * number of secs in one day * /
struct date dt;
struct time tm;
/ * get today's date and time * /
getdate(&dt);
gettime (&tm) ;
printf("Today is %d %s %d\n", dt.da_day, month[dt.da_mon], dt .da-year);

#include <dos.h>
struct date {
    int  da_year; / * current year * /
    char da_day;  / * day of the month * /
    char da_mon;  / * month (1 = Jan) * /
};
struct dosdate_t {
    unsigned char day;       / * 1-31 * /
    unsigned char month;     / * 1-12 * /
    unsigned int  year;      / * 1980 - 2099 * /
    unsigned char dayofweek; / * 0 - 6 (O=Sunday) * /
};


Date and time in UTC
2022-01-02T17:31:52+00:00
2022-01-02T17:31:52Z
20220102T173152Z

*/

#include "STU_UTIL.h"

#include <stdlib.h>  /* putenv */
#include <stdio.h>   /* printf() */
#include <time.h>    /* time(), tzset() */
/*
    ISO 8601
    UTC
    2022-01-02T17:31:52Z

    Z means zulu time - the time on meridian zero - UTC/GMT.
*/

// struct tm
// {
// int   tm_sec;   /* Seconds */
// int   tm_min;   /* Minutes */
// int   tm_hour;  /* Hour (0 - 23) */
// int   tm_mday;  /* Day of month (1 - 31) */
// int   tm_mon;   /* Month (0 - 11) */
// int   tm_year;  /* Year (calender year minus 1900) */
// int   tm_wday;  /* Weekday (0 - 6; Sunday is 0) */
// int   tm_yday;  /* Day of year (0 -365) */
// int   tm_isdst; /* Nonzero if daylight saving time is in effect. */
// };

void get_datetime(char * datatime)
{

    /* USA - Eastern Standard & Daylight Savings Time */
    // The format of the TZ environment string follows:  TZ = zzz[+/-]d[d][lll]
    // char * tzstr = "TZ=EST5EDT";
    // //char *tzstr = "TZ=EST5";
    char tzstr[11] = "TZ=EST5EDT";
    time_t edtt;
    time_t gmtt;
    struct tm *gmt;
    struct tm *edt;
    char ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";

#pragma warning(suppress : 4996)
    putenv(tzstr);
#pragma warning(suppress : 4996)
    tzset();
    edtt = time(NULL);
    edt = localtime(&edtt);
    // printf("EST5EDT: %s", asctime(edt));
    gmtt = time(NULL);
    gmt = gmtime(&gmtt);
    // printf("UTC/GMT: %s", asctime(gmt));

    // // printf("edt->tm_sec: %d\n", edt->tm_sec);
    // // printf("edt->tm_min: %d\n", edt->tm_min);
    // // printf("edt->tm_hour: %d\n", edt->tm_hour);
    // // printf("edt->tm_mday: %d\n", edt->tm_mday);
    // // printf("edt->tm_mon: %d\n", edt->tm_mon);
    // // printf("edt->tm_year: %d\n", edt->tm_year);
    // // printf("edt->tm_wday: %d\n", edt->tm_wday);
    // // printf("edt->tm_yday: %d\n", edt->tm_yday);
    // // printf("edt->tm_isdst: %d\n", edt->tm_isdst);
    // printf("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", edt->tm_sec, edt->tm_min, edt->tm_hour, edt->tm_mday, edt->tm_mon, edt->tm_year, edt->tm_wday, edt->tm_yday, edt->tm_isdst);
    // // printf("gmt->tm_sec: %d\n", gmt->tm_sec);
    // // printf("gmt->tm_min: %d\n", gmt->tm_min);
    // // printf("gmt->tm_hour: %d\n", gmt->tm_hour);
    // // printf("gmt->tm_mday: %d\n", gmt->tm_mday);
    // // printf("gmt->tm_mon: %d\n", gmt->tm_mon);
    // // printf("gmt->tm_year: %d\n", gmt->tm_year);
    // // printf("gmt->tm_wday: %d\n", gmt->tm_wday);
    // // printf("gmt->tm_yday: %d\n", gmt->tm_yday);
    // // printf("gmt->tm_isdst: %d\n", gmt->tm_isdst);
    // printf("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", gmt->tm_sec, gmt->tm_min, gmt->tm_hour, gmt->tm_mday, gmt->tm_mon, gmt->tm_year, gmt->tm_wday, gmt->tm_yday, gmt->tm_isdst);

    // // printf("%d - %d - %d T %d : %d : %d Z\n", edt->tm_year, edt->tm_mon, edt->tm_mday, edt->tm_hour, edt->tm_min, edt->tm_sec);
    // // printf("%d - %d - %d T %d : %d : %d Z\n", gmt->tm_year, gmt->tm_mon, gmt->tm_mday, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);

    // printf("%4d-%02d-%02dT%02d:%02d:%02dZ\n", (edt->tm_year + 1900), (edt->tm_mon + 1), edt->tm_mday, edt->tm_hour, edt->tm_min, edt->tm_sec);
    // printf("%4d-%02d-%02dT%02d:%02d:%02dZ\n", (gmt->tm_year + 1900), (gmt->tm_mon + 1), gmt->tm_mday, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);

    //sprintf(ISO8601_DateTime, "%4d-%02d-%02dT%02d:%02d:%02dZ", (gmt->tm_year + 1900), (gmt->tm_mon + 1), gmt->tm_mday, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
    // printf("ISO8601_DateTime: %s\n",ISO8601_DateTime);

    sprintf(datatime, "%4d-%02d-%02dT%02d:%02d:%02dZ", (gmt->tm_year + 1900), (gmt->tm_mon + 1), gmt->tm_mday, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);

    // //return ISO8601_DateTime;
}
