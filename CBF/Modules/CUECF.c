#include <mosquitto.h>
#include "../Utils/headers.h"
#include "utils.h"
#include <unistd.h>


struct mosquitto *mosq = NULL;
bool end = false;
bool start = false;
bool shutdown = false;

/*
 * Struct to handle commands received from CBF
 */
typedef struct  {
    char* command;
    void (*function)();
} cbf_command_t;

/*
 *  Struct to handle messages coming from a specific topic
 */
typedef struct  {
    char* topic;
    void (*redirect_message)(const struct mosquitto_message *message);
} on_message_t;

/*
 * Function to be called when the Start command was received
 */
void start_command_received(){
    start = true;
}

/*
 * Function to be called when the Finish command was received
 */
void finish_command_received() {
    char *reply = UE_FINISH_SUCCESS;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false);
    end = true;
    printf("Received finish command\n");
}

void reset_command_received() {
    char *reply = UE_RESET_SUCCESS;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false);
    printf("Received reset command\n");
    end = false;
    start = false;
    shutdown = true;
}

/*
 * Struct to map the command to the corresponding function
 */
static const cbf_command_t cbf_commands [] = {
    {START_COMMAND, start_command_received},
    {FINISH_COMMAND, finish_command_received},
    {RESET_COMMAND, reset_command_received}
};

/*
 * Function to redirect CBF commands to the corresponding function
 */
void handle_cbf_command(const struct mosquitto_message *message) {
    const char *payload = (const char *)message->payload;
    size_t num_functions = sizeof(cbf_commands) / sizeof(cbf_commands[0]);

    for (size_t i = 0; i < num_functions; i++) {
        if (strcmp(payload, cbf_commands[i].command) == 0) {
            cbf_commands[i].function();
        }
    }
}


// Ainda sem implementação 
void handle_ue_setup(const struct mosquitto_message *message) {
    char *reply = UE_SETUP_READY;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false);
    printf("Received start command\n");
}

/*
 * Struct to map the topic to the corresponding handler
 */
static const on_message_t on_message [] = {
    {COMMAND, handle_cbf_command},
    {CUECF_SETUP, handle_ue_setup},
};


/*
 * Function to redirect messages to their corresponding handler 
 */
void on_message_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    const char* topic = (const char*)message->topic;
    size_t num_topics = sizeof(on_message) / sizeof(on_message[0]);

    for (size_t i = 0; i < num_topics; i++) {
        if (strcmp(topic, on_message[i].topic) == 0) {
            on_message[i].redirect_message(message);
        }
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc){
    if (rc == 0)
    {
        mosquitto_subscribe(mosq, NULL, CUECF_SETUP, 1);
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

int initial_configuration() {
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, initial_connection);
    mosquitto_message_callback_set(mosq, on_message_received);
    

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);
    return 0;
}

void destroy(){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main() {
    if (initial_configuration()) {
        printf("Error in configuration\n");
    }

    while (!start);

    while(!end);

    printf("Experience finished!\n");

    while(!shutdown);

    printf("Shutting down...\n");

    destroy();

    return 0;
}