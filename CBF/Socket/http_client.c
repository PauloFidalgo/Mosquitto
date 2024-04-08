#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"
#include "../Utils/utils.h"

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

void send_post_gnb_placement_setup_request(char *id, char *x, char *y, char *z) {

    char* json;
    create_gnb_placement_json(&json, x, y, z);

    char request[2048];
    size_t json_len = strlen(json);
    
    sprintf(request, "POST /gnb/placement/ HTTP/1.1\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: %zu\r\n"
                    "\r\n"
                    "%s",
                    json_len, json);
    
    printf("%s\n", request);

    if (send(sock, request, strlen(request), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Send request sent successfully\n");
    }
    
    // Receive and process response
    char response[2048]; // Adjust size as needed
    ssize_t bytes_received = recv(sock, response, sizeof(response), 0);
    if (bytes_received == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data to use it as a string
    response[bytes_received] = '\0';

    // Print the response
    printf("Received response from server:\n%s\n", response); 

    free(json);
}

void receive_get_gnb_placement_setup_request(char* res) {

    char* body = strstr(res, "{");

    printf(body);

    char* x_char = (char*)malloc(20 * sizeof(char)); 
    char* y_char = (char*)malloc(20 * sizeof(char));
    char* z_char = (char*)malloc(20 * sizeof(char));

    parse_gnb_placement_json(body, &x_char, &y_char, &z_char);

    printf("Gnb Placement Status Received\n");
    printf("x_char: %s\ny_char: %s\nz_char: %s", x_char, y_char, z_char);

    free(x_char);
    free(y_char);
    free(z_char);
}

void send_get_gnb_placement_setup_request(char* id) {

    char request[2048];
    sprintf(request, "GET /gnb/placement/ HTTP/1.1\r\n", id);

    if (send(sock, request, strlen(request), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    } else{
        printf("Send request sent successfully\n");
    }

    // Receive and process response
    char response[1024]; // Adjust size as needed
    ssize_t bytes_received = recv(sock, response, sizeof(response), 0);
    if (bytes_received == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    receive_get_gnb_placement_setup_request(response);
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

int main(int argc, char* argv[]) {

    if (argc < 3) {
        return 1;
    }
    
    create_socket();
    connect_to_server();

    if (strcmp(argv[1], "POST") == 0) {
        send_post_gnb_placement_setup_request(argv[2], argv[3], argv[4], argv[5]);
    }
    else {
        send_get_gnb_placement_setup_request(argv[2]);
    }

    close(sock);
    return 0;
}
