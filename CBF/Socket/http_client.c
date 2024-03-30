#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"

int sock;

void create_socket() {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
}

void connect_to_server() {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SOCKET_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
}

void send_request(char *request, char *id) {
    char message[1024];
    sprintf(message, request, id);

    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
}

void receive_response() {
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = read(sock, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytes_received, stdout);
    }

    if (bytes_received < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    create_socket();
    connect_to_server();
    send_request(GNB_RADIO_COMMUNICATIONS_STATUS, "256");
    receive_response();
    close(sock);
    return 0;
}
