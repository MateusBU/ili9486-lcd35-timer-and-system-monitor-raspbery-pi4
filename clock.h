#ifndef CLOCK_H
#define CLOCK_H
/* ===========================
 *          INCLUDES
 * =========================== */
#include <stdio.h>
#include <stdint.h>

/* ===========================
 *           DEFINES
 * =========================== */


/* ===========================
 *            TYPES
 * =========================== */
typedef void (*ClockFn)();
extern const int dN_CLOCK_STYLES;
/* ===========================
 *     GLOBAL VARIABLES
 * =========================== */

/* ===========================
 *   FUNCTION DECLARATIONS
 * =========================== */

void clock_printfCurrentTime();
void clock_getCurrentTime(int *hour, int *minute, int *second);
void clock_digital();
void clock_readCurrentTime();
 
extern ClockFn clockStyles[];
#endif