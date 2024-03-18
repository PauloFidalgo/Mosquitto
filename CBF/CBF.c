#include "headers.h"
#include <mosquitto.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

uint8_t modules_ready = 0x00;
uint8_t modules_ack = 0x00;
bool finished = false;
bool started = false;
struct mosquitto *mosq = NULL;

void initial_connection(struct mosquitto *mosq, void *userdata, int rc) {
    if (rc == 0) {
        printf("Connected to MQTT broker from CBF \n");
        mosquitto_subscribe(mosq, NULL, CAF_ERR, 1);
    }
    else
    {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void send_reconfig(const struct mosquitto_message *message)
{   
    char* json = NULL;
    char *payload = NULL;
    get_payload_from_json((char *)message->payload, &payload);
    create_json(&json, get_current_time(), CBF, payload);

    mosquitto_publish(mosq, NULL, BEAM_CONF, strlen(json), json, 1, true);
    mosquitto_publish(mosq, NULL, LIS_CONF, strlen(json), json, 1, true);

    cJSON_free(json);
    free(payload);
    
    /*
    char *payload;
    get_payload_from_json((char *)message->payload, &payload);

    mosquitto_publish(mosq, NULL, BEAM_CONF, strlen(payload), payload, 1, true);
    mosquitto_publish(mosq, NULL, LIS_CONF, strlen(payload), payload, 1, true);

    free(payload);
    */
}

void gNB_placement_setup(){

}
void gNB_radio_Communications_setup(){

}
void gNB_radio_sensing_setup(){

}
void gNB_radio_sensing_setup(){

}
void gNB_X_APP_setup(){
    
}
void ue_placement_setup(){

}
void ue_radio_Communications_setup(){

}
void ue_radio_sensing_setup(){

}
void ue_video_sensing_setup(){

}
void lis_placement_setup(){

}
void lis_radio_communication_setup(){

}
void lis_radio_sensing_setup(){

}
void lis_video_sensing_setup(){

}
void ctcf_setup(){

}
void d_s(){

}
void vr_s_setup(){

}
void net_s_setup(){

}
void ml_model_upload(){

}
void odr_dataset_upload(){

}

void handle_command_reply(const struct mosquitto_message *message)
{
    if (strcmp((char *)message->payload, GNB_READY) == 0)
    {
        modules_ready |= 0x01;
    }
    else if (strcmp((char *)message->payload, UE_READY) == 0)
    {
        modules_ready |= 0x02;
    }
    else if (strcmp((char *)message->payload, LIS_READY) == 0)
    {
        modules_ready |= 0x04;
    }
    else if (strcmp((char *)message->payload, VC_READY) == 0)
    {
        modules_ready |= 0x08;
    }
    else if (strcmp((char *)message->payload, CODRF_READY) == 0)
    {
        modules_ready |= 0x10;
    }
    else if (strcmp((char *)message->payload, CAF_READY) == 0)
    {
        modules_ready |= 0x20;
    }
    else if (strcmp((char *)message->payload, GNB_ACK) == 0)
    {
        modules_ack |= 0x01;
    }
    else if (strcmp((char *)message->payload, UE_ACK) == 0)
    {
        modules_ack |= 0x02;
    }
    else if (strcmp((char *)message->payload, LIS_ACK) == 0)
    {
        modules_ack |= 0x04;
    }
    else if (strcmp((char *)message->payload, VC_ACK) == 0)
    {
        modules_ack |= 0x08;
    }
    else if (strcmp((char *)message->payload, CODRF_ACK) == 0)
    {
        modules_ack |= 0x10;
    }
    else if (strcmp((char *)message->payload, CAF_ACK) == 0)
    {
        modules_ack |= 0x20;
    }

    // Add more else if conditions for other payloads if needed
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (strcmp(message->topic, RECONF) == 0)
    {
        send_reconfig(message);
    }
    else if (strcmp(message->topic, DATA) == 0)
    {
    }
    else if (strcmp(message->topic, COMMAND) == 0)
    {
        handle_command_reply(message);
    }
    // Add more else if conditions for other topics if needed
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

    mosquitto_connect_callback_set(mosq, initial_connection);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Unable to connect to MQTT broker .\n");
        return 1;
    }

    mosquitto_loop_start(mosq);

    return 0;
}

void config()
{
    mosquitto_subscribe(mosq, NULL, DATA, 1);
    mosquitto_subscribe(mosq, NULL, RECONF, 1);
    printf("Subscribed topics DATA and RECONF\n");
}

void send_finish_command()
{
    char *message = FINISH_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
    printf("Published finish Command\n");
}
void send_experience_finished_command()
{
    char *message = EXPERIENCE_FINISHED;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
    printf("Published experience finished Command\n");
}
void send_start_command()
{
    mosquitto_subscribe(mosq, NULL, COMMAND, 1);

    char *message = START_COMMAND;
    mosquitto_publish(mosq, NULL, COMMAND, strlen(message), message, 1, true);
    printf("Published start Command\n");
}

void timer_handler()
{
    printf("TimeOut\n");
    send_finish_command();

    while (modules_ack != 0x2F);

    printf("All modules finhished with the exception of CODRF\n");

    send_experience_finished_command();

    while ((modules_ack & 0x10) != 0);

    printf("Experience terminated shutting down\n");
    finished = true;
}
void idle()
{
    char input[100];

    printf("Enter when you want to start\n");
    while (!started)
    {
        printf("Please enter something:\n");
        scanf("%s", &input);
        if (strcmp(input, "start") == 0)
        {

            started = true;
        }
    }

    send_start_command();

    while (modules_ready != 0x3F);


    printf("All modules ready\n");
}

int run()
{
    signal(SIGALRM, timer_handler);
    alarm(EXPERIENCE_TIME_S);

    while (!finished);

    return 0;
}

cJSON gNB_placement_status() {

}

cJSON gNB_radiocommunicatio_ns_status() {
    
}

cJSON gNB_radiosensing_status() {
    
}

cJSON gNB_videosensing_status() {

}

cJSON gNB_xapp_status() {

}

cJSON ue_placement_status() {

}

char* ue_radiocommunication_s_status() {
    char* ibrilhos;
    create_json(&ibrilhos, "conassa", "persons", "philips");
    return ibrilhos;
}

cJSON ue_radiosensing_status() {

}

cJSON ue_videosensing_status() {

}

cJSON lis_placement_status() {

}

cJSON lis_radiocommunication_s_status() {

}

cJSON lis_radiosensing_status() {

}

cJSON lis_videosensing_status() {

}

cJSON ctcf_status() {

}

cJSON ds3_status() {

}

cJSON vrs_status() {

}

cJSON nets_status() {

}

cJSON ml_download() {

}

cJSON odr_download() {

}

cJSON cbf_status() {

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

    idle();

    config();

    run();

    destroy();

    return 0;
}
