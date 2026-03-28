/* ===========================
*          INCLUDES
* =========================== */
#include <string.h>

#include "systemMonitor.h"

/* ===========================
 *           DEFINES
 * =========================== */
#define dPATH_RAM_USAGE "/proc/meminfo"
#define dPATH_TEMPERATURE "/sys/class/thermal/thermal_zone0/temp"
#define dPATH_IP "hostname -I 2>/dev/null | awk '{print $1}'"
/* ===========================
 *     LOCAL VARIABLES
 * =========================== */

/* ===========================
 *    LOCAL PROTOTYPES
 * =========================== */

/* ===========================
 *   GLOBAL FUNCTIONS
 * =========================== */
void systemMonitor_getRamUsage(float *usedMB, float *totalMB) {
    FILE *fileMem = fopen(dPATH_RAM_USAGE, "r");

    if(fileMem == NULL) {
        *usedMB = *totalMB = 0;
        return;
    }

    long totalMem = 0, freeMem = 0, buffers = 0, cached = 0, sreclaimable = 0;
    char key[64]; long val;

    //gets the entire line as a string
    while (fscanf(fileMem, "%63s %ld kB\n", key, &val) == 2) {
        //line on /proc/meminfo = MemTotal:       16384256 kB
        //after fscanf key = "MemTotal:" val = 16384256

        //verify if the actually line is MemTotal
        //strcmp = compare if key is equal to MemTotal:
        if (!strcmp(key, "MemTotal:")) totalMem = val; 
        if (!strcmp(key, "MemFree:")) freeMem  = val;
        if (!strcmp(key, "Buffers:")) buffers = val;
        if (!strcmp(key, "Cached:")) cached  = val;
        if (!strcmp(key, "SReclaimable:")) sreclaimable = val;
    }

    // ------------------
    //       OR
    // ------------------
    // char line[128];

    // gets the entire line into a string
    // while (fgets(line, sizeof(line), fileMem)) {
            //sscanf: 1 = could read the number %ld, 0 = could not
            //save in totalMem
    //     if (sscanf(line, "MemTotal: %ld kB", &totalMem) == 1) continue;
    //     if (sscanf(line, "MemFree: %ld kB", &freeMem) == 1) continue;
    //     if (sscanf(line, "Buffers: %ld kB", &buffers) == 1) continue;
    //     if (sscanf(line, "Cached: %ld kB", &cached) == 1) continue;
    //     if (sscanf(line, "SReclaimable: %ld kB", &sreclaimable) == 1) continue;
    // }

    fclose(fileMem);

    long used = totalMem - freeMem - buffers - cached - sreclaimable;

    *totalMB = totalMem / 1024.0f;
    *usedMB = used / 1024.0f;
}

int16_t systemMonitor_getTemperature() {
    FILE *temperatureFile;
    int temperatureValue;
    const char *pathFile = dPATH_TEMPERATURE;
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

void systemMonitor_getIP(char *buffer, int bufsz) {
    FILE *file = popen(dPATH_IP, "r");

    if(file == NULL) {
        strncpy(buffer, "N/A", bufsz);
        return;
    }
    fgets(buffer, bufsz, file);

    int len = strlen(buffer);
    if(len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    if(strlen(buffer) == 0) {
        strncpy(buffer, "N/A", bufsz);
    }

}
/* ===========================
 *   LOCAL FUNCTIONS
 * =========================== */