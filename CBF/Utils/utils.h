#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "cJSON.h"

enum acknowledge {success, invalid_values, module_offline,unknown};
enum acknowledge acknowledge_from_string(const char* str);
const char* acknowledge_to_string(enum acknowledge ack);

char *get_current_time();
void create_json(char **json_data, char *current_time, char *publisher, char *payload);
void parse_json(char *json_data, char **timestamp, char **publisher, char **payload);
void get_payload_from_json(char *json_data, char **payload);
void create_gnb_placement_json(char **json_data, char *x, char *y, char *z);
void parse_gnb_placement_json(char *json_data, char **x, char **y, char **z);
void create_gnb_radio_sensing_json(char **json_data, char *periodicity, char *iq_samples);
void parse_gnb_radio_sensing_json(char *json_data, char **periodicity, char **iq_samples);
void create_gnb_band_configuration_json(char **json_data, char *band, char *ssbArfcn, char *pointAArfcn);
void parse_gnb_band_configuration_json(char *json_data, char **band, char **ssbArfcn, char **pointAArfcn);
void create_gnb_ru_json(char **json_data, char *band_configuration, char *band_width_mhz, char *tdd_configuration, char *logical_ant_rx, char *logical_ant_tx);
void parse_gnb_ru_json(char *json_data, char **band_configuration, char **band_width_mhz, char **tdd_configuration, char **logical_ant_rx, char **logical_ant_tx);
void create_gnb_f1_json(char **json_data, char *remote_port, char *local_port, char *local_ip, char *remote_ip);
void parse_gnb_f1_json(char *json_data, char **remote_port, char **local_port, char **local_ip, char **remote_ip);
void create_gnb_o_cu_json(char **json_data, char *cn, char *f1);
void parse_gnb_o_cu_json(char *json_data, char **cn, char **f1);
void create_gnb_o_du_json(char **json_data, char *f1, char *ru, char *t_7_2);
void parse_gnb_o_du_json(char *json_data, char **f1, char **ru, char **t_7_2);
void create_gnb_o_ru_json(char **json_data, char *t_7_2, char *ru);
void parse_gnb_o_ru_json(char *json_data, char **t_7_2, char **ru);
void create_gnb_config_json(char **json_data, char *o_cu, char *o_du, char *o_ru, char *gnb_placement, char *gnb_radio_sensing);
void parse_gnb_config_json(char *json_data, char **o_cu, char **o_du, char **o_ru, char **gnb_placement, char **gnb_radio_sensing);
void create_gnb_cn_json(char **json_data, char *data1, char *data2);
void parse_gnb_7_2_json(char *json_data, char **data1, char **data2);
void create_gnb_7_2_json(char **json_data, char *data1, char *data2);
void parse_gnb_7_2_json(char *json_data, char **data1, char **data2);
void create_gnb_telemetry_json(char **json_data, char *rssi, char *rsrp, char *rsrq, char *phr, char *sinr, char *cqi, char *mcs, char *bler, char *bitrate, char *ri);
void parse_gnb_telemetry_json(char* json_data, char** rssi, char** rsrp, char** rsrq, char** phr, char** sinr, char** cqi, char** mcs, char** bler, char** bitrate, char** ri);


#endif /* _UTILS_H_ */

