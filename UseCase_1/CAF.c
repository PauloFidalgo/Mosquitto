#include "headers.h"
#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

struct mosquitto *mosq = NULL;
bool start = false;
bool end = false;

void handle_command(const struct mosquitto_message *message)
{
    if (strcmp((char *)message->payload, START_COMMAND) == 0)
    {
        char *reply = CAF_READY;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        start = true;
        printf("Received start command\n");
    }

    if (strcmp((char *)message->payload, FINISH_COMMAND) == 0)
    {
        char *reply = CAF_ACK;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        end = true;
        printf("Received finish command\n");
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, GNB_RS) == 0)
    {
        printf("[Received]: %s\n", (char *)message->payload);
    }
    else if (strcmp(message->topic, UE_RS) == 0)
    {
        printf("[Received]: %s\n", (char *)message->payload);
    }
    else if (strcmp(message->topic, LIS_RS) == 0)
    {
        printf("[Received]: %s\n", (char *)message->payload);
    }
    else if (strcmp(message->topic, VIDEO_S) == 0)
    {
        printf("[Received]: %s\n", (char *)message->payload);
    }
    else if (strcmp(message->topic, COMMAND) == 0)
    {
        handle_command(message);
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to COMMAND broker from CAF\n");
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker from CAF: %s\n", mosquitto_connack_string(rc));
    }
}

int initial_config()
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, initial_connection);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);
    return 0;
}

void config()
{
    mosquitto_subscribe(mosq, NULL, GNB_RS, 1);
    mosquitto_subscribe(mosq, NULL, UE_RS, 1);
    mosquitto_subscribe(mosq, NULL, LIS_RS, 1);
    mosquitto_subscribe(mosq, NULL, VIDEO_S, 1);
    printf("Connected to ALL topics broker\n");
}

int run()
{
    int i = 0;
    while (!end)
    {   
        char* json;
        char msg[20];
        sprintf(msg, "%d", i);
        i++;
        create_json(&json, get_current_time(), CAF, msg);

        mosquitto_publish(mosq, NULL, RECONF, strlen(json), json, 1, true);

        cJSON_free(json);

        usleep(DELAY_US);
    }
    
    return 0;
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    if (initial_config()) return 1;

    while (!start);

    config();

    run();

    destroy();

    return 0;
}
