#include <mosquitto.h>
#include "../Utils/headers.h"
#include "utils.h"
#include <unistd.h>
#include "../Utils/topics.h"


struct mosquitto *mosq = NULL;
bool end = false;
bool start = false;
bool shutdown = false;
int x = 10;
int y = 20;
int z = 15;

/*
 * Struct to handle commands received from CBF
 */
typedef struct  {
    char* command;
    void (*function)();
} cbf_command_t;

typedef struct  {
    char* topic;
    void (*function)();
} gnb_get_t;

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
    char *reply = GNB_FINISH_SUCCESS;
    if (mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false) != MOSQ_ERR_SUCCESS){
        printf("Couldn't publish finishing command on topic COMMAND\n");
    }
    end = true;
    printf("Received finish command\n");
}

void reset_command_received() {
    char *reply = GNB_RESET_SUCCESS;
    if(mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false )!= MOSQ_ERR_SUCCESS){
        printf("Couldn't publish reset command on topic COMMAND\n");
    }

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

/*
 * Function to redirect reconfiguration commands 
 */
void handle_beam_config_command(const struct mosquitto_message *message) {
    // To Do
}

// Ainda sem implementação 
void handle_gnb_setup(const struct mosquitto_message *message) {
    char *reply = GNB_SETUP_READY;
    if(mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, false)!= MOSQ_ERR_SUCCESS){
        printf("Couldn't publish\"gnb setup ready\" on topic command\n");
    }
    printf("Received start command\n");
}

void handle_get_gnb_placement_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);

    if(mosquitto_publish(mosq, NULL, GET_GNB_PLACEMENT_ST_ACK_TP, strlen(placement), placement, 1, false) != MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb placement on topic \"GET_GNB_PLACEMENT_ST_TP \"");
    }

    cJSON_free(placement);
}

void handle_get_gnb_radio_comm_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    if(mosquitto_publish(mosq, NULL, GET_GNB_RADIO_COMM_ST_TP, strlen(placement), placement, 1, false) != MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb radio comm on topic \"GET_GNB_RADIO_COMM_ST_TP \"");
    }

    cJSON_free(placement);
}

void handle_get_gnb_radio_sensing_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    if(mosquitto_publish(mosq, NULL, GET_GNB_RADIO_SENSING_ST_TP, strlen(placement), placement, 1, false)!= MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb radio sensing on topic \"GET_GNB_RADIO_SENSING_ST_TP \"");
    }

    cJSON_free(placement);
}

void handle_get_gnb_video_sensing_st(const struct mosquitto_message *message) {
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);

    if(mosquitto_publish(mosq, NULL, GET_GNB_VIDEO_SENSING_ST_TP, strlen(placement), placement, 1, false)!= MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb video sensing on topic \"GET_GNB_VIDEO_SENSING_ST_TP \"");
    }


    cJSON_free(placement);
}

void handle_get_gnb_x_app_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    if(mosquitto_publish(mosq, NULL, GET_GNB_X_APP_ST_TP, strlen(placement), placement, 1, false) != MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb X APP on topic \"GET_GNB_X_APP_ST_TP \"");
    }

    cJSON_free(placement);
}



void parse(char *json_data, char **x, char **y, char **z)
{
    cJSON *json = cJSON_Parse(json_data);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON data.\n");
        cJSON_Delete(json);
        return;
    }

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "x");
    if (cJSON_IsString(temp) && (temp->valuestring != NULL)) {
        *x = temp->valuestring;
    }

    temp = cJSON_GetObjectItemCaseSensitive(json, "y");
    if (cJSON_IsString(temp) && (temp->valuestring != NULL)) {
        *y = temp->valuestring;
    }

    temp = cJSON_GetObjectItemCaseSensitive(json, "z");
    if (cJSON_IsString(temp) && (temp->valuestring != NULL)) {
        *z = temp->valuestring;
    }

    cJSON_Delete(json);
}


void handle_post_gnb_placement_st(const struct mosquitto_message *message) {

    char* x_char = (char*)malloc(10 * sizeof(char)); 
    char* y_char = (char*)malloc(10 * sizeof(char));
    char* z_char = (char*)malloc(10 * sizeof(char));

    parse_gnb_placement_json((char *)message->payload, &x_char, &y_char, &z_char);

    int x_int = 0;
    int y_int = 0;
    int z_int = 0;

    for (int i = 0; x_char[i] != '\0'; i++) {
        x_int = x_int * 10 + (x_char[i] - 48);
    }

    for (int i = 0; y_char[i] != '\0'; i++) {
        y_int = y_int * 10 + (y_char[i] - 48);
    }

    for (int i = 0; z_char[i] != '\0'; i++) {
        z_int = z_int * 10 + (z_char[i] - 48);
    }


    if (x_int >= 0 && y_int >= 0 && z_int >= 0) {
        x = x_int;
        y = y_int;
        z = z_int;
    }
    else {
    }
    

    if (mosquitto_publish(mosq, NULL, POST_GNB_PLACEMENT_ACK_TP, strlen(GET_3D_S_ST_ACK_TP), GET_3D_S_ST_ACK_TP, 1, false)!= MOSQ_ERR_SUCCESS){
        printf("Couldn't publish gnb placement on topic \"POST_GNB_PLACEMENT_ACK_TP \"");
    }

    printf("Published Ack message\n");

    free(x_char);
    free(y_char);
    free(z_char);
}

void handle_post_gnb_radio_comm_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    mosquitto_publish(mosq, NULL, GET_GNB_RADIO_COMM_ST_TP, strlen(placement), placement, 1, false);

    cJSON_free(placement);
}

void handle_post_gnb_radio_sensing_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    mosquitto_publish(mosq, NULL, GET_GNB_RADIO_SENSING_ST_TP, strlen(placement), placement, 1, false);

    cJSON_free(placement);
}

void handle_post_gnb_video_sensing_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    mosquitto_publish(mosq, NULL, GET_GNB_VIDEO_SENSING_ST_TP, strlen(placement), placement, 1, false);

    cJSON_free(placement);
}

void handle_post_gnb_x_app_st(const struct mosquitto_message *message){
    char* placement;
    char x_char[10];
    char y_char[10];
    char z_char[10];

    sprintf(x_char, "%d", x);
    sprintf(y_char, "%d", y);
    sprintf(z_char, "%d", z);

    create_gnb_placement_json(&placement, x_char, y_char, z_char);
    mosquitto_publish(mosq, NULL, GET_GNB_X_APP_ST_TP, strlen(placement), placement, 1, false);

    cJSON_free(placement);
}


/*
 * Struct to map the topic to the corresponding handler
 */
static const on_message_t on_message [] = {
    {COMMAND, handle_cbf_command},
    {CGNBCF_SETUP, handle_gnb_setup},
    {BEAM_CONF, handle_beam_config_command},
    {GET_GNB_PLACEMENT_ST_TP, handle_get_gnb_placement_st},
    {GET_GNB_RADIO_COMM_ST_TP, handle_get_gnb_radio_comm_st},
    {GET_GNB_RADIO_SENSING_ST_TP, handle_get_gnb_radio_sensing_st},
    {GET_GNB_VIDEO_SENSING_ST_TP, handle_get_gnb_video_sensing_st},
    {GET_GNB_X_APP_ST_TP, handle_get_gnb_x_app_st},
    {POST_GNB_PLACEMENT_TP, handle_post_gnb_placement_st},
    {POST_GNB_RADIO_COMM_TP, handle_post_gnb_radio_comm_st},
    {POST_GNB_RADIO_SENSING_TP, handle_post_gnb_radio_sensing_st},
    {POST_GNB_VIDEO_SENSING_TP, handle_post_gnb_video_sensing_st},
    {POST_GNB_X_APP_TP, handle_post_gnb_x_app_st}
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
        mosquitto_subscribe(mosq, NULL, CGNBCF_SETUP, 1);
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
        mosquitto_subscribe(mosq, NULL, BEAM_CONF, 1);
        mosquitto_subscribe(mosq, NULL, GET_GNB_PLACEMENT_ST_TP, 1);
        mosquitto_subscribe(mosq, NULL, GET_GNB_RADIO_COMM_ST_TP, 1);
        mosquitto_subscribe(mosq, NULL, GET_GNB_RADIO_SENSING_ST_TP, 1);
        mosquitto_subscribe(mosq, NULL, GET_GNB_VIDEO_SENSING_ST_TP, 1);
        mosquitto_subscribe(mosq, NULL, GET_GNB_X_APP_ST_TP, 1);
        mosquitto_subscribe(mosq, NULL, POST_GNB_PLACEMENT_TP, 1);
        mosquitto_subscribe(mosq, NULL, POST_GNB_RADIO_COMM_TP, 1);
        mosquitto_subscribe(mosq, NULL, POST_GNB_RADIO_SENSING_TP, 1);
        mosquitto_subscribe(mosq, NULL, POST_GNB_VIDEO_SENSING_TP, 1);
        mosquitto_subscribe(mosq, NULL, POST_GNB_X_APP_TP, 1);
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

    printf("CgNBCF connected to Mosquitto Broker\n");

    while (!start);

    while(!end);

    printf("Experience finished!\n");

    while(!shutdown);

    printf("Shutting down...\n");

    destroy();

    return 0;
}