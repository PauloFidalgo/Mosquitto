#include "CBFMQTT.h"

uint8_t modules_error = 0x00;
uint8_t modules_ack = 0x00;
bool db_fnsh_ack = false;
bool finished = false;
bool started = false;
char* current_ack = ACK_START_EXPERIENCE;
char* current_err = ERROR_START_EXPERIENCE;
struct mosquitto *mosq = NULL;
char* gnb_status = NULL;

typedef struct {
    const char *topic;
    void (*handler)(const struct mosquitto_message *);
} MessageHandler_t;

typedef struct {
    const char *payload;
    unsigned int flag;
} ReplyFlag_t;

typedef struct {
    const char* ack;
    void (*ack_handler)(const struct mosquitto_message *);
} ReplyMessage_t;


static const ReplyFlag_t replySuccessFlag[] = {
    {GNB_SETUP_READY, 0x01},
    {UE_SETUP_READY, 0x02},
    {LIS_SETUP_READY, 0x04},
    {VC_SETUP_READY, 0x08},
    {CODRF_SETUP_READY, 0x10},
};

static const ReplyFlag_t replyErrorFlag[] = {
    {GNB_SETUP_ERROR, 0x01},
    {UE_SETUP_ERROR, 0x02},
    {LIS_SETUP_ERROR, 0x04},
    {VC_SETUP_ERROR, 0x08},
    {CODRF_SETUP_ERROR, 0x10},
};

static const ReplyFlag_t replyFinishSuccessFlag[] = {
    {GNB_FINISH_SUCCESS, 0x01},
    {UE_FINISH_SUCCESS, 0x02},
    {LIS_FINISH_SUCCESS, 0x04},
    {VC_FINISH_SUCCESS, 0x08},
    {CODRF_FINISH_SUCCESS, 0x10},
};

static const ReplyFlag_t replyResetSuccessFlag[] = {
    {GNB_RESET_SUCCESS, 0x01},
    {UE_RESET_SUCCESS, 0x02},
    {LIS_RESET_SUCCESS, 0x04},
    {VC_RESET_SUCCESS, 0x08},
    {CODRF_RESET_SUCCESS, 0x10},
};

void setup_ack_success_handler(const struct mosquitto_message *message){
    const char *payload = (const char *)message->payload;
    size_t numFlags = sizeof(replySuccessFlag) / sizeof(replySuccessFlag[0]);

    for (size_t i = 0; i < numFlags; ++i)
    {
        if (strcmp(payload, replySuccessFlag[i].payload) == 0)
        {
            modules_ack |= replySuccessFlag[i].flag;
        }
    }
}

void finish_ack_success_handler(const struct mosquitto_message *message){
    const char *payload = (const char *)message->payload;
    size_t numFlags = sizeof(replyFinishSuccessFlag) / sizeof(replyFinishSuccessFlag[0]);

    for (size_t i = 0; i < numFlags; ++i)
    {
        if (strcmp(payload, replyFinishSuccessFlag[i].payload) == 0)
        {
            modules_ack |= replyFinishSuccessFlag[i].flag;
        }
    }
}

void reset_ack_success_handler(const struct mosquitto_message *message){
    const char *payload = (const char *)message->payload;
    size_t numFlags = sizeof(replyResetSuccessFlag) / sizeof(replyResetSuccessFlag[0]);

    for (size_t i = 0; i < numFlags; ++i)
    {
        if (strcmp(payload, replyResetSuccessFlag[i].payload) == 0)
        {
            modules_ack |= replyResetSuccessFlag[i].flag;
        }
    }
}

static const ReplyMessage_t replySuccessMessage [] = {
    {ACK_START_EXPERIENCE, setup_ack_success_handler},
    {ACK_FINISH_EXPERIENCE, finish_ack_success_handler},
    {ACK_RESET, reset_ack_success_handler}
};

// pode dar tenda por causa do tamanho dentro do for ( do numErrors)
void setup_ack_error_handler(const struct mosquitto_message *message){
    const char *payload = (const char *)message->payload;
    size_t numErrors = sizeof(replyErrorFlag) / sizeof(replyErrorFlag[0]);

    for (size_t i = 0; i < numErrors; ++i)
    {
        if (strcmp(payload, replyErrorFlag[i].payload) == 0)
        {
            modules_error |= replyErrorFlag[i].flag;
        }
    }
}
void finish_ack_error_handler(const struct mosquitto_message *message){}

void reset_ack_error_handler(const struct mosquitto_message *message){}

static const ReplyMessage_t replyErrorMessage [] = {
    {ERROR_START_EXPERIENCE, setup_ack_error_handler},
    {ERROR_FINISH_EXPERIENCE, finish_ack_error_handler},
    {ERROR_RESET, reset_ack_error_handler}
};

void command_handler(const struct mosquitto_message *message) {
    size_t num_command_handlers = sizeof(replySuccessMessage) / sizeof(replySuccessMessage[0]);

    for (size_t i = 0; i < num_command_handlers; i++) {
        if (strcmp(replySuccessMessage[i].ack, current_ack) == 0) {
            replySuccessMessage[i].ack_handler(message);
        }
    }
}

void handle_schedule_received(const struct mosquitto_message *message){
    printf("[Received]: %s\n", (char *)message->payload);
    started = true;
}

void errors_handler(const struct mosquitto_message *message) {
    size_t num_error_handlers = sizeof(replyErrorMessage) / sizeof(replyErrorMessage[0]);

    for (size_t i = 0; i < num_error_handlers; i++) {
        if (strcmp(replyErrorMessage[i].ack, current_err) == 0) {
            replyErrorMessage[i].ack_handler(message);
        }
    }
}

void gnb_getters_handler(const struct mosquitto_message *message) {
    const char* msg = (const char *)message->payload;

    gnb_status = msg;
}

static const MessageHandler_t messageHandler[] = {
    {COMMAND, command_handler}, 
    {CAF_SCHEDULE, handle_schedule_received},
    {ERRORS, errors_handler},
    {GNB_GETTERS, gnb_getters_handler}
};


void finish_db_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message){
    db_fnsh_ack = true;
}


void on_message_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    const char *topic = (const char *)message->topic;
    size_t num_topics = sizeof(messageHandler) / sizeof(messageHandler[0]);

    for (size_t i = 0; i < num_topics; i++) {
        if (strcmp(topic, messageHandler[i].topic) == 0) {
            messageHandler[i].handler(message);
        }
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc){
    if (rc == 0)
    {
        mosquitto_subscribe(mosq, NULL, CAF_SCHEDULE, 1);
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
        mosquitto_subscribe(mosq, NULL, ERRORS, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

int initial_config(){
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
        fprintf(stderr, "Unable to connect to MQTT broker .\n");
        return 1;
    }

    mosquitto_loop_start(mosq);

    return 0;
}

void wait_for_schedule(){
    while (!started) {};
}

int wait_setup_acknowledge_from_all_modules(){   
    while (modules_error == 0 && modules_ack != 0x04);

    current_ack = ACK_FINISH_EXPERIENCE;
    current_err = ERROR_FINISH_EXPERIENCE;
    modules_ack = 0;

    if (modules_error == 0) return 0;

    modules_error = 0;

    return 1;
}

void send_configuration_to_all_modules(const struct configuration_t *configuration) {
    char *message = "CONFIGURATION";

    mosquitto_publish(mosq, NULL, CGNBCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CLISCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CVCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CORDF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CUECF_SETUP, strlen(message), message, 1, true);
}

void send_start_command() {
    char *message = START_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void send_finish_command(){
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

int wait_finish_acknowledge(){

    while (modules_error == 0 && modules_ack != 0x04);

    current_ack = ACK_RESET;
    current_err = ERROR_RESET;
    modules_error = 0;
    modules_ack = 0;

    if (modules_error == 0)
        return 0;

    return 1;
}

void send_finish_command_to_db(){
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void wait_db_finish_acknowledge(){
    //while (!db_fnsh_ack);
    while(0);
    db_fnsh_ack = false;
}

void reset(){
    modules_ack = 0x00,
    modules_error = 0x00;
    db_fnsh_ack = false;
    finished = false;
    started = false;

    char *message = RESET_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);

    while (modules_ack != 0x04);

    modules_ack = 0;
    modules_error = 0;
    current_ack = ACK_START_EXPERIENCE;
    current_err = ERROR_START_EXPERIENCE;
}

void subscribe_gnb_status() {
    mosquitto_subscribe(mosq, NULL, GNB_GETTERS, 1);
}

void unsubscribe_gnb_status() {
    mosquitto_unsubscribe(mosq, NULL, GNB_GETTERS);
}

void get_gnb_status_mqtt(char* status, char* message) {
    mosquitto_publish(mosq, NULL, GNB_GETTERS, strlen(message), message, 1, true);
    
    int attempt = 10;
    while (gnb_status == NULL && attempt > 0) {
        attempt--;
    }

    if (gnb_status != NULL) {
        status = gnb_status;
    }

    gnb_status = NULL;
}

void destroy(){
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
