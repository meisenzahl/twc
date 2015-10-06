#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

    request = (char *) malloc(0);

    while (1) {
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
        printf("%s:%d\n", ip, port);
        printf("%s", request);

        n = write(sock, HTTP_STATUS_CODE_200, strlen(HTTP_STATUS_CODE_200));
        if (n < 0) {
            error("ERROR writing to socket");
        }

        free(request);
        request_length = 0;
    }
}
