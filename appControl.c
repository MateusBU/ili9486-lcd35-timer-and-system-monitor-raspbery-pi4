/* ===========================
*          INCLUDES
* =========================== */
#include "appControl.h"
#include "clock.h"

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
    clock_getCurrentTime(&hour, &min, &sec);

    // Rotate style every second
    if(sec != last_sec) {
        last_sec = sec;
        style = sec % dN_CLOCK_STYLES;
    }
}

/* ===========================
*   LOCAL FUNCTIONS
* =========================== */
