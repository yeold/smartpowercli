#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define REQUEST_DATA        0x37
#define REQUEST_STARTSTOP   0x80
#define REQUEST_STATUS      0x81
#define REQUEST_ONOFF       0x82
#define REQUEST_VERSION     0x83

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

    fd = open(device, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Error opening %s: %s\n", device, strerror(errno));
        return 1;
    }

    if (strcmp(argv[2], "data") == 0) 
    {
        tx = REQUEST_DATA;
    } 
    else if (strcmp(argv[2], "startstop") == 0) 
    {
        tx = REQUEST_STARTSTOP;
        ssize_t bytes_written = write(fd, &tx, sizeof(tx));
        if (bytes_written < 0) 
        {
            fprintf(stderr, "Error writing to %s: %s\n", device, strerror(errno));
            return 1;
        }
    } 
    else if (strcmp(argv[2], "status") == 0) 
    {
        tx = REQUEST_STATUS;
        ssize_t bytes_written = write(fd, &tx, sizeof(tx));
        if (bytes_written < 0) 
        {
            fprintf(stderr, "Error writing to %s: %s\n", device, strerror(errno));
            return 1;
        }

        bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read < 0) 
        {
            fprintf(stderr, "Error reading from %s: %s\n", device, strerror(errno));
            return 1;
        }
        printf("Status:\n");
        printf("  Power: %s\n", buf[1] ? "on" : "off");
        printf("  Start/Stop: %s\n", buf[2] ? "on" : "off");
    } 
    else if (strcmp(argv[2], "onoff") == 0) 
    {
        tx = REQUEST_ONOFF;
        ssize_t bytes_written = write(fd, &tx, sizeof(tx));
        if (bytes_written < 0) 
        {
            fprintf(stderr, "Error writing to %s: %s\n", device, strerror(errno));
            return 1;
        }
    } 
    else if (strcmp(argv[2], "version") == 0) 
    {
        tx = REQUEST_VERSION;
        ssize_t bytes_written = write(fd, &tx, sizeof(tx));
        if (bytes_written < 0) {
            fprintf(stderr, "Error writing to %s: %s\n", device, strerror(errno));
            return 1;
        }

        bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read < 0) 
        {
            fprintf(stderr, "Error reading from %s: %s\n", device, strerror(errno));
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

    close(fd);

    printf("Request %s sent successfully.\n", argv[1]);
    return 0;
}