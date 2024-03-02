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
        mosquitto_subscribe(mosq, NULL, LIS_CONF, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)
    {
        printf("Received message on topic %s: %s\n", message->topic, (char *)message->payload);
    }
    else
    {
        printf("Received empty message on topic %s\n", message->topic);
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
        char *message = "Hello, MQTT!";
        mosquitto_publish(mosq, NULL, LIS_RS, strlen(message), message, 1, true);

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
