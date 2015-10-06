#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "http_handler.h"

int main(void)
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr;
    struct sockaddr_storage cli_addr;
    int optval = 1;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    /* Reuse address */
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        error("ERROR rebinding address");
    }

    /* Initialize socket structure */
    memset((char *) &serv_addr, 0, sizeof(serv_addr));    // replaced bzero (deprecated)
    portno = 4000;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind the host address using bind() */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    /* Start listening for the clients */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }

        /* Fork child process */
        int pid = fork();
        if (pid < 0) {
            error("ERROR on fork");
        }
        else if (pid == 0) {
            /* This is the client process */
            close(sockfd);

            char ipstr[INET6_ADDRSTRLEN + 1];
            int cli_port;

            if (cli_addr.ss_family == AF_INET) {
                struct sockaddr_in *s = (struct sockaddr_in *)&cli_addr;
                cli_port = ntohs(s->sin_port);
                inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
            }
            else {
                struct sockaddr_in6 *s = (struct sockaddr_in6 *)&cli_addr;
                cli_port = ntohs(s->sin6_port);
                inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
            }

            http_handle_client(newsockfd, ipstr, cli_port);
            exit(EXIT_SUCCESS);
        }
        else {
            close(newsockfd);
        }
    }

    return EXIT_SUCCESS;
}
