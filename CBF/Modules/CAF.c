#include "../Utils/headers.h"
#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "requests.h"

struct mosquitto *mosq = NULL;
bool start = false;
bool end = false;

void delay(int milliseconds){
    usleep(milliseconds * 1000);
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

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);
    return 0;
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    if (initial_config()) return 1;

    delay(5000);
    
    char* message = "ibrilhos";

    mosquitto_publish(mosq, NULL, CAF_SCHEDULE, strlen(message), message, 1, true);
    
    printf("published message \n");
    destroy();

    return 0;
}
