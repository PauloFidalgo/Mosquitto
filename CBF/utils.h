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
void create_ctcf_setup_json(char* json_data, char** ruleName, char** trafficType, char** action, char** )

#endif /* _UTILS_H_ */