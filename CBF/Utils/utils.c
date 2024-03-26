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

void create_json(char** json_data, char* current_time, char* publisher, char* payload)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "Timestamp", current_time);
    cJSON_AddStringToObject(json, "Publisher", publisher);
    cJSON_AddStringToObject(json, "Payload", payload);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_json(char* json_data, char** timestamp, char** publisher, char** payload){
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

void create_gnb_placement_json(char** json_data, char* x, char* y, char* z) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "x", x);
    cJSON_AddStringToObject(json, "y", y);
    cJSON_AddStringToObject(json, "z", z);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_radio_sensing_json(char** json_data, char* periodicity, char* iq_samples) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "periodicity", periodicity);
    cJSON_AddStringToObject(json, "iq_samples", iq_samples);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_band_configuration_json(char** json_data, char* band, char* ssbArfcn, char* pointAArfcn) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "band", band);
    cJSON_AddStringToObject(json, "ssbArfcn", ssbArfcn);
    cJSON_AddStringToObject(json, "pointAArfcn", pointAArfcn);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_ru_json(char** json_data, char* band_configuration, char* band_width_mhz, char* tdd_configuration, char* logical_ant_rx, char* logical_ant_tx) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "band_configuration", band_configuration);
    cJSON_AddStringToObject(json, "band_width_mhz", band_width_mhz);
    cJSON_AddStringToObject(json, "tdd_configuration", tdd_configuration);
    cJSON_AddStringToObject(json, "logical_ant_rx", logical_ant_rx);
    cJSON_AddStringToObject(json, "logical_ant_tx", logical_ant_tx);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_f1_json(char** json_data, char* remote_port, char* local_port, char* local_ip, char* remote_ip) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "remote_port", remote_port);
    cJSON_AddStringToObject(json, "local_port", local_port);
    cJSON_AddStringToObject(json, "local_ip", local_ip);
    cJSON_AddStringToObject(json, "remote_ip", remote_ip);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_o_cu_json(char** json_data, char* cn, char* f1) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "cn", cn);
    cJSON_AddStringToObject(json, "f1", f1);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_o_du_json(char** json_data, char* f1, char* ru, char* t_7_2) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "f1", f1);
    cJSON_AddStringToObject(json, "ru", ru);
    cJSON_AddStringToObject(json, "7_2", t_7_2);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_o_ru_json(char** json_data, char* t_7_2, char* ru) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "7_2", t_7_2);
    cJSON_AddStringToObject(json, "ru", ru);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_config_json(char** json_data, char* o_cu, char* o_du, char* o_ru, char* gnb_placement, char* gnb_radio_sensing) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "o_cu", o_cu);
    cJSON_AddStringToObject(json, "o_du", o_du);
    cJSON_AddStringToObject(json, "o_ru", o_ru);
    cJSON_AddStringToObject(json, "gnb_placement", gnb_placement);
    cJSON_AddStringToObject(json, "gnb_radio_sensing", gnb_radio_sensing);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void create_gnb_telemetry_json(char** json_data, char* rssi, char* rsrp, char* rsrq, char* phr, char* sinr, char* cqi, char* mcs, char* bler, char* bitrate, char* ri) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "rssi", rssi);
    cJSON_AddStringToObject(json, "rsrp", rsrp);
    cJSON_AddStringToObject(json, "rsrq", rsrq);
    cJSON_AddStringToObject(json, "phr", phr);
    cJSON_AddStringToObject(json, "sinr", sinr);
    cJSON_AddStringToObject(json, "cqi", cqi);
    cJSON_AddStringToObject(json, "mcs", mcs);
    cJSON_AddStringToObject(json, "bler", bler);
    cJSON_AddStringToObject(json, "bitrate", bitrate);
    cJSON_AddStringToObject(json, "ri", ri);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}