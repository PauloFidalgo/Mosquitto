#include "CBFMQTT.h"

uint8_t modules_error = 0x00;
uint8_t modules_ack = 0x00;
bool db_fnsh_ack = false;
bool finished = false;
bool started = false;
char *current_ack = ACK_START_EXPERIMENT;
char *current_err = ERROR_START_EXPERIMENT;
struct mosquitto *mosq = NULL;
int attempts = 3;

char *gnb_placement_status = NULL;
char *gnb_radio_comm_status = NULL;
char *gnb_radio_sensing_status = NULL;
char *gnb_video_sensing_status = NULL;
char *gnb_x_app_status = NULL;

// UE Status
char *ue_placement_status = NULL;
char *ue_radio_comm_status = NULL;
char *ue_radio_sensing_status = NULL;
char *ue_video_sensing_status = NULL;

// LIS Status
char *lis_placement_status = NULL;
char *lis_radio_comm_status = NULL;
char *lis_radio_sensing_status = NULL;
char *lis_video_sensing_status = NULL;

// CTCF Status
char *ctcf_status = NULL;

/* ------------  SETTERS ------------ */
// GNB Setup Ack
char *gnb_placement_setup_ack = NULL;
char *gnb_radio_comm_setup_ack = NULL;
char *gnb_radio_sensing_setup_ack = NULL;
char *gnb_video_sensing_setup_ack = NULL;
char *gnb_x_app_setup_ack = NULL;

// UE Setup Ack
char *ue_placement_setup_ack = NULL;
char *ue_radio_comm_setup_ack = NULL;
char *ue_radio_sensing_setup_ack = NULL;
char *ue_video_sensing_setup_ack = NULL;

// LIS Setup Ack
char *lis_placement_setup_ack = NULL;
char *lis_radio_comm_setup_ack = NULL;
char *lis_radio_sensing_setup_ack = NULL;
char *lis_video_sensing_setup_ack = NULL;

// CTCF Setup Ack
char *ctcf_setup_ack = NULL;


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

void get_gnb_placement_handler(const struct mosquitto_message *message) {
    gnb_placement_status = strdup((char *)message->payload);
}

void get_gnb_radio_comm_handler(const struct mosquitto_message *message) {
    gnb_radio_comm_status = (char *)message->payload;
}

void get_gnb_radio_sensing_handler(const struct mosquitto_message *message) {
    gnb_radio_sensing_status = (char *)message->payload;
}

void get_gnb_video_sensing_handler(const struct mosquitto_message *message) {
    gnb_video_sensing_status = (char *)message->payload;
}

void get_gnb_x_app_handler(const struct mosquitto_message *message) {
    gnb_x_app_status = (char *)message->payload;
}

void get_ue_placement_handler(const struct mosquitto_message *message) {
    ue_placement_status = (char *)message->payload;
}

void get_ue_radio_comm_handler(const struct mosquitto_message *message) {
    ue_radio_comm_status = (char *)message->payload;
}

void get_ue_radio_sensing_handler(const struct mosquitto_message *message) {
    ue_radio_sensing_status = (char *)message->payload;
}

void get_ue_video_sensing_handler(const struct mosquitto_message *message) {
    ue_video_sensing_status = (char *)message->payload;
}

void get_lis_placement_handler(const struct mosquitto_message *message) {
    lis_placement_status = (char *)message->payload;
}

void get_lis_radio_comm_handler(const struct mosquitto_message *message) {
    lis_radio_comm_status = (char *)message->payload;
}

void get_lis_radio_sensing_handler(const struct mosquitto_message *message) {
    lis_radio_sensing_status = (char *)message->payload;
}

void get_lis_video_sensing_handler(const struct mosquitto_message *message) {
    lis_video_sensing_status = (char *)message->payload;
}


void setup_ack_success_handler(const struct mosquitto_message *message)
{
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

void finish_ack_success_handler(const struct mosquitto_message *message)
{
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

void reset_ack_success_handler(const struct mosquitto_message *message)
{
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

static const ReplyMessage_t replySuccessMessage[] = {
    {ACK_START_EXPERIMENT, setup_ack_success_handler},
    {ACK_FINISH_EXPERIMENT, finish_ack_success_handler},
    {ACK_RESET, reset_ack_success_handler}};

void setup_ack_error_handler(const struct mosquitto_message *message)
{
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

void finish_ack_error_handler(const struct mosquitto_message *message) {}

void reset_ack_error_handler(const struct mosquitto_message *message) {}

static const ReplyMessage_t replyErrorMessage[] = {
    {ERROR_START_EXPERIMENT, setup_ack_error_handler},
    {ERROR_FINISH_EXPERIMENT, finish_ack_error_handler},
    {ERROR_RESET, reset_ack_error_handler}};

void command_handler(const struct mosquitto_message *message)
{
    size_t num_command_handlers = sizeof(replySuccessMessage) / sizeof(replySuccessMessage[0]);

    for (size_t i = 0; i < num_command_handlers; i++)
    {
        if (strcmp(replySuccessMessage[i].ack, current_ack) == 0)
        {
            replySuccessMessage[i].ack_handler(message);
        }
    }
}

void handle_schedule_received(const struct mosquitto_message *message)
{
    printf("[Received]: %s\n", (char *)message->payload);
    started = true;
}

void errors_handler(const struct mosquitto_message *message)
{
    size_t num_error_handlers = sizeof(replyErrorMessage) / sizeof(replyErrorMessage[0]);

    for (size_t i = 0; i < num_error_handlers; i++)
    {
        if (strcmp(replyErrorMessage[i].ack, current_err) == 0)
        {
            replyErrorMessage[i].ack_handler(message);
        }
    }
}

void post_gnb_placement_ack_handler(const struct mosquitto_message *message) {
    gnb_placement_setup_ack = (char *)message->payload;
}

static const MessageHandler_t messageHandler[] = {
    {COMMAND, command_handler},
    {CAF_SCHEDULE, handle_schedule_received},
    {ERRORS, errors_handler},
    {GET_GNB_PLACEMENT_ST_ACK_TP, get_gnb_placement_handler},
    {POST_GNB_PLACEMENT_ACK_TP, post_gnb_placement_ack_handler}
    };

void finish_db_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    db_fnsh_ack = true;
}

void on_message_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    const char *topic = (const char *)message->topic;
    size_t num_topics = sizeof(messageHandler) / sizeof(messageHandler[0]);

    for (size_t i = 0; i < num_topics; i++)
    {
        if (strcmp(topic, messageHandler[i].topic) == 0)
        {
            messageHandler[i].handler(message);
        }
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        const char* topics[] = {
            CAF_SCHEDULE,
            COMMAND,
            ERRORS,
            GET_GNB_PLACEMENT_ST_ACK_TP,
            GET_GNB_RADIO_COMM_ST_ACK_TP,
            GET_GNB_RADIO_SENSING_ST_ACK_TP,
            GET_GNB_VIDEO_SENSING_ST_ACK_TP,
            GET_GNB_X_APP_ST_ACK_TP,
            GET_UE_PLACEMENT_ST_ACK_TP,
            GET_UE_RADIO_COMM_ST_ACK_TP,
            GET_UE_RADIO_SENSING_ST_ACK_TP,
            GET_UE_VIDEO_SENSING_ST_ACK_TP,
            GET_LIS_PLACEMENT_ST_ACK_TP,
            GET_LIS_RADIO_COMM_ST_ACK_TP,
            GET_LIS_RADIO_SENSING_ST_ACK_TP,
            GET_LIS_VIDEO_SENSING_ST_ACK_TP,
            GET_CTCF_ST_ACK_TP,
            GET_3D_S_ST_ACK_TP,
            GET_VRS_ST_ACK_TP,
            GET_ML_DW_ACK_TP,
            GET_ODR_DW_ACK_TP,
            GET_EXP_ST_ACK_TP,
            POST_GNB_PLACEMENT_ACK_TP,
            POST_GNB_RADIO_COMM_ACK_TP,
            POST_GNB_RADIO_SENSING_ACK_TP,
            POST_GNB_VIDEO_SENSING_ACK_TP,
            POST_GNB_X_APP_ACK_TP,
            POST_UE_PLACEMENT_ACK_TP,
            POST_UE_RADIO_COMM_ACK_TP,
            POST_UE_RADIO_SENSING_ACK_TP,
            POST_UE_VIDEO_SENSING_ACK_TP,
            POST_LIS_PLACEMENT_ACK_TP,
            POST_LIS_RADIO_COMM_ACK_TP,
            POST_LIS_RADIO_SENSING_ACK_TP,
            POST_LIS_VIDEO_SENSING_ACK_TP,
            POST_CTCF_ACK_TP,
            POST_3D_S_ACK_TP,
            POST_VRS_ACK_TP,
            POST_ML_DW_ACK_TP,
            POST_ODR_DW_ACK_TP,
            POST_EXP_ACK_TP
        };

        if (mosquitto_subscribe_multiple(mosq, NULL, sizeof(topics) / sizeof(char*), topics, 1, 0, NULL) != MOSQ_ERR_SUCCESS){
            printf("Error subscribing topics\n");
        } 
    }
    
    else {
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
    mosquitto_message_callback_set(mosq, on_message_received);


    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker .\n");
        return 1;
    }

    mosquitto_loop_start(mosq);

    return 0;
}

void wait_for_schedule()
{
    while (!started)
    {
    };
    started = false;
    mosquitto_unsubscribe(mosq, NULL, CAF_SCHEDULE);
}

void setup_timeout_handler()
{
    char *message = "CONFIGURATION";
    if (!(modules_ack & 0x01))
    {
        mosquitto_publish(mosq, NULL, CGNBCF_SETUP, strlen(message), message, 1, false);
    }
    if (!(modules_ack & 0x02))
    {
        mosquitto_publish(mosq, NULL, CUECF_SETUP, strlen(message), message, 1, false);
    }
    if (!(modules_ack & 0x04))
    {
        mosquitto_publish(mosq, NULL, CLISCF_SETUP, strlen(message), message, 1, false);
    }
    attempts--;
}

int wait_setup_acknowledge_from_all_modules(uint8_t *modules)
{
    signal(SIGALRM, setup_timeout_handler);
    alarm(5);

    uint8_t last_ack = 0;
    int last_attempts = 3;
    attempts = 3;

    while (modules_error == 0 && modules_ack != 0x01 && attempts > 0)
    {
        if (last_ack != modules_ack)
        {
            alarm(5);
            last_ack = modules_ack;
        }
        if (last_attempts != attempts)
        {
            alarm(5);
            last_attempts = attempts;
        }
    }

    current_ack = ACK_FINISH_EXPERIMENT;
    current_err = ERROR_FINISH_EXPERIMENT;

    *modules = modules_ack;
    modules_ack = 0;

    if (attempts <= 0)
    {
        return 1;
    }

    if (modules_error == 0)
        return 0;

    modules_error = 0;

    return -1;
}

void send_configuration_to_all_modules(const configuration_t *configuration)
{
    char *message = "CONFIGURATION";

    mosquitto_publish(mosq, NULL, CGNBCF_SETUP, strlen(message), message, 1, false);
    mosquitto_publish(mosq, NULL, CLISCF_SETUP, strlen(message), message, 1, false);
    mosquitto_publish(mosq, NULL, CVCF_SETUP, strlen(message), message, 1, false);
    mosquitto_publish(mosq, NULL, CORDF_SETUP, strlen(message), message, 1, false);
    mosquitto_publish(mosq, NULL, CUECF_SETUP, strlen(message), message, 1, false);
}

void send_start_command()
{
    char *message = START_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, false);
}

void send_finish_command()
{
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, false);
}

int wait_finish_acknowledge()
{

    while (modules_error == 0 && modules_ack != 0x01);

    current_ack = ACK_RESET;
    current_err = ERROR_RESET;
    modules_error = 0;
    modules_ack = 0;

    if (modules_error == 0)
        return 0;

    return 1;
}

void send_finish_command_to_db()
{
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, false);
}

void wait_db_finish_acknowledge()
{
    // while (!db_fnsh_ack);
    while (0);
    db_fnsh_ack = false;
}

void reset()
{
    modules_ack = 0x00,
    modules_error = 0x00;
    db_fnsh_ack = false;
    finished = false;
    started = false;

    char *message = RESET_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, false);

    while (modules_ack != 0x01);

    modules_ack = 0;
    modules_error = 0;
    current_ack = ACK_START_EXPERIMENT;
    current_err = ERROR_START_EXPERIMENT;
}

static request_t get_requests[] = {
    {GNB_PLACEMENT_STATUS, GET_GNB_PLACEMENT_ST_TP, &gnb_placement_status},
    {GNB_RADIO_COMMUNICATIONS_STATUS, GET_GNB_RADIO_COMM_ST_TP, &gnb_radio_comm_status},
    {GNB_RADIO_SENSING_STATUS, GET_GNB_RADIO_SENSING_ST_TP, &gnb_radio_sensing_status},
    {GNB_VIDEO_SENSING_STATUS, GET_GNB_VIDEO_SENSING_ST_TP, &gnb_video_sensing_status},
    {GNB_X_APP_STATUS, GET_GNB_X_APP_ST_TP, &gnb_x_app_status},

    {UE_PLACEMENT_STATUS, GET_UE_PLACEMENT_ST_TP, &ue_placement_status},
    {UE_RADIO_COMMUNICATIONS_STATUS, GET_UE_RADIO_COMM_ST_TP, &ue_radio_comm_status},
    {UE_RADIO_SENSING_STATUS, GET_UE_RADIO_SENSING_ST_TP, &ue_radio_sensing_status},
    {UE_VIDEO_SENSING_STATUS,GET_UE_VIDEO_SENSING_ST_TP, &ue_video_sensing_status},

    {LIS_PLACEMENT_STATUS,GET_LIS_PLACEMENT_ST_TP, &lis_placement_status},
    {LIS_RADIO_COMMUNICATIONS_STATUS, GET_LIS_RADIO_COMM_ST_TP, &lis_radio_comm_status},
    {LIS_RADIO_SENSING_STATUS, GET_LIS_RADIO_SENSING_ST_TP, &lis_radio_sensing_status},
    {LIS_VIDEO_SENSING_STATUS, GET_LIS_VIDEO_SENSING_ST_TP, &lis_video_sensing_status},
};

static request_t post_requests [] = {
    {GNB_PLACEMENT_STATUS, POST_GNB_PLACEMENT_TP, &gnb_placement_setup_ack},
    {GNB_RADIO_COMMUNICATIONS_STATUS, POST_GNB_RADIO_COMM_TP, &gnb_radio_comm_setup_ack},
    {GNB_RADIO_SENSING_STATUS, POST_GNB_RADIO_SENSING_TP, &gnb_radio_sensing_setup_ack},
    {GNB_VIDEO_SENSING_STATUS, POST_GNB_VIDEO_SENSING_TP, &gnb_video_sensing_setup_ack},
    {GNB_X_APP_STATUS, POST_GNB_X_APP_TP, &gnb_x_app_setup_ack},

    {UE_PLACEMENT_STATUS, POST_UE_PLACEMENT_TP, &ue_placement_setup_ack},
    {UE_RADIO_COMMUNICATIONS_STATUS, POST_UE_RADIO_COMM_TP, &ue_radio_comm_setup_ack},
    {UE_RADIO_SENSING_STATUS, POST_UE_RADIO_SENSING_TP, &ue_radio_sensing_setup_ack},
    {UE_VIDEO_SENSING_STATUS, POST_UE_VIDEO_SENSING_TP, &ue_video_sensing_setup_ack},

    {LIS_PLACEMENT_STATUS, POST_LIS_PLACEMENT_TP, &lis_placement_setup_ack},
    {LIS_RADIO_COMMUNICATIONS_STATUS, POST_LIS_RADIO_COMM_TP, &lis_radio_comm_setup_ack},
    {LIS_RADIO_SENSING_STATUS, POST_LIS_RADIO_SENSING_TP, &lis_radio_sensing_setup_ack},
    {LIS_VIDEO_SENSING_STATUS, POST_LIS_VIDEO_SENSING_TP, &lis_video_sensing_setup_ack},
};

int configuration_received(const char* url, const char* body, char* status) {
    size_t numPost = sizeof(post_requests) / sizeof(post_requests[0]);

    for (size_t i = 0; i < numPost; ++i)
    {
        if (strcmp(url, post_requests[i].request) == 0)
        {
            mosquitto_publish(mosq, NULL, post_requests[i].topic, strlen(body), body, 1, false);

            int attempts = 10;
            
            while (*post_requests[i].status == NULL && attempts > 0) {
                usleep(100 * 1000);
                attempts--;
            }
        
            int st = 0;
    
            if (*post_requests[i].status == NULL) {
                return 500;
            }
            else {
                strcpy(status, *post_requests[i].status);
                *post_requests[i].status = NULL;
                return 200;
            }
        }
    }
    return -1;
}

int get_status_received(const char* url, char* status) {
    size_t numGet = sizeof(get_requests) / sizeof(get_requests[0]);

    for (size_t i = 0; i < numGet; ++i)
    {
        if (strcmp(url, get_requests[i].request) == 0)
        {
            mosquitto_publish(mosq, NULL, get_requests[i].topic, strlen(url), url, 1, false);

            int attempts = 10;
            
            while (*get_requests[i].status == NULL && attempts > 0) {
                usleep(100 * 1000);
                attempts--;
            }

            int st = 0;
    
            if (*get_requests[i].status == NULL) {
                return 500;
            }
            else {
                strcpy(status, *get_requests[i].status);
                *get_requests[i].status = NULL;
                return 200;
            }
        }
    }

    return -1;
}


void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
