#include "socket.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int server_fd, client_socket;
struct sockaddr_in address;
int addrlen;

typedef struct {
    char *request;
    void (*function)();
} request_t;

char *get_config() {

    return "Sample configuration data\n";
}

void handle_get_config() {
    char *config_data = get_config();
    HTTP_response response;
    response.body = config_data;
    response.status = OK;
    // Send HTTP response
    send_response(client_socket, response);
}

static const request_t requests[] = {
    {GET_CONFIG, handle_get_config},

};

void send_response(int client_socket, HTTP_response response) {
    // Generate HTTP response
    char http_header[1024];
    sprintf(http_header, "HTTP/1.1 %d\r\nContent-Type: text/plain\r\n\r\n", response.status);

    // Send HTTP header
    send(client_socket, http_header, strlen(http_header), 0);

    // Send body
    send(client_socket, response.body, strlen(response.body), 0);
}

void handle_request(int client_socket) {
    // Receive HTTP request
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("Received request: %s\n", buffer);

    size_t num_functions = sizeof(requests) / sizeof(requests[0]);

    for (size_t i = 0; i < num_functions; i++) {
        if (strstr(buffer, requests[i].request) != NULL) {
            requests[i].function();
        }
    }

    // Close connection
    close(client_socket);
}

void create_server() {
    addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SOCKET_PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void accept_connections() {
    // Accept connections and handle requests
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            continue;
        }

        handle_request(client_socket);
    }
}

int main() {
    create_server();
    printf("Server listening on port %d\n", SOCKET_PORT);
    accept_connections();
    return 0;
}
