#include "CBF.h"
#include "CBFMQTT.h"


struct CbfStateMachine {
    u_int32_t state;
    void (*transition_func)();
};

void experience_timer_handler() {
    state = STATE_RUNNING_TIME_EXP;
}

void timer_handler() {
    state = STATE_ON_TIME;
}

void delay(int milliseconds){
    usleep(milliseconds * 1000);
}

void state_idle(){
    printf("idle, waiting for schedule\n");

    wait_for_schedule();

    state = STATE_SCHEDULED;
}

void state_scheduled(){
    printf("I am Scheduled!\n");
    signal(SIGALRM, timer_handler);
    alarm(EXPERIENCE_TIME_S);   
}

void state_on_time(){  
    printf("Getting configuration from database\n");
    if (1) {
        // Success
        state = STATE_SEND_CONFIG_TO_MODULES;
    }
    else {
        // Error
        state = STATE_IDLE;
    }
}

void state_send_config_to_modules() {
    printf("Got the configurations, sending them to modules\n");
    send_configuration_to_all_modules();
    if (wait_setup_acknowledge_from_all_modules()) {
        state = STATE_SEND_STR_CMD;
    }
    else {
        state = STATE_IDLE;
    }
}

void state_send_str_cmd() {
    printf("All modules acknowledged, sending start command!\n");
    send_start_command();
    state = STATE_RUNNING;
    signal(SIGALRM, experience_timer_handler);
    alarm(EXPERIENCE_TIME_S);   
}

void state_running() {}

void state_running_time_exp() {
    printf("Running time expired!\n");
    send_finish_command();
    state = STATE_WAIT_FNSH_ACK;
}

void state_wait_fnsh_ack() {
    printf("Waiting finish acknowledge\n");
    wait_finish_acknowledge();
    state = STATE_SEND_FNSH_DB_CMD;
}

void state_send_fnsh_db_cmd() {
    printf("Sending finish command\n");
    send_finish_command_to_db();
    state = STATE_WAIT_FNSH_DB_ACK;
}

void state_wait_fnsh_db_ack() {
    printf("Waiting finish command from db\n");
    wait_db_finish_acknowledge();
    state = STATE_RESET;
}

void state_reset() {
    printf("Sending reset command\n");
    reset();
    state = STATE_FINISHED;
}

static const struct CbfStateMachine cbfStateMachine [] = {
    {STATE_IDLE, state_idle},
    {STATE_SCHEDULED, state_scheduled},
    {STATE_ON_TIME, state_on_time},
    {STATE_SEND_CONFIG_TO_MODULES, state_send_config_to_modules},
    {STATE_SEND_STR_CMD, state_send_str_cmd},
    {STATE_RUNNING, state_running},
    {STATE_RUNNING_TIME_EXP, state_running_time_exp},
    {STATE_WAIT_FNSH_ACK, state_wait_fnsh_ack},
    {STATE_SEND_FNSH_DB_CMD, state_send_fnsh_db_cmd},
    {STATE_WAIT_FNSH_DB_ACK, state_wait_fnsh_db_ack},
    {STATE_RESET, state_reset},
};

int main(){
    if (initial_config()) {
        printf("Error Connecting to MQTT server\n");
    }

    while (state != STATE_FINISHED) {
        cbfStateMachine[state].transition_func(); 
    }

    return 0;
}
