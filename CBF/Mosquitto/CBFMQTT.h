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
#include "../Modules/CBF.h"


void initial_connection(struct mosquitto *mosq, void *userdata, int rc);
int initial_config();
void on_schedule_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void reset();
void destroy();

/*
 * Waiters
 */
void wait_for_schedule();
int wait_setup_acknowledge_from_all_modules(uint8_t *modules);
int wait_finish_acknowledge();
void wait_db_finish_acknowledge();

/*
 * Senders 
 */
void send_configuration_to_all_modules(const struct configuration_t *configuration);
void send_start_command();
void send_finish_command();
void send_finish_command_to_db();

/*
 * Message handlers
 */
void setup_ack_success_handler(const struct mosquitto_message *message);
void setup_ack_error_handler(const struct mosquitto_message *message);
void setup_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void reset_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void finish_db_ack_handler(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

/*
 * Getters from GNB API
 */
void get_gnb_placement_status_mqtt(char* status, char* message);

void subscribe_gnb_status();
void unsubscribe_gnb_status();

#endif /* _CBFMQTT_H_ */
