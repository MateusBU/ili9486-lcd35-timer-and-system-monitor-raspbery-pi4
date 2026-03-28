/* ===========================
*          INCLUDES
* =========================== */
#include <stdlib.h>
#include <time.h>

#include "clock.h"
/* ===========================
 *           DEFINES
 * =========================== */

/* ===========================
 *     LOCAL VARIABLES
 * =========================== */
time_t now;
struct tm *t;
/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */
static void clock_readCurrentTime();

/* ===========================
 *   GLOBAL FUNCTIONS
 * =========================== */

void clock_printfCurrentTime() {
    printf("%02d/%02d/%04d\n", t->tm_mon + 1, t->tm_mday, t->tm_year + 1900);
    printf("%02d:%02d:%02d\n", t->tm_hour, t->tm_min, t->tm_sec);
}

void clock_getCurrentTime(int *hour, int *minute, int *second) {
    *hour = t->tm_hour;
    *second = t->tm_sec;
    *minute = t->tm_min;
}
/* ===========================
*   LOCAL FUNCTIONS
* =========================== */
static void clock_readCurrentTime() {
    time_t now = time(NULL); //current system date in seconds
    struct tm *t = localtime(&now);
}