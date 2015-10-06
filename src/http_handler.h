#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

void error(char * msg);
void http_handle_client(int sock, char * ip, int port);

#endif
