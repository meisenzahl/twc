#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "http_handler.h"
#include "http_status_codes.h"

void error(char * msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void http_handle_client(int sock, char * ip, int port)
{
    int n;
    int buffer_len = 256;
    char buffer[buffer_len];
    char * request;
    size_t request_length = 0;

    char * s;
    char * p;
    int index;

    char * method;
    char * resource;

    request = (char *) malloc(0);

    do {
        memset(buffer, 0, buffer_len);
        n = read(sock, buffer, buffer_len - 1);
        if (n > 0) {
            request = (char *) realloc(request, request_length + n);
            request_length += n;

            strncat(request, buffer, n);
        }
        if (n < buffer_len) {
        	break;
        }
    } while (n > 0);
    if (n < 0) {
        error("ERROR reading from socket");
    }
    else if (n == 0) {
        printf("Socket [%s:%d] was closed.\n", ip, port);
        exit(EXIT_SUCCESS);
    }

    s = strchr(request, ' ');
    index = (int) (s - request);
    method = (char *) malloc(index);
    strncpy(method, request, index);

    p = request + index + 1;
    s = strchr(p, ' ');
    index = (int) (s - p);
    resource = (char *) malloc(index);
    strncpy(resource, p, index);

    printf("%s %s %s\n", ip, method, resource);

    n = write(sock, HTTP_STATUS_CODE_200, strlen(HTTP_STATUS_CODE_200));
    if (n < 0) {
        error("ERROR writing to socket");
    }

    free(request);
    request_length = 0;

    free(method);
}

char * http_get_file(const char * filename)
{
    int fd = open(filename, O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);
    char *data = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);

    return data;
}