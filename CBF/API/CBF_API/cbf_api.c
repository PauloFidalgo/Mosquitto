#include "cbf_api.h"


int get_gnb_placement_status(char *status) {
    subscribe_gnb_status();
    int res = send_message_and_wait();
    int res = get_gnb_placement_status_mqtt(status, GET_GNB_PLACEMENT_ST);
    unsubscribe_gnb_status();

    return res;
}

int get_gnb_radio_communications_status(char *status) {
    subscribe_gnb_status();
    int res = get_gnb_radio_comm_status_mqtt(status, GET_GNB_RADIO_COMM_ST);
    unsubscribe_gnb_status();

    return res;
}

int get_gnb_radio_sensing_status(char *status) {
    subscribe_gnb_status();
    int res = get_gnb_radio_sensing_status_mqtt(status, GET_GNB_RADIO_SENSING_ST);
    unsubscribe_gnb_status();

    return res;
}

int get_gnb_video_sensing_status(char *status) {
    subscribe_gnb_status();
    int res = get_gnb_video_sensing_status_mqtt(status, GET_GNB_VIDEO_SENSING_ST);
    unsubscribe_gnb_status();

    return res;
}

int get_gnb_x_app_status(char *status) {
    subscribe_gnb_status();
    int res = get_gnb_x_app_status_mqtt(status, GET_GNB_X_APP_ST);
    unsubscribe_gnb_status();

    return res;
}

int get_ue_placement_status(char *status) {
    subscribe_ue_status();
    int res = get_ue_placement_status_mqtt(status, GET_UE_PLACEMENT_ST);
    unsubscribe_ue_status();

    return res;
}

int get_ue_radio_communications_status(char *status) {
    subscribe_ue_status();
    int res = get_ue_radio_comm_status_mqtt(status, GET_UE_RADIO_COMM_ST);
    unsubscribe_ue_status();

    return res;
}

int get_ue_radio_sensing_status(char *status) {
    subscribe_ue_status();
    int res = get_ue_radio_sensing_status_mqtt(status, GET_UE_RADIO_SENSING_ST);
    unsubscribe_ue_status();

    return res;
}

int get_ue_video_sensing_status(char *status) {
    subscribe_ue_status();
    int res = get_ue_video_sensing_status_mqtt(status, GET_UE_VIDEO_SENSING_ST);
    unsubscribe_ue_status();

    return res;
}

int get_lis_placement_status(char *status) {
    subscribe_lis_status();
    int res = get_lis_placement_status_mqtt(status, GET_LIS_PLACEMENT_ST);
    unsubscribe_lis_status();

    return res;
}

int get_lis_radio_communications_status(char *status) {
    subscribe_lis_status();
    int res = get_lis_radio_comm_status_mqtt(status, GET_LIS_RADIO_COMM_ST);
    unsubscribe_lis_status();

    return res;
}

int get_lis_radio_sensing_status(char *status) {
    subscribe_lis_status();
    int res = get_lis_radio_sensing_status_mqtt(status, GET_LIS_RADIO_SENSING_ST);
    unsubscribe_lis_status();

    return res;
}

int get_lis_video_sensing_status(char *status) {
    subscribe_lis_status();
    int res = get_lis_video_sensing_status_mqtt(status, GET_LIS_VIDEO_SENSING_ST);
    unsubscribe_lis_status();

    return res;
}

/* --------------------- SETTERS --------------------- */

int set_gnb_placement_setup(const char *setup) {
    subscribe_gnb_setup();
    int res = set_gnb_placement_setup_mqtt(setup);
    unsubscribe_gnb_setup();

    return res;
}

int set_gnb_radio_communications_setup(const char *setup) {
    subscribe_gnb_setup();
    int res = set_gnb_radio_comm_setup_mqtt(setup);
    unsubscribe_gnb_setup();

    return res;
}

int set_gnb_radio_sensing_setup(const char *setup) {
    subscribe_gnb_setup();
    int res = set_gnb_radio_sensing_setup_mqtt(setup);
    unsubscribe_gnb_setup();

    return res;
}

int set_gnb_video_sensing_setup(const char* setup) {
    subscribe_gnb_setup();
    int res = set_gnb_video_sensing_setup_mqtt(setup);
    unsubscribe_gnb_setup();

    return res;
}

int set_gnb_x_app_setup(const char* setup) {
    subscribe_gnb_setup();
    int res = set_gnb_x_app_setup_mqtt(setup);
    unsubscribe_gnb_setup();

    return res;
}

int set_ue_placement_setup(const char* setup) {
    subscribe_ue_setup();
    int res = set_ue_placement_setup_mqtt(setup);
    unsubscribe_ue_setup();

    return res;
}

int set_ue_radio_communications_setup(const char* setup) {
    subscribe_ue_setup();
    int res = set_ue_radio_comm_setup_mqtt(setup);
    unsubscribe_ue_setup();

    return res;
}

int set_ue_radio_sensing_setup(const char* setup) {
    subscribe_ue_setup();
    int res = set_ue_radio_sensing_setup_mqtt(setup);
    unsubscribe_ue_setup();

    return res;
}

int set_ue_video_sensing_setup(const char* setup) {
    subscribe_ue_setup();
    int res = set_ue_video_sensing_setup_mqtt(setup);
    unsubscribe_ue_setup();

    return res;
}

int set_lis_placement_setup(const char* setup) {
    subscribe_lis_setup();
    int res = set_lis_placement_setup_mqtt(setup);
    unsubscribe_lis_setup();

    return res;
}

int set_lis_radio_communications_setup(const char* setup) {
    subscribe_lis_setup();
    int res = set_lis_radio_comm_setup_mqtt(setup);
    unsubscribe_lis_setup();

    return res;
}

int set_lis_radio_sensing_setup(const char* setup) {
    subscribe_lis_setup();
    int res = set_lis_radio_sensing_setup_mqtt(setup);
    unsubscribe_lis_setup();

    return res;
}

int set_lis_video_sensing_setup(const char* setup) {
    subscribe_lis_setup();
    int res = set_lis_video_sensing_setup_mqtt(setup);
    unsubscribe_lis_setup();

    return res;
}