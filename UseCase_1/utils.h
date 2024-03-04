#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "cJSON.h"

void create_json(char **json_data, char *current_time, char *topic, char *publisher, char *payload);
char *get_current_time();
void parse_json(char* json_data, char** timestamp, char** topic, char** publisher, char** payload);
void get_payload_from_json(char* json_data, char** payload);

#endif /* _UTILS_H_ */