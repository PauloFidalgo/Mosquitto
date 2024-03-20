#ifndef _CBF_H_
#define _CBF_H_

#include "headers.h"
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "CBFMQTT.c"

int state = STATE_IDLE;


void delay(int milliseconds);

/*
 * Handlers
 */
void experience_timer_handler();
void timer_handler();

/*
 * States
 */
void state_idle();
void state_scheduled();
void state_on_time();
void state_send_config_to_modules();
void state_send_str_cmd();
void state_running();
void state_wait_fnsh_ack();
void state_send_fnsh_db_cmd();
void state_wait_fnsh_db_ack();
void state_reset();

#endif /* CBF_H_ */