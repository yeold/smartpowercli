#ifndef SMARTPOWERCLI_H
#define SMARTPOWERCLI_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Constants for request types
#define REQUEST_DATA        0x37
#define REQUEST_STARTSTOP   0x80
#define REQUEST_STATUS      0x81
#define REQUEST_ONOFF       0x82
#define REQUEST_VERSION     0x83

ssize_t send_request(const char* device_path, int request);
ssize_t read_response(const char * device_path, unsigned char* buf, size_t buf_size);

#endif // SMARTPOWERCLI_H