#ifndef _CBFMQTT_H_
#define _CBFMQTT_H_

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

int initial_config();

void initial_connection(struct mosquitto *mosq, void *userdata, int rc);

void send_configuration_to_modules();

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

void on_schedule_received(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);

void send_reconfig(const struct mosquitto_message *message);

void handle_command_reply(const struct mosquitto_message *message);

void config();

void send_finish_command();

void send_experience_finished_command();

void send_start_command();

void timer_handler();

void idle();

int run();

void destroy();

#endif