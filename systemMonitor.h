#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H
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

/* ===========================
 *     GLOBAL VARIABLES
 * =========================== */

/* ===========================
 *   FUNCTION DECLARATIONS
 * =========================== */
void systemMonitor_getRamUsage(float *usedMB, float *totalMB);
int16_t systemMonitor_getTemperature();
void systemMonitor_getIP(char *buffer, int bufsz);

#endif