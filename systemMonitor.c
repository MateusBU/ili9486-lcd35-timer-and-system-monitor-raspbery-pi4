/* ===========================
*          INCLUDES
* =========================== */

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

/* ===========================
 *   GLOBAL FUNCTIONS
 * =========================== */
int16_t systemMonitor_getTemperature() {
    FILE *temperatureFile;
    int temperatureValue;
    const char *pathFile = "/sys/class/thermal/thermal_zone0/temp";
    temperatureFile = fopen(pathFile, "r");
    if(temperatureFile == NULL) {
        return -100;
    }
    fscanf(temperatureFile, "%d", &temperatureValue); // read formated data from a file
    temperatureValue /= 1000;
    printf ("The temperature is %d C.\n", temperatureValue);
    fclose(temperatureFile);
    return temperatureValue;
}
/* ===========================
 *   LOCAL FUNCTIONS
 * =========================== */