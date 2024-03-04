#include "utils.h"

char *get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t current_time = tv.tv_sec;
    struct tm *timeinfo = localtime(&current_time);
    static char timestamp[25]; // Adjust the size based on your needs
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    char milliseconds[5];
    snprintf(milliseconds, sizeof(milliseconds), ".%03d", (int)(tv.tv_usec / 1000));
    strcat(timestamp, milliseconds);
    return timestamp;
}

void create_json(char** json_data, char* current_time, char* topic, char* publisher, char* payload)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "Timestamp", current_time);
    cJSON_AddStringToObject(json, "Publisher", publisher);
    cJSON_AddStringToObject(json, "Payload", payload);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_json(char* json_data, char** timestamp, char** topic, char** publisher, char** payload){
    cJSON* json = cJSON_Parse(json_data);

    cJSON* temp = cJSON_GetObjectItemCaseSensitive(json, "Timestamp");
    *timestamp = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "Publisher"); 
    *publisher = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "Payload"); 
    *payload = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void get_payload_from_json(char* json_data, char** payload){
    cJSON* json = cJSON_Parse(json_data);
    cJSON* temp = cJSON_GetObjectItemCaseSensitive(json, "Payload"); 
    *payload = strdup(temp->valuestring);
    cJSON_Delete(json);
}