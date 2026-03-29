/* ===========================
*          INCLUDES
* =========================== */
#include <stdlib.h>
#include <string.h>
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
int dN_CLOCK_STYLES = sizeof(clockStyles) / sizeof(clockStyles[0]);

/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */
 
static void draw_seg7_digit(int x, int y, int digit, uint16_t color, uint16_t bg,
                             int sw, int sh);
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

void clock_digital(int hour, int minute, int second) {
    uint16_t bg = rgb(5, 5, 15);
    uint16_t color = rgb(0, 255, 100);
    uint16_t dim = rgb(20, 20, 30);

    fb_fill_rect(0, 0, LCD_WIDTH, dCLOCK_H, bg);

    // Title
    const char stringTitle[] = "DIGITAL!!!!!!!";
    uint8_t scaleTitle = 2;

    uint16_t titleWidth = fb_string_width(stringTitle, scaleTitle);
    uint16_t titleX = (LCD_WIDTH - titleWidth) / 2;
    fb_draw_string(titleX, 8, stringTitle, rgb(0, 180, 80), bg, scaleTitle);

    int segmentWidth = 26, segmentHeight = 36; //7 segment dimension
    int digitTotalWidth = segmentWidth + 9;

    // HH:MM:SS — centered
    int totalWidth = (6 * digitTotalWidth) + (2 * 10) + (2 * 8); //6digits + 2colons

    int x = (LCD_WIDTH - totalWidth) / 2;
    int y = 60;

    // HOUR
    draw_seg7_digit(x, y, hour/10, color, bg, segmentWidth, segmentHeight);
    draw_seg7_digit(x + digitTotalWidth, y, hour%10, color, bg, segmentWidth, segmentHeight);
    
    // colon
    fb_fill_rect(x+digitTotalWidth*2+2, y+segmentWidth/2-4,  5, 5, color);
    fb_fill_rect(x+digitTotalWidth*2+2, y+segmentWidth+segmentWidth/2, 5, 5, color);
    
    //MINUTE
    draw_seg7_digit(x+digitTotalWidth*2+10, y, minute/10, color, bg, segmentWidth, segmentHeight);
    draw_seg7_digit(x + digitTotalWidth*3+10, y, minute%10, color, bg, segmentWidth, segmentHeight);
    
    // colon
    fb_fill_rect(x+digitTotalWidth*4+18, y+segmentWidth/2-4,  5, 5, color);
    fb_fill_rect(x+digitTotalWidth*4+18, y+segmentWidth+segmentWidth/2, 5, 5, color);
    
    //SECONDS
    draw_seg7_digit(x+digitTotalWidth*4+18+8, y, second/10, color, bg, segmentWidth, segmentHeight);
    draw_seg7_digit(x + digitTotalWidth*5+18+8, y, second%10, color, bg, segmentWidth, segmentHeight);

    // DATE
    char date[32];
    strftime(date, sizeof(date), "%A, %d %B %Y", t); //Example: Sunday, 29 March 2026
    int dlen = strlen(date);
    fb_draw_string((LCD_WIDTH - dlen*6*2)/2, 200, date, rgb(0, 150, 60), bg, 2);

    // DAY OF YEAR / WEEK NUMBER
    strftime(date, sizeof(date), "Week %V -- Day %j", t);
    fb_draw_string((LCD_WIDTH - (int) strlen(date)*6*2)/2, 240, date, rgb(0, 100, 40), bg, 2);
}

void clock_readCurrentTime() {
    now = time(NULL); //current system date in seconds
    t = localtime(&now);
}
/* ===========================
*   LOCAL FUNCTIONS
* =========================== */
// Large 7-segment-style digits using thick rectangles
 
// Segments: a(top) b(top-right) c(bot-right) d(bot) e(bot-left) f(top-left) g(mid)
// Encoding: bit 0=a, 1=b, 2=c, 3=d, 4=e, 5=f, 6=g
static const uint8_t seg7[10] = {
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111, // 9
};
static void draw_seg7_digit(int x, int y, int digit, uint16_t color, uint16_t bg,
                             int sw, int sh) {
    // sw = segment width, sh = segment half-height
    uint8_t segs = seg7[digit];
    int t = 3; // thickness
 
    // Clear digit area
    fb_fill_rect(x, y, sw + t*2, sh*2 + t*3, bg);
 
    // a: top horizontal
    if (segs & (1<<0)) fb_fill_rect(x+t, y, sw, t, color);
    // b: top-right vertical
    if (segs & (1<<1)) fb_fill_rect(x+t+sw, y+t, t, sh, color);
    // c: bot-right vertical
    if (segs & (1<<2)) fb_fill_rect(x+t+sw, y+t+sh+t, t, sh, color);
    // d: bottom horizontal
    if (segs & (1<<3)) fb_fill_rect(x+t, y+t*2+sh*2, sw, t, color);
    // e: bot-left vertical
    if (segs & (1<<4)) fb_fill_rect(x, y+t+sh+t, t, sh, color);
    // f: top-left vertical
    if (segs & (1<<5)) fb_fill_rect(x, y+t, t, sh, color);
    // g: middle horizontal
    if (segs & (1<<6)) fb_fill_rect(x+t, y+t+sh, sw, t, color);
}