//gcc -o mastercode mastercode.c -lwiringPi
// ./mastercode.c

#include <wiringPi.h> 
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>

#define SLAVE_ADDRESS 0x11  // Het I2C-adres van de STM32 slave
#define SLAVE_ADDRESS2 0x13

int main() {
    int fd;
    int fb;
    int button_status;

    // Initialiseer wiringPi en I2C
    if (wiringPiSetup() == -1) {
        printf("Fout bij het instellen van wiringPi\n");
        return 1;
    }

    // Open de I2C bus en connecteer met de slave
    fd = wiringPiI2CSetup(SLAVE_ADDRESS);
    if (fd == -1) {
        printf("Fout bij het openen van slave 1\n");
        return 1;
    }
    fb = wiringPiI2CSetup(SLAVE_ADDRESS2);
    if (fb == -1) {
        printf("Fout bij het openen van slave 2\n");
        return 1;
    }

    // Main loop om de knopstatus van de slave op te halen
    while (1) {
        // Lees een byte van de STM32 slave
        button_status = wiringPiI2CRead(fd);
        
        if (button_status == 1) {
            printf("Knop is ingedrukt\n");
            wiringPiI2CWrite(fb, '1');
        } else {
            printf("Knop is niet ingedrukt\n");
            wiringPiI2CWrite(fb, '0');
        }
    }

    return 0;
}
