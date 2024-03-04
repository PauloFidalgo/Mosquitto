#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include "headers.h"
#include "utils.h"

struct mosquitto *mosq = NULL;
bool start = false;
bool end = false;

void handle_command(const struct mosquitto_message *message)
{

    if (strcmp((char *)message->payload, START_COMMAND) == 0)
    {
        char *reply = UE_READY;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        start = true;
        printf("Received start command\n");
    }

    if (strcmp((char *)message->payload, FINISH_COMMAND) == 0)
    {
        char *reply = UE_ACK;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        end = true;
        printf("Received finish command\n");
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, COMMAND) == 0)
    {
        handle_command(message);
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to COMMAND broker from CUECF\n");
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
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

int run()
{
    while (!end)
    {
    
        char* json;
        char* json1;

        create_json(&json, get_current_time(), CUECF, "data");
        create_json(&json1, get_current_time(), CUECF, "ue_radio_sensing");

        mosquitto_publish(mosq, NULL, DATA, strlen(json), json, 1, true);
        mosquitto_publish(mosq, NULL, UE_RS, strlen(json1), json1, 1, true);

        cJSON_free(json);
        cJSON_free(json1);
        usleep(1000000);
    
    }
    return 0;
}

void config()
{
    mosquitto_message_callback_set(mosq, on_message);
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    if (initial_config()) // check
        return 1;

    while (!start)
        ;

    config();

    if (run())
        return 1;

    destroy();

    return 0;
}
