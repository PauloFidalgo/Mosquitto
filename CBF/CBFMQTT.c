#include "headers.h"
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"
#include "CBF.h"
#include "CBFMQTT.h"

extern int state;

uint8_t modules_ready = 0x00;
uint8_t modules_error = 0x00;
uint8_t modules_ack = 0x00;
bool db_fnsh_ack = false;
bool finished = false;
bool started = false;
struct mosquitto *mosq = NULL;

struct ReplyFlag
{
    const char *payload;
    unsigned int flag;
};

struct TopicActions
{
    const char *topic;
    void (*action)(const struct mosquitto_message *);
};

static const struct ReplyFlag replySuccessFlag[] = {
    {GNB_SETUP_READY, 0x01},
    {UE_SETUP_READY, 0x02},
    {LIS_SETUP_READY, 0x04},
    {VC_SETUP_READY, 0x08},
    {CODRF_SETUP_READY, 0x10},
};

static const struct ReplyFlag replyErrorFlag[] = {
    {GNB_SETUP_ERROR, 0x01},
    {UE_SETUP_ERROR, 0x02},
    {LIS_SETUP_ERROR, 0x04},
    {VC_SETUP_ERROR, 0x08},
    {CODRF_SETUP_ERROR, 0x10},
};

static const struct ReplyFlag replyFinishSuccessFlag[] = {
    {GNB_FINISH_SUCCESS, 0x01},
    {UE_FINISH_SUCCESS, 0x02},
    {LIS_FINISH_SUCCESS, 0x04},
    {VC_FINISH_SUCCESS, 0x08},
    {CODRF_FINISH_SUCCESS, 0x10},
};

static const struct ReplyFlag replyResetSuccessFlag[] = {
    {GNB_RESET_SUCCESS, 0x01},
    {UE_RESET_SUCCESS, 0x02},
    {LIS_RESET_SUCCESS, 0x04},
    {VC_RESET_SUCCESS, 0x08},
    {CODRF_RESET_SUCCESS, 0x10},
};

void initial_connection(struct mosquitto *mosq, void *userdata, int rc)
{
    if (rc == 0)
    {
        printf("Connected to MQTT broker from CBF \n");
        mosquitto_subscribe(mosq, NULL, CAF_SCHEDULE, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_schedule_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    printf("Received schedule\n");
    mosquitto_unsubscribe(mosq, NULL, CAF_SCHEDULE);
    started = true;
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
    mosquitto_message_callback_set(mosq, on_schedule_received);

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
        ;
}

void setup_ack_success_handler(const struct mosquitto_message *message)
{
    const char *payload = (const char *)message->payload;
    size_t numFlags = sizeof(replySuccessFlag) / sizeof(replySuccessFlag[0]);

    for (size_t i = 0; i < numFlags; ++i)
    {
        if (strcmp(payload, replySuccessFlag[i].payload) == 0)
        {
            modules_ready |= replySuccessFlag[i].flag;
        }
    }
}

// pode dar tenda por causa do tamanho dentro do for ( do numErrors)
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

void setup_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, COMMAND) == 0)
    {
        setup_ack_success_handler(message);
    }
    else if (strcmp(message->topic, ERRORS) == 0)
    {
        setup_ack_error_handler(message);
    }
}

int wait_setup_acknowledge_from_all_modules()
{
    while (modules_error == 0 && modules_ack != 0x17)
        ;

    if (modules_error == 0)
        return 0;

    modules_error = 0;

    return 1;
}

void send_configuration_to_all_modules()
{
    char *message = "CONFIGURATION";
    mosquitto_message_callback_set(mosq, setup_ack_handler);

    mosquitto_publish(mosq, NULL, CGNBCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CLISCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CVCF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CORDF_SETUP, strlen(message), message, 1, true);
    mosquitto_publish(mosq, NULL, CUECF_SETUP, strlen(message), message, 1, true);
}

void send_start_command()
{
    char *message = START_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void send_finish_command()
{
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void modules_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
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

void reset_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
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

int wait_finish_acknowledge()
{
    mosquitto_message_callback_set(mosq, modules_ack_handler);

    while (modules_error == 0 && modules_ack != 0x07)
        ;

    if (modules_error == 0)
        return 0;

    return 1;
}

void send_finish_command_to_db()
{
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
}

void finish_db_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    db_fnsh_ack = true;
}

void wait_db_finish_acknowledge()
{
    mosquitto_message_callback_set(mosq, finish_db_ack_handler);
    while (!db_fnsh_ack)
        ;
}

void reset()
{
    modules_ack = 0x00,
    modules_error = 0x00;
    modules_ready = 0x00;
    db_fnsh_ack = false;
    finished = false;
    started = false;

    char *message = RESET;
    mosquitto_message_callback_set(mosq, modules_ack_handler);
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);

    while (modules_ack != 0x17)
        ;
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
