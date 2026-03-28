/* ===========================
*          INCLUDES
* =========================== */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "appControl.h"
#include "clock.h"
#include "ili9486.h"
#include "systemMonitor.h"
/* ===========================
 *           DEFINES
 * =========================== */

/* ===========================
 *     LOCAL VARIABLES
 * =========================== */

/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */
static void on_signal(int s);

/* ===========================
 *   GLOBAL FUNCTIONS
 * =========================== */

int main(void) {
    signal(SIGINT,  on_signal);
    signal(SIGTERM, on_signal);

    printf("=== Clock + System Monitor ===\n");

    if(lcd_init() < 0) {
        fprintf(stderr, "LCD init failed\n"); 
        return 1;
    }
       
    
    while(1) {
        appControl_Handler();

        // Sleep ~50ms then check again
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 50000000 };
        nanosleep(&ts, NULL);
    }
    lcd_close();
    return 0;
}
 /* ===========================
 *   LOCAL FUNCTIONS
 * =========================== */
static void on_signal(int s) { 
    (void)s; 
    lcd_close(); 
    exit(0);
}