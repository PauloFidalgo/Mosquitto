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

void create_json(char **json_data, char *current_time, char *publisher, char *payload)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "Timestamp", current_time);
    cJSON_AddStringToObject(json, "Publisher", publisher);
    cJSON_AddStringToObject(json, "Payload", payload);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_json(char *json_data, char **timestamp, char **publisher, char **payload)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "Timestamp");
    *timestamp = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "Publisher");
    *publisher = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "Payload");
    *payload = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void get_payload_from_json(char *json_data, char **payload)
{
    cJSON *json = cJSON_Parse(json_data);
    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "Payload");
    *payload = strdup(temp->valuestring);
    cJSON_Delete(json);
}

void create_gnb_placement_json(char **json_data, char *x, char *y, char *z)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "x", x);
    cJSON_AddStringToObject(json, "y", y);
    cJSON_AddStringToObject(json, "z", z);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_placement_json(char *json_data, char **x, char **y, char **z)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "x");
    *x = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "y");
    *y = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "z");
    *z = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_radio_sensing_json(char **json_data, char *periodicity, char *iq_samples)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "periodicity", periodicity);
    cJSON_AddStringToObject(json, "iq_samples", iq_samples);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_radio_sensing_json(char *json_data, char **periodicity, char **iq_samples)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "periodicity");
    *periodicity = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "iq_samples");
    *iq_samples = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_band_configuration_json(char **json_data, char *band, char *ssbArfcn, char *pointAArfcn)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "band", band);
    cJSON_AddStringToObject(json, "ssbArfcn", ssbArfcn);
    cJSON_AddStringToObject(json, "pointAArfcn", pointAArfcn);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_band_configuration_json(char *json_data, char **band, char **ssbArfcn, char **pointAArfcn)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "band");
    *band = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "ssbArfcn");
    *ssbArfcn = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "pointAArfcn");
    *pointAArfcn = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_ru_json(char **json_data, char *band_configuration, char *band_width_mhz, char *tdd_configuration, char *logical_ant_rx, char *logical_ant_tx)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "band_configuration", band_configuration);
    cJSON_AddStringToObject(json, "band_width_mhz", band_width_mhz);
    cJSON_AddStringToObject(json, "tdd_configuration", tdd_configuration);
    cJSON_AddStringToObject(json, "logical_ant_rx", logical_ant_rx);
    cJSON_AddStringToObject(json, "logical_ant_tx", logical_ant_tx);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_ru_json(char *json_data, char **band_configuration, char **band_width_mhz, char **tdd_configuration, char **logical_ant_rx, char **logical_ant_tx)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "band_configuration");
    *band_configuration = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "band_width_mhz");
    *band_width_mhz = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "tdd_configuration");
    *tdd_configuration = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "logical_ant_rx");
    *logical_ant_rx = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "logical_ant_tx");
    *logical_ant_tx = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_f1_json(char **json_data, char *remote_port, char *local_port, char *local_ip, char *remote_ip)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "remote_port", remote_port);
    cJSON_AddStringToObject(json, "local_port", local_port);
    cJSON_AddStringToObject(json, "local_ip", local_ip);
    cJSON_AddStringToObject(json, "remote_ip", remote_ip);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_f1_json(char *json_data, char **remote_port, char **local_port, char **local_ip, char **remote_ip)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "remote_port");
    *remote_port = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "local_port");
    *local_port = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "local_ip");
    *local_ip = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "remote_ip");
    *remote_ip = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_o_cu_json(char **json_data, char *cn, char *f1)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "cn", cn);
    cJSON_AddStringToObject(json, "f1", f1);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_o_cu_json(char *json_data, char **cn, char **f1)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "cn");
    *cn = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "f1");
    *f1 = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_o_du_json(char **json_data, char *f1, char *ru, char *t_7_2)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "f1", f1);
    cJSON_AddStringToObject(json, "ru", ru);
    cJSON_AddStringToObject(json, "7_2", t_7_2);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_o_du_json(char *json_data, char **f1, char **ru, char **t_7_2)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "f1");
    *f1 = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "ru");
    *ru = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "t_7_2");
    *t_7_2 = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_o_ru_json(char **json_data, char *t_7_2, char *ru)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "7_2", t_7_2);
    cJSON_AddStringToObject(json, "ru", ru);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_o_ru_json(char *json_data, char **t_7_2, char **ru)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "t_7_2");
    *t_7_2 = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "ru");
    *ru = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_config_json(char **json_data, char *o_cu, char *o_du, char *o_ru, char *gnb_placement, char *gnb_radio_sensing)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "o_cu", o_cu);
    cJSON_AddStringToObject(json, "o_du", o_du);
    cJSON_AddStringToObject(json, "o_ru", o_ru);
    cJSON_AddStringToObject(json, "gnb_placement", gnb_placement);
    cJSON_AddStringToObject(json, "gnb_radio_sensing", gnb_radio_sensing);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_config_json(char *json_data, char **o_cu, char **o_du, char **o_ru, char **gnb_placement, char **gnb_radio_sensing)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "o_cu");
    *o_cu = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "o_du");
    *o_du = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "o_ru");
    *o_ru = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "gnb_placement");
    *gnb_placement = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "gnb_radio_sensing");
    *gnb_radio_sensing = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_cn_json(char **json_data, char *data1, char *data2)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "c", data1);
    cJSON_AddStringToObject(json, "n", data2);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_cn_json(char *json_data, char **data1, char **data2)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "data1");
    *data1 = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "data2");
    *data2 = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_7_2_json(char **json_data, char *data1, char *data2)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "7", data1);
    cJSON_AddStringToObject(json, "2", data2);

    *json_data = cJSON_Print(json);
    cJSON_Delete(json);
}

void parse_gnb_7_2_json(char *json_data, char **data1, char **data2)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "data1");
    *data1 = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "data2");
    *data2 = strdup(temp->valuestring);

    cJSON_Delete(json);
}

void create_gnb_telemetry_json(char **json_data, char *rssi, char *rsrp, char *rsrq, char *phr, char *sinr, char *cqi, char *mcs, char *bler, char *bitrate, char *ri)
{
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

void parse_gnb_telemetry_json(char* json_data, char** rssi, char** rsrp, char** rsrq, char** phr, char** sinr, char** cqi, char** mcs, char** bler, char** bitrate, char** ri)
{
    cJSON *json = cJSON_Parse(json_data);

    cJSON *temp = cJSON_GetObjectItemCaseSensitive(json, "rssi");
    *rssi = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "rsrp");
    *rsrp = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "rsrq");
    *rsrq = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "phr");
    *phr = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "cqi");
    *cqi = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "mcs");
    *mcs = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "bler");
    *bler = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "bitrate");
    *bitrate = strdup(temp->valuestring);

    temp = cJSON_GetObjectItemCaseSensitive(json, "ri");
    *ri = strdup(temp->valuestring);

    cJSON_Delete(json);
}

const char* acknowledge_to_string(enum acknowledge ack) {
    switch (ack) {
        case success:
            return "success";
        case invalid_values:
            return "invalid_values";
        case module_offline:
            return "module_offline";
        default:
            return "unknown";
    }
}


enum acknowledge acknowledge_from_string(const char* str) {
    if (strcmp(str, "success") == 0) {
        return success;
    } else if (strcmp(str, "invalid_values") == 0) {
        return invalid_values;
    } else if (strcmp(str, "module_offline") == 0) {
        return module_offline;
    } else {
        return unknown;
    }
}