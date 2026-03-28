/* ===========================
*          INCLUDES
* =========================== */
#include "appControl.h"
#include "clock.h"
#include "ili9486.h"

/* ===========================
 *           DEFINES
 * =========================== */

/* ===========================
 *     LOCAL VARIABLES
 * =========================== */

/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */

/* ===========================
 *   GLOBAL FUNCTIONS
 * =========================== */
void appControl_Handler() {
    static int style = 0, last_sec = -1, last_min = -1;
    
    int hour, min, sec;
    clock_readCurrentTime();
    clock_getCurrentTime(&hour, &min, &sec);

    // Rotate style every second
    if(sec != last_sec) {
        last_sec = sec;
        style = sec % dN_CLOCK_STYLES;
        
        clockStyles[style]();
        // Flush everything at once
        fb_flush();
 
        clock_printfCurrentTime();
        fflush(stdout);
    }
}

/* ===========================
*   LOCAL FUNCTIONS
* =========================== */
