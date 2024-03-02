#include <stdio.h>
#include <mosquitto.h>
#include <unistd.h>
#include <string.h>
#include "headers.h"
int rc;
struct mosquitto *mosq;

int config()
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);

    rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60);
    if (rc != 0)
    {
        printf("Client could not connect to broker! Error Code: %d\n", rc);
        mosquitto_destroy(mosq);
        return 1;
    }
    printf("We are now connected to the broker!\n");
    return 0;
}

int run()
{
    while (1)
    {

        const char *message = "sending message from gnb_radio_sensing";
        mosquitto_publish(mosq, NULL, GNB_RS, strlen(message), message, 0, false);

        const char *message1 = "sending message from ue_radio_sensing";
        mosquitto_publish(mosq, NULL, UE_RS, strlen(message), message1, 0, false);

        const char *message2 = "sending message from lis_radio_sensing";
        mosquitto_publish(mosq, NULL, LIS_RS, strlen(message), message2, 0, false);

        const char *message3 = "sending message from video_sensing";
        mosquitto_publish(mosq, NULL, VIDEO_S, strlen(message), message3, 0, false);

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