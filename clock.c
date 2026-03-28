/* ===========================
*          INCLUDES
* =========================== */
#include <stdlib.h>
#include <time.h>

#include "clock.h"
#include "ili9486.h"

/* ===========================
 *           DEFINES
 * =========================== */
#define dCLOCK_H   320   // clock area height (top)

/* ===========================
 *     LOCAL VARIABLES
 * =========================== */
time_t now;
struct tm *t;

ClockFn clockStyles[] = {
    clock_digital,
};
const int dN_CLOCK_STYLES = sizeof(clockStyles) / sizeof(clockStyles[0]);
/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */
 
// ─── Color helpers ────────────────────────────────────────────────────────
static inline uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
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

void clock_digital() {
    uint16_t bg = rgb(189, 177, 69);
    uint16_t color = rgb(0, 255, 100);
    uint16_t dim = rgb(20, 20, 30);

    fb_fill_rect(0, 0, LCD_WIDTH, dCLOCK_H, bg);
}

void clock_readCurrentTime() {
    now = time(NULL); //current system date in seconds
    t = localtime(&now);
}
/* ===========================
*   LOCAL FUNCTIONS
* =========================== */