#ifndef CBF_API_H_
#define CBF_API_H_

#include "../Mosquitto/cbf_mqtt.h"
#include "../Utils/headers.h"

// Gnb placement (x, y, z)
int get_gnb_placement_status(char *status);

// Gnb placement (x, y, z)
int set_gnb_placement_setup(const char* setup);

// O_CU, O_DU, O_RU, (bitrate e CQI)
int get_gnb_radio_communications_status(char *status);

// O_CU, O_DU, O_RU, (bitrate e CQI)
int set_gnb_radio_communications_setup(const char *setup);

// Gnb radio sensing, RSSI, RSRP, RSRQ, RI
int get_gnb_radio_sensing_status(char *status);

// Gnb radio sensing, RSSI, RSRP, RSRQ, RI
int set_gnb_radio_sensing_setup(const char *setup);

int get_gnb_video_sensing_status(char *status);

int set_gnb_video_sensing_setup(const char *setup);

int get_gnb_x_app_status(char *status);

int set_gnb_x_app_setup(const char *setup);

int get_ue_placement_status(char *status);

int set_ue_placement_setup(const char *setup);

int get_ue_radio_communications_status(char *status);

int set_ue_radio_communications_setup(const char *setup);

int get_ue_radio_sensing_status(char *status);

int set_ue_radio_sensing_setup(const char *setup);

int get_ue_video_sensing_status(char *status);

int set_ue_video_sensing_setup(const char *setup);

int get_lis_placement_status(char *status);

int set_lis_placement_setup(const char *setup);

int get_lis_radio_communication_status(char *status);

int set_lis_radio_communication_setup(const char *setup);

int get_lis_radio_sensing_status(char *status);

int set_lis_radio_sensing_setup(const char *setup);

int get_lis_video_sensing_status(char *status);

int set_lis_video_sensing_setup(const char *setup);

int get_ctcf_status(char *status);

int set_ctcf_setup(const char* setup);

int get_3d_s_status(char *status);

int set_3d_s_setup(const char* setup);

int get_vr_s_status(char *status);

int set_vr_s_setup(const char* setup);

int get_net_s_status(char *status);

int set_net_s_setup(const char* setup);

int upload_ml_model(const char* model);

int download_ml_model(char *model);

int upload_odr_dataset(const char *dataset);

int download_odr_dataset(char *dataset);

int get_experiment_status(char *status);

#endif