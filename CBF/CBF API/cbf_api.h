#ifndef CBF_API_H_
#define CBF_API_H_

// Gnb placement (x, y, z)
void get_gnb_placement_status(char *placement);

// Gnb placement (x, y, z)
void set_gnb_placement_setup(const char* placement);

// O_CU, O_DU, O_RU, (bitrate e CQI)
void get_gnb_radio_communications_status(char *status);

// O_CU, O_DU, O_RU, (bitrate e CQI)
void set_gnb_radio_communications_setup(const char *setup);

// Gnb radio sensing, RSSI, RSRP, RSRQ, RI
void get_gnb_radio_sensing_status(char *status);

// Gnb radio sensing, RSSI, RSRP, RSRQ, RI
void set_gnb_radio_sensing_setup(const char *setup);

void get_gnb_video_sensing_status(char *status);

void set_gnb_video_sensing_setup(const char *setup);

void get_gnb_x_app_status(char *status);

void set_gnb_x_app_setup(const char *setup);

void get_ue_placement_status(char *status);

void set_ue_placement_setup(const char *setup);

void get_ue_radio_communications_status(char *status);

void set_ue_radio_communications_setup(const char *setup);

void get_ue_radio_sensing_status(char *status);

void set_ue_radio_sensing_setup(const char *setup);

void get_ue_video_sensing_status(char *status);

void set_ue_video_sensing_setup(const char *setup);

void get_lis_placement_status(char *status);

void set_lis_placement_setup(const char *setup);

void get_lis_radio_communication_status(char *status);

void set_lis_radio_communication_setup(const char *setup);

void get_lis_radio_sensing_status(char *status);

void set_lis_radio_sensing_setup(const char *setup);

void get_lis_video_sensing_status(char *status);

void set_lis_video_sensing_setup(const char *setup);

void get_ctcf_status(char *status);

void set_ctcf_setup(const char* setup);

void get_3d_s_status(char *status);

void set_3d_s_setup(const char* setup);

void get_vr_s_status(char *status);

void set_vr_s_setup(const char* setup);

void get_net_s_status(char *status);

void set_net_s_setup(const char* setup);

void upload_ml_model(const char* model);

void download_ml_model(char *model);

void upload_odr_dataset(const char *dataset);

void download_odr_dataset(char *dataset);

void get_experiment_status(char *status);

#endif