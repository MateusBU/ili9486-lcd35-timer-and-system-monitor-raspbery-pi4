/* ===========================
*          INCLUDES
* =========================== */
#include <stdio.h>
#include <stdint.h>
#include "systemMonitor.h"

// ─── defines ─────────────────────────────────────────────────────────────────

// ─── functions ─────────────────────────────────────────────────────────────────
int main(void) {
    float used = 0, total = 0;
    systemMonitor_getRamUsage(&used, &total);
    printf("USED: %02f, TOTAL: %02f\n", used, total);
    while(1) {
          
    }
    return 0;
}