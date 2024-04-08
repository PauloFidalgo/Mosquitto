#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "socket.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../Mosquitto/CBFMQTT.h"
#include "../Utils/requests.h"
#include "../Utils/topics.h"



void send_response(int client_socket, HTTP_response response);
char* extract_data_from_URI(char* buffer);
void create_server();
void accept_connections();


#endif /*_HTTP_SERVER_H_ */

