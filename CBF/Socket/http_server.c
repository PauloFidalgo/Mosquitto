#include "http_server.h"
#include <sys/select.h>

int server_fd, client_socket;
struct sockaddr_in address;
int addrlen;


void send_response(int client_socket, HTTP_response response)
{
    if (response.status == OK) {
            // Generate HTTP response
        char http_header[1024];
        sprintf(http_header, "HTTP/1.1 %d\r\nContent-Type: text/plain\r\n\r\n", response.status);

        // Send HTTP header
        send(client_socket, http_header, strlen(http_header), 0);

        // Send body
        send(client_socket, response.body, strlen(response.body), 0);
    }
    else {
        char http_header[1024];
        sprintf(http_header, "HTTP/1.1 %d\r\nContent-Type: text/plain\r\n\r\n", response.status);

        // Send HTTP header
        send(client_socket, http_header, strlen(http_header), 0);
    }
}


char *extract_data_from_URI(char *buffer) {

    char *token = strtok(buffer, "/");
    while (token != NULL)
    {
        if (isdigit(token[0]))
        {
            char *id = strdup(token);
            if (id != NULL)
            {
                for (int i = 0; id[i]; i++)
                {
                    if (!isdigit(id[i]))
                    {
                        id[i] = '\0';
                        break;
                    }
                }
                printf("Extracted ID: %s\n", id);
                return id;
            }
            else
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, "/");
    }
    printf("Failed to extract ID\n");
    return NULL;
    
}

/*
void handle_get_request(int client_socket, const char* url)
{
    size_t num_functions = sizeof(requests) / sizeof(requests[0]);

    for (size_t i = 0; i < num_functions; i++)
    {
        if (strcmp(url, requests[i].request) == 0)
        {
            handle_status_request(*requests[i].get_status, requests[i].message, requests[i].get_topic);
        }
    }
}

void handle_post_request(int client_socket, const char* url, const char* body)
{
    size_t num_functions = sizeof(requests) / sizeof(requests[0]);

    for (size_t i = 0; i < num_functions; i++)
    {
        if (strcmp(url, requests[i].request) == 0) {
            handle_status_request(*requests[i].post_status, body, requests[i].post_topic);
        }
    }
}
*/

void create_server()
{   
    addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SOCKET_PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

/*
void handle_request(void* arg){
     int client_socket = *((int*)arg);
    char request[1024];
    ssize_t bytes_received = recv(client_socket, request, sizeof(request), 0);
    if (bytes_received < 0) {
        perror("Failed");
        close(client_socket);
        pthread_exit(NULL);
    }

    char* request_line_end = strstr(request, "\r\n");
    if (request_line_end != NULL) {
        char method[10], url[1024];
        sscanf(request, "%s %s", method, url);

        if (strcmp(method, "POST") == 0) {
            char* body_start = strstr(request, "\r\n\r\n");
                    if (body_start != NULL) {
                        body_start += 4;
                        
                        HTTP_response response;
                        char* body=(char*)malloc(sizeof(char) * 1024);
                        int res = configuration_received(url, body_start, body);
                        if (res != -1) {
                            response.status = res;
                            response.body = body;
                        }
                        else {
                            response.status = 404;
                        }

                        send_response(client_socket, response);
                        free(body);
                    }
        } else if (strcmp(method, "GET") == 0) {
            
        } else {
            const char* response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
            send(client_socket, response, strlen(response), 0);
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}
*/

void accept_connections()
{
    
    while(1){
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        
        int activity = select(server_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0)
        {
            perror("select");
            continue;
        }

        if (FD_ISSET(server_fd, &read_fds))
        {
            char request[1024];
            if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                continue;
            }

            ssize_t bytes_received = recv(client_socket, request, sizeof(request), 0);
            if (bytes_received < 0) {
                perror("Failed");
                close(client_socket);
                continue;
            }

            char* request_line_end = strstr(request, "\r\n");
            if (request_line_end != NULL) {
                char method[10], url[1024];
                sscanf(request, "%s %s", method, url);

                if (strcmp(method, "POST") == 0) {
                    char* body_start = strstr(request, "\r\n\r\n");
                    if (body_start != NULL) {
                        body_start += 4;
                        
                        HTTP_response response;
                        char* body=(char*)malloc(sizeof(char) * 1024);
                        int res = configuration_received(url, body_start, body);
                        if (res != -1) {
                            response.status = res;
                            response.body = body;
                        }
                        else {
                            response.status = 404;
                        }

                        send_response(client_socket, response);
                        free(body);
                    }
                }
                else if (strcmp(method,"GET") == 0){
                        
                        HTTP_response response;
                        char* body = (char*)malloc(sizeof(char) * 1024);
                        int res = get_status_received(url, body);

                        if (res != -1) {
                            response.status = res;
                            response.body = body;
                        }
                        else {
                            response.status = 404;
                        }

                        send_response(client_socket, response);
                        free(body);
                }
                else {
                    const char* response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
                    send(client_socket, response, strlen(response), 0);
                }
            }

            close(client_socket);
        }
    }

    /*
    while (1)
    {
        char request[1024];
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            continue;
        }

        ssize_t bytes_received = recv(client_socket, request, sizeof(request), 0);
        if (bytes_received < 0) {
            perror("Failed");
            close(client_socket);
            continue;
        }

        char* request_line_end = strstr(request, "\r\n");
        if (request_line_end != NULL) {

            char method[10], url[1024];
            sscanf(request, "%s %s", method, url);


            if (strcmp(method, "POST") == 0) {
                char* body_start = strstr(request, "\r\n\r\n");
                if (body_start != NULL) {
                    body_start += 4;
                    handle_post_request(client_socket, url, body_start);
                }
            }
            else if(strcmp(method,"GET") == 0){
                handle_get_request(client_socket, url);
            }
            else {
                const char* response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
                send(client_socket, response, strlen(response), 0);
            }
        }

        close(client_socket);
    }
    */
}