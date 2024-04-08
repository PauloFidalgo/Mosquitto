#ifndef _CBFMQTT_H_
#define _CBFMQTT_H_

#include "../Utils/headers.h"
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../Utils/utils.h"
#include "../Utils/topics.h"
#include "../Socket/socket.h"

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

typedef struct {
    const char* message;
    void (*request_handler)(const struct mosquitto_message *);
} ApiRequestHandler_t;

typedef struct {
    char* gnb_config;
    char* ue_config;
    char* lis_config;
    char* cvcf_config;
} configuration_t;

typedef struct {
    const char* request;
    const char* topic;
    char** status;
} request_t;

void initial_connection(struct mosquitto *mosq, void *userdata, int rc);
int initial_config();
void on_schedule_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void reset();
void destroy();

/*
 * Wait for Ack
 */
void wait_for_schedule();
int wait_setup_acknowledge_from_all_modules(uint8_t *modules);
int wait_finish_acknowledge();
void wait_db_finish_acknowledge();

/*
 * Publisher 
 */
void send_configuration_to_all_modules(const configuration_t *configuration);
void send_start_command();
void send_finish_command();
void send_finish_command_to_db();

/*
 * Message handlers
 */
void setup_ack_success_handler(const struct mosquitto_message *message);
void finish_ack_success_handler(const struct mosquitto_message *message);
void reset_ack_success_handler(const struct mosquitto_message *message);
void setup_ack_error_handler(const struct mosquitto_message *message);
void finish_ack_error_handler(const struct mosquitto_message *message);
void reset_ack_error_handler(const struct mosquitto_message *message);
void command_handler(const struct mosquitto_message *message);
void handle_schedule_received(const struct mosquitto_message *message);
void errors_handler(const struct mosquitto_message *message);
void on_message_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void setup_timeout_handler();


void reset_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void finish_db_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);


/*
 * Getters from UE API
 */
int get_ue_placement_status_mqtt(char* status, char* message);
int get_ue_radio_comm_status_mqtt(char* status, char* message);
int get_ue_radio_sensing_status_mqtt(char* status, char* message);
int get_ue_video_sensing_status_mqtt(char* status, char* message);

int set_ue_placement_setup_mqtt(const char *setup);
int set_ue_radio_comm_setup_mqtt(const char *setup);
int set_ue_radio_sensing_setup_mqtt(const char *setup);
int set_ue_video_sensing_setup_mqtt(const char *setup);


/*
 * Getters from LIS API
 */
int get_lis_placement_status_mqtt(char* status, char* message);
int get_lis_radio_comm_status_mqtt(char* status, char* message);
int get_lis_radio_sensing_status_mqtt(char* status, char* message);
int get_lis_video_sensing_status_mqtt(char* status, char* message);

int set_lis_placement_setup_mqtt(const char *setup);
int set_lis_radio_comm_setup_mqtt(const char *setup);
int set_lis_radio_sensing_setup_mqtt(const char *setup);
int set_lis_video_sensing_setup_mqtt(const char* setup);

int get_status_received(const char* url, char* status);
int configuration_received(const char* url, const char* body, char* status);

#endif /* _CBFMQTT_H_ */
