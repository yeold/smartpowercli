/**
 * @file smartpowercli.c
 * @brief Command-line interface for interacting with a the Odroid Smart Power 1.
 * 
 * Usage: ./smartpowercli <path to device descriptor> <command>
 * Commands: onoff startstop status data version
 * 
 * Example: ./smartpowercli /dev/hidraw0 onoff
 */

#include "smartpowercli.h"
#include <stdio.h>
#include <string.h>

int open_device(const char* device_path);

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <path to device descriptor> <command>\n", argv[0]);
        printf("Commands: onoff startstop status data version\n");
        return 1;
    }

    int fd;
    unsigned char buf[256];
    ssize_t bytes_read;
    int tx;
    if (argv[1] == NULL) 
    {
        return 1;
    }
    else if
    (argv[2] == NULL) 
    {
        return 1;
    }
    char* device = argv[1];

    fd = open_device(device);
    if (fd < 0) {
        return 1;
    }

    if (strcmp(argv[2], "data") == 0) 
    {
        tx = REQUEST_DATA;

        ssize_t bytes_written = send_request(device, tx);
        if (bytes_written < 0) 
        {
            return 1;
        }

        bytes_read = read_response(device, buf, sizeof(buf));
        if (bytes_read < 0) 
        {
            return 1;
        }

        char voltage[12];
        char amperage[12];
        char wattage[12];
        char watthour[12];
        strncpy(voltage, (char*)&buf[2], 5);
        strncpy(amperage, (char*)&buf[10], 5);
        strncpy(wattage, (char*)&buf[17], 6);
        strncpy(watthour, (char*)&buf[24], 7);

        printf("Data:\n");
        printf("  Voltage: %.3f V\n", atof(voltage));
        printf("  Amperage: %.3f A\n", atof(amperage));
        printf("  Wattage: %.3f W\n", atof(wattage));
        printf("  Watthour: %.3f Wh\n", atof(watthour));
    } 
    else if (strcmp(argv[2], "startstop") == 0) 
    {
        tx = REQUEST_STARTSTOP;
        ssize_t bytes_written = send_request(device, tx);
        if (bytes_written < 0) 
        {
            return 1;
        }
    }       
    else if (strcmp(argv[2], "status") == 0) 
    {
        tx = REQUEST_STATUS;
        ssize_t bytes_written = send_request(device, tx);
        if (bytes_written < 0) 
        {
            return 1;
        }

        bytes_read = read_response(device, buf, sizeof(buf));
        if (bytes_read < 0) 
        {
            return 1;
        }
        printf("Status:\n");
        printf("  Start/stop: %s\n", buf[1] ? "on" : "off");
        printf("  On/off: %s\n", buf[2] ? "on" : "off");
    } 
    else if (strcmp(argv[2], "onoff") == 0) 
    {
        tx = REQUEST_ONOFF;
        ssize_t bytes_written = send_request(device, tx);
        if (bytes_written < 0) 
        {
            return 1;
        }
    } 
    else if (strcmp(argv[2], "version") == 0) 
    {
        tx = REQUEST_VERSION;
        ssize_t bytes_written = send_request(device, tx);
        if (bytes_written < 0) {
            return 1;
        }

        bytes_read = read_response(device, buf, sizeof(buf));
        if (bytes_read < 0) 
        {
            return 1;
        }
        char version[12];
        strncpy(version, (char*)&buf[12], 5);
        printf("Version: %s\n", version);
    }       
    else 
    {
        fprintf(stderr, "Invalid request type: %s\n", argv[1]);
        return 1;
    }

    return 0;
}

int open_device(const char* device_path) 
{
    int fd = open(device_path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Error opening %s: %s\n", device_path, strerror(errno));
        return 1;
    }
    return fd;
}

ssize_t send_request(const char* device_path, int request)
{
    int fd = open_device(device_path);

    ssize_t size = sizeof(request);
    ssize_t bytes_written = write(fd, &request, size);

    close(fd);

    if (bytes_written < 0) {
        fprintf(stderr, "Error writing to device: %s\n", strerror(errno));
        return 1;
    }
    return bytes_written;
}

ssize_t read_response(const char * device_path, unsigned char* buf, size_t buf_size)
{
    int fd = open_device(device_path);

    ssize_t bytes_read = read(fd, buf, buf_size);

    close(fd);

    if (bytes_read < 0) {
        fprintf(stderr, "Error reading from device: %s\n", strerror(errno));
        return 1;
    }
    return bytes_read;
}
