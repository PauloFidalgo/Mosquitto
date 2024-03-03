#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include <time.h>
#include "headers.h"



uint8_t modules_ready = 0x00;
uint8_t modules_ack = 0x00;
bool finished = false;
struct mosquitto *mosq = NULL;

void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to MQTT broker\n");
        mosquitto_subscribe(mosq, NULL, RECONF, 1);
        mosquitto_subscribe(mosq, NULL, DATA, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void send_reconfig(const struct mosquitto_message *message) {
    mosquitto_publish(mosq, NULL, BEAM_CONF, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, LIS_CONF, strlen(message), message, 1, true);
}

void handle_command_reply(const struct mosquitto_message *message) {
    if (strcmp((char *)message->payload, GNB_READY) == 0) {
        modules_ready |= 0x01;
    }
    else if (strcmp((char *)message->payload, UE_READY) == 0) {
        modules_ready |= 0x02;
    }
    else if (strcmp((char *)message->payload, LIS_READY) == 0) {
        modules_ready |= 0x04;
    }
    else if (strcmp((char *)message->payload, VC_READY) == 0) {
        modules_ready |= 0x08;
    }
    else if (strcmp((char *)message->payload, CODRF_READY) == 0) {
        modules_ready |= 0x10;
    }
    else if (strcmp((char *)message->payload, GNB_ACK) == 0) {
        modules_ack |= 0x01;
    }
    else if (strcmp((char *)message->payload, UE_ACK) == 0) {
        modules_ack |= 0x02;
    }
    else if (strcmp((char *)message->payload, LIS_ACK) == 0) {
        modules_ack |= 0x04;
    }
    else if (strcmp((char *)message->payload, VC_ACK) == 0) {
        modules_ack |= 0x08;
    }
    else if (strcmp((char *)message->payload, CODRF_ACK) == 0) {
        modules_ack |= 0x10;
    }
    // Add more else if conditions for other payloads if needed
}


void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, RECONF) == 0)
    {
        send_reconfig(message);
    }
    else if (strcmp(message->topic, DATA) == 0)
    {
        printf("Irbilhosn\n");
    }
    else if (strcmp(message->topic, COMMAND) == 0)
    {
        handle_command_reply(message);
    }
    // Add more else if conditions for other topics if needed
}

int config()
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);

    if (mosquitto_threaded_set(mosq, true) != MOSQ_ERR_SUCCESS)
    {
        printf("Error: Unable to set threaded mode");
        return 1;
    }
    return 0;
}

void send_finish_command() {
    char* message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void send_start_command() {
    mosquitto_subscribe(mosq, NULL, COMMAND, 1);

    char* message = START_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void timer_handler() {
    send_finish_command();

    //while (modules_ack != 0x1F);

    finished = true;
}

int run()
{
    char porks[100];
    printf("Enter when you want to start\n");
    while (fgets(porks, sizeof(porks), stdin) == NULL || porks[0] == '\n') {
        printf("Please enter something:\n");
    }

    send_start_command();
    //while (modules_ready != 0x1F);

    signal(SIGALRM, timer_handler);
    alarm(5);

    while(!finished);
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    if (config()) return 1;

    if (run()) return 1;

    destroy();

    return 0;
}