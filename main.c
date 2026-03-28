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
    printf("USED: %.2f MB, TOTAL: %.2f MB\n", used, total);

    char ip[32]; 
    systemMonitor_getIP(ip, sizeof(ip));
    printf("IP: %s\n", ip);
    
    while(1) {
          
    }
    return 0;
}