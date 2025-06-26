/**************************************************************
* Course: CSC-415-02 Fall 2024
* Name: Juan Segura Rico
* Student ID: 921725126
* GitHub: velojr
* Project: Assignment 6 – Coding a Device Driver
*
* File: segurarico_juan_HW6_main.c
*
* Description: File for testing our translation device driver 
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define IOCTL_SWITCH_MODE _IOW('t', 1, int)

void testTranslation(int fd, const char *word) {
    char buffer[56];
    int writeTest, readTest;

    // Write word to device
    strncpy(buffer, word, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination
    writeTest = write(fd, buffer, strlen(buffer) + 1);
    if (writeTest < 0) {
        perror("Write failed");
        return;
    }

    // Read translation from device
    readTest = read(fd, buffer, sizeof(buffer));
    if (readTest < 0) {
        perror("Read failed");
        return;
    }

    printf("Translated '%s' to '%s'\n", word, buffer);
}

void switchTranslation(int fd, int mode) {
    if (ioctl(fd, IOCTL_SWITCH_MODE, mode) < 0) {
        perror("Failed to switch translation mode");
        return;
    }
    printf("Switched translation mode to %d\n", mode);
}

int main() {
    int fd;

    // Open device
    fd = open("/dev/translator", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }
    printf("Device open success.\n");

    // Test translation from English to Spanish
    printf("\nTesting English to Spanish translation:\n");
    testTranslation(fd, "Hello");
    testTranslation(fd, "Friend");

    // Switch translation mode
    printf("\nSwitching to Spanish to English translation:\n");
    switchTranslation(fd, 1);

    // Test translation from Spanish to English
    testTranslation(fd, "Hola");
    testTranslation(fd, "Amigo");

    // Close device
    close(fd);
    printf("Device closed.\n");

    return EXIT_SUCCESS;
}
