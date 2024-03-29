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
        char *reply = LIS_READY;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        start = true;
        printf("Received start command\n");
    }

    if (strcmp((char *)message->payload, FINISH_COMMAND) == 0)
    {
        char *reply = LIS_ACK;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        end = true;
        printf("Received finish command\n");
    }
}


void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, LIS_CONF) == 0)
    {
        // Handle LIS_CONF topic
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

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to COMMAND broker from CLISCF\n");
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker from CLISCF: %s\n", mosquitto_connack_string(rc));
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
    mosquitto_subscribe(mosq, NULL, LIS_CONF, 1);
    printf("Subscribed topic LIS_CONFIG\n");
}

int run()
{
    while (!end)
    {

        char* json;
        create_json(&json, get_current_time(), CLISCF, "lis_radio_sensing" );

        mosquitto_publish(mosq, NULL, LIS_RS, strlen(json), json, 1, true);

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
