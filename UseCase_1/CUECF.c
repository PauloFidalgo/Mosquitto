#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include "headers.h"

struct mosquitto *mosq = NULL;



int config()
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

    if (mosquitto_threaded_set(mosq, true) != MOSQ_ERR_SUCCESS)
    {
        printf("Error: Unable to set threaded mode");
        return 1;
    }
    return 0;
}

int run()
{
    while (1)
    {
        // Publishing a message
        char *message = "Hello aqui, vai o datin do uezim!";
        mosquitto_publish(mosq, NULL, DATA, strlen(message), message, 1, true);
        
        *message = "Hello, aqui vai o ue_sensingzim";
        mosquitto_publish(mosq, NULL, UE_RS, strlen(message), message, 1, true);

        // Sleep for a short time before publishing the next message
        usleep(1000000); // 1 second
    }
}

void destroy()
{
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main()
{
    
    if (config())
    {
        return 1;
    }

    if (run())
    {
        return 1;
    }

    destroy();

    return 0;
}
