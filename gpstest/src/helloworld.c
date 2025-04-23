#include "xil_printf.h"
#include "xparameters.h"
#include "PmodGPS.h"
#include "PmodOLED.h"
#include "xil_cache.h"
#include <string.h>
#include <sleep.h>
#include <stdlib.h>
#include <stdio.h>

#define GPS_UART_BASEADDR   XPAR_PMODGPS_0_AXI_LITE_UART_BASEADDR
#define GPS_GPIO_BASEADDR   XPAR_PMODGPS_0_AXI_LITE_GPIO_BASEADDR
#define OLED_GPIO_BASEADDR  XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR
#define OLED_SPI_BASEADDR   XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR
#define PERIPHERAL_CLK      200000000

PmodGPS GPS;
PmodOLED myOLED;

// GPS_formatSentence()
void parseGPGGA(const char* nmea, char* utc, char* lat, char* lon, char* alt) {
    char temp[128];
    strncpy(temp, nmea, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char* tokens[15] = {0};
    int i = 0;
    char* token = strtok(temp, ",");
    while (token && i < 15) {
        tokens[i++] = token;
        token = strtok(NULL, ",");
    }

    if (i >= 10) {
        snprintf(utc, 16, "%s", tokens[1]);
        snprintf(lat, 16, "%s %s", tokens[2], tokens[3]);
        snprintf(lon, 16, "%s %s", tokens[4], tokens[5]);
        snprintf(alt, 8, "%s %s", tokens[9], tokens[10]);
    } else {
        strcpy(utc, "--");
        strcpy(lat, "--");
        strcpy(lon, "--");
        strcpy(alt, "--");
    }
}

void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_RISCV_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_RISCV_USE_DCACHE
    Xil_DCacheEnable();
#endif
#endif
}

void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_RISCV_USE_ICACHE
    Xil_ICacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_RISCV_USE_DCACHE
    Xil_DCacheDisable();
#endif
#endif
}

void Initialize() {
    xil_printf("[SYS] Initializing GPS...\r\n");
    GPS_begin(&GPS, GPS_GPIO_BASEADDR, GPS_UART_BASEADDR, PERIPHERAL_CLK);
    GPS_setUpdateRate(&GPS, 1000);

    xil_printf("[SYS] Initializing OLED...\r\n");
    OLED_Begin(&myOLED, OLED_GPIO_BASEADDR, OLED_SPI_BASEADDR, 0, 0);
    OLED_ClearBuffer(&myOLED);
    OLED_SetCursor(&myOLED, 0, 0);
    OLED_PutString(&myOLED, "GPS Ready...");
    OLED_Update(&myOLED);
}

void Run() {
    char sentence[128];
    char utc[16], lat[16], lon[16], alt[8];
    int displayIndex = 0;

    while (1) {
        if (GPS_getSentence(&GPS, sentence)) {
            xil_printf("[GPS] Raw Sentence: %s\r\n", sentence);
            char* ggaStart = strstr(sentence, "$GPGGA");
            if (ggaStart) {
                parseGPGGA(ggaStart, utc, lat, lon, alt);

                OLED_ClearBuffer(&myOLED);
                switch (displayIndex) {
                    case 0:
                        OLED_SetCursor(&myOLED, 0, 0);
                        OLED_PutString(&myOLED, "Latitude:");
                        OLED_SetCursor(&myOLED, 0, 1);
                        OLED_PutString(&myOLED, lat);
                        break;
                    case 1:
                        OLED_SetCursor(&myOLED, 0, 0);
                        OLED_PutString(&myOLED, "Longitude:");
                        OLED_SetCursor(&myOLED, 0, 1);
                        OLED_PutString(&myOLED, lon);
                        break;
                    case 2:
                        OLED_SetCursor(&myOLED, 0, 0);
                        OLED_PutString(&myOLED, "Altitude:");
                        OLED_SetCursor(&myOLED, 0, 1);
                        OLED_PutString(&myOLED, alt);
                        break;
                    case 3:
                        OLED_SetCursor(&myOLED, 0, 0);
                        OLED_PutString(&myOLED, "UTC Time:");
                        OLED_SetCursor(&myOLED, 0, 1);
                        OLED_PutString(&myOLED, utc);
                        break;
                }
                OLED_Update(&myOLED);

                displayIndex = (displayIndex + 1) % 4;
            }
        } else {
            OLED_ClearBuffer(&myOLED);
            OLED_SetCursor(&myOLED, 0, 0);
            OLED_PutString(&myOLED, "Waiting for signal");
            OLED_Update(&myOLED);
        }

        sleep(2);
    }
}

int main(void) {
    EnableCaches();
    Initialize();
    Run();
    DisableCaches();
    return 0;
}
