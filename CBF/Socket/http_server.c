#include "socket.h"
#include <arpa/inet.h>
#include <ctype.h>
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

void send_response(int client_socket, HTTP_response response) {
    // Generate HTTP response
    char http_header[1024];
    sprintf(http_header, "HTTP/1.1 %d\r\nContent-Type: text/plain\r\n\r\n", response.status);

    // Send HTTP header
    send(client_socket, http_header, strlen(http_header), 0);

    // Send body
    send(client_socket, response.body, strlen(response.body), 0);
}

void handle_gnb_placement_status() {
    char *placement_status = "ready\n";
    HTTP_response response;
    response.body = placement_status;
    response.status = OK;
    // Send HTTP response
    send_response(client_socket, response);
}

void handle_gnb_radio_comm_status() {
    printf("ola\n");
    char *placement_status = "hiihihi CAVALOOOOOOOO\n";
    HTTP_response response;
    response.body = placement_status;
    response.status = OK;
    // Send HTTP response
    send_response(client_socket, response);
}
void handle_radio_sensing_status() {}
void handle_video_sensing_status() {}
void handle_Xapp_status() {}
void handle_ue_placement_status() {}
void handle_ue_radio_comm_status() {}
void handle_ue_radio_sensing_status() {}
void handle_ue_video_sensing_status() {}
void handle_lis_placement_status() {}
void handle_lis_radio_comm_status() {}
void handle_lis_radio_sensing_status() {}
void handle_lis_video_sensing_status() {}
void handle_ctcf_status() {}
void handle_3d_status() {}
void handle_vrs_status() {}
void handle_nets_status() {}
void handle_ml_model_download() {}
void handle_odr_dataset_download() {}
void handle_experiment_status() {}

static const request_t requests[] = {
    {GNB_PLACEMENT_STATUS, handle_gnb_placement_status},
    {GNB_RADIO_COMMUNICATIONS_STATUS, handle_gnb_radio_comm_status},
    {GNB_RADIO_SENSING_STATUS, handle_radio_sensing_status},
    {GNB_VIDEO_SENSING_STATUS, handle_video_sensing_status},
    {GNB_XAPP_STATUS, handle_Xapp_status},
    {UE_PLACEMENT_STATUS, handle_ue_placement_status},
    {UE_RADIO_COMMUNICATIONS_STATUS, handle_ue_radio_comm_status},
    {UE_RADIO_SENSING_STATUS, handle_ue_radio_sensing_status},
    {UE_VIDEO_SENSING_STATUS, handle_ue_video_sensing_status},
    {LIS_PLACEMENT_STATUS, handle_lis_placement_status},
    {LIS_RADIO_COMMUNICATIONS_STATUS, handle_lis_radio_comm_status},
    {LIS_RADIO_SENSING_STATUS, handle_lis_radio_sensing_status},
    {LIS_VIDEO_SENSING_STATUS, handle_lis_video_sensing_status},
    {CTCF_STATUS, handle_ctcf_status},
    {_3D_S_STATUS, handle_3d_status},
    {VRS_STATUS, handle_vrs_status},
    {NETS_STATUS, handle_nets_status},
    {ML_MODEL_DOWNLOAD, handle_ml_model_download},
    {ODR_DATASET_DOWNLOAD, handle_odr_dataset_download},
    {EXPERIMENT_STATUS, handle_experiment_status},
};

char *extract_id(char *buffer) {
    // Tokenize the URL string
    char *token = strtok(buffer, "/");
    while (token != NULL) {
        // Check if the token matches the expected pattern for ID
        if (isdigit(token[0])) {
            // Allocate memory for the ID
            char *id = strdup(token);
            if (id != NULL) {
                for (int i = 0; id[i]; i++) {
                    if (!isdigit(id[i])) {
                        id[i] = '\0';
                        break;
                    }
                }
                printf("Extracted ID: %s\n", id);
                return id;
            } else {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
        }
        // Get the next token
        token = strtok(NULL, "/");
    }
    printf("Failed to extract ID\n");
    return NULL; // Return NULL if ID extraction fails
}

void handle_request(int client_socket) {
    // Receive HTTP request
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("Received request: %s\n", buffer);

    char buffer_copy[1024];
    strcpy(buffer_copy, buffer);
    char *id = extract_id(buffer_copy);

    size_t num_functions = sizeof(requests) / sizeof(requests[0]);

    for (size_t i = 0; i < num_functions; i++) {
        char request_template[1024];
        if (sprintf(request_template, requests[i].request, id) < 0) {
            printf("Error constructing the request with the id\n");
        }

        if (strstr(buffer, request_template) != NULL) {
            requests[i].function();
        }
    }
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