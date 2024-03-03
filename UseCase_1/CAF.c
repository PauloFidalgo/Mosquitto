#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include "headers.h"

struct mosquitto *mosq = NULL;

void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to MQTT broker\n");
        mosquitto_subscribe(mosq, NULL, GNB_RS, 1);
        mosquitto_subscribe(mosq,NULL,UE_RS,1);
        mosquitto_subscribe(mosq,NULL,LIS_RS,1);
        mosquitto_subscribe(mosq,NULL,VIDEO_S,1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
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

int run()
{
    while (1)
    {
        // Publishing a message
        char *message = "Hello, from CAF!";
        mosquitto_publish(mosq, NULL, RECONF, strlen(message), message, 1, true);

        // Sleep for a short time before publishing the next message
        usleep(1000000); // 1 second
    }
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    if (config())
    {
        return 1;
    }

    if (run())
    {
        return 1;
    }

    destroy();

    return 0;
}
