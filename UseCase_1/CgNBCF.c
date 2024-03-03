#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include "headers.h"

struct mosquitto *mosq = NULL;
bool start = false;
bool end = false;

int handle_command(const struct mosquitto_message *message) {
    if (strcmp((char *)message->payload, START_COMMAND) == 0) {
        char* reply = GNB_READY;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        start = true;
        printf("Received start command\n");
        return 0;
    }

    if (strcmp((char *)message->payload, FINISH_COMMAND) == 0) {
        char* reply = GNB_ACK;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        end = true;
        printf("Received finish command\n");
        return 0;
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to COMMAND broker from CgNBCF\n");
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to BEAM CONFIG broker\n");
        mosquitto_subscribe(mosq, NULL, BEAM_CONF, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, BEAM_CONF) == 0)
    {
        // Handle BEAM_CONF topic
    }
    else if (strcmp(message->topic, COMMAND) == 0)
    {
        handle_command(message);
    }
    else
    {
        // Default case
    }
}


int initial_config() {
     mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, initial_connection);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);
    /*
    if (mosquitto_threaded_set(mosq, true) != MOSQ_ERR_SUCCESS)
    {
        printf("Error: Unable to set threaded mode");
        return 1;
    }
    */
    return 0;
}


int config()
{

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    return 0;
}

int idle() {
    mosquitto_message_callback_set(mosq, on_message);
}

int run()
{
    while (start && !end)
    {
        // Publishing a message
        char *message = "Hello, MQTT!";
        mosquitto_publish(mosq, NULL, GNB_RS, strlen(message), message, 1, true);

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
    if (initial_config()) return 1;

    while (!start) idle();

    if (config()) return 1;

    if (run()) return 1;

    destroy();

    return 0;
}
