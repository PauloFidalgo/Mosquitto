#include "CBF.h"


int state = STATE_IDLE;

typedef struct {
    u_int32_t state;
    void (*transition_func)();
} CbfStateMachine;

const struct configuration_t configuration = {};

void experiment_timer_handler() {
    state = STATE_RUNNING_TIME_EXP;
}

void timer_handler() {
    state = STATE_ON_TIME;
}

void delay(int milliseconds){
    usleep(milliseconds * 1000);
}

void state_idle(){
    printf("Idle, waiting for schedule\n");

    wait_for_schedule();

    signal(SIGALRM, timer_handler);
    alarm(EXPERIMENT_TIME_S); 
    state = STATE_SCHEDULED;
}

void state_scheduled(){
    printf("Schedule received, waiting for experiment time...\n");
    delay(1000);
}

// Ideia é dar algumas tentativas de ir buscar à base de dados i.e. 10, se falhar, passar para estado ERROR_GETTING_CONFIGURATION
// Dar parse da configuração para o struct configuration
void state_on_time(){  
    printf("Getting configuration from database\n");
    if (1) {
        // Success
        state = STATE_SEND_CONFIG_TO_MODULES;
    }
    else {
        // Error 
        //state = ERROR_GETTING_CONFIGURATION;
    }
}

void state_send_config_to_modules() {
    printf("Got the configurations, sending them to modules\n");
    send_configuration_to_all_modules(&configuration);
    uint8_t modules = 0x00;
    int res = wait_setup_acknowledge_from_all_modules(&modules);
    if (res == 0) {
        state = STATE_SEND_STR_CMD;
    }
    else if (res == 1) {
        // Timeout
        if (!(modules & 0x01)) {
            printf("TIMEOUT: CgNBCF not connected\n");    
        }
        if (!(modules & 0x02)) {
            printf("TIMEOUT: CUECF not connected\n");  
        }
        if (!(modules & 0x04)) {
            printf("TIMEOUT: CLISCF not connected\n");  
        }

        state = STATE_IDLE;
    }
    else {
        state = STATE_IDLE;
    }
}

void state_send_str_cmd() {
    printf("All modules acknowledged, sending start command!\n");
    send_start_command();
    state = STATE_RUNNING;
    signal(SIGALRM, experiment_timer_handler);
    alarm(EXPERIMENT_TIME_S);   
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

static const CbfStateMachine cbfStateMachine [] = {
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
    while (initial_config()) {
        printf("Error Connecting to MQTT server\n");
        delay(5000);
    }

    while (state != STATE_FINISHED) {
        cbfStateMachine[state].transition_func(); 
    }

    return 0;
}
