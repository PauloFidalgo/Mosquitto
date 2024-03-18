#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "cJSON.h"

void create_json(char **json_data, char *current_time, char *publisher, char *payload);
char *get_current_time();
void parse_json(char* json_data, char** timestamp, char** publisher, char** payload);
void get_payload_from_json(char* json_data, char** payload);
void create_lis_setup_json(char* json_data, char** placement, char** radio_communication_setup, char** radio_communication_feedback, char** radio_sensing_setup, char** pre_configured_beam, char** custom_phase_profile, char** source_angle, char** destination_angle, char** beam_width, char** time_step, char** sensing_custom_phase_profile);
void create_ctcf_setup_json(char* json_data, char** rule_name, char** traffic_type, char** action, char** destination, char** priority, char** total_packets, char** packet_loss, char** latency, char** band_width_usage);

#endif /* _UTILS_H_ */