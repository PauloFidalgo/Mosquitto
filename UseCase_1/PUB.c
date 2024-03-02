#include <stdio.h>
#include <mosquitto.h>
#include <unistd.h>
#include <string.h>
#include "headers.h"

int main(){
	int rc;
	struct mosquitto * mosq;

	mosquitto_lib_init();

	mosq = mosquitto_new(RECONF, true, NULL);

	rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60);
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}
	printf("We are now connected to the broker!\n");
    while (1){
        
        const char *message = "sending message to lis_conf";
        mosquitto_publish(mosq, NULL, LIS_CONF, strlen(message), message, 0, false);
        // Sleep for a short time before publishing the next message
        usleep(1000000); // 1 second
    }
	

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}