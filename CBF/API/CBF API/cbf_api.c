#include "cbf_api.h"

void get_gnb_placement_status(char *status) {
    subscribe_gnb_status();
    get_gnb_placement_status_mqtt(status, GET_GNB_PLACEMENT_ST);
    unsubscribe_gnb_status();
}

void get_gnb_radio_communications_status(char *status) {
    subscribe_gnb_status();
    get_gnb_placement_status_mqtt(status, GET_GNB_RADIO_COMM_ST);
    unsubscribe_gnb_status();
}

void get_gnb_radio_sensing_status(char *status) {
    subscribe_gnb_status();
    get_gnb_placement_status_mqtt(status, GET_GNB_RADIO_SENSING_ST);
    unsubscribe_gnb_status();
}

void get_gnb_video_sensing_status(char *status) {
    subscribe_gnb_status();
    get_gnb_placement_status_mqtt(status, GET_GNB_VIDEO_SENSING_ST);
    unsubscribe_gnb_status();
}

void get_gnb_x_app_status(char *status) {
    subscribe_gnb_status();
    get_gnb_placement_status_mqtt(status, GET_GNB_X_APP_ST);
    unsubscribe_gnb_status();
}