#ifndef CGNBCF_ATTRIBUTES_H_
#define CGNBCF_ATTRIBUTES_H_

typedef struct {
    int x;
    int y;
    int z;
} cgnb_placement_t;

typedef struct {
    int peridiocity;
    float iq_samples;
} cgnb_radio_sensing_t;

typedef struct {
    int remote_port;
    int local_port;
    char *local_ip;
    char *remote_ip;
} ru_t ;

typedef struct {
    int local_port;
    int remote_port;
    char *local_ip;
    char *remote_ip;
} f1_t; 

typedef struct {
    char* cn;
    f1_t f1;
} o_cu_t;

typedef struct {
    f1_t f1;
    char* t_7_2;
    ru_t ru;
} o_du_t;

typedef struct {
   char* t_7_2;
   ru_t ru;
} o_ru_t;

typedef struct {
     o_cu_t o_cu;
     o_du_t o_du;
     o_ru_t o_ru;
     cgnb_placement_t cgnb_placement;
     cgnb_radio_sensing_t cgnb_radio_sensing;
} cgnb_config_t;

typedef struct {
    int rssi;
    int rsrp;
    int rsrq;
    int phr;
    int sinr;
    int cqi;
    int mcs;
    float bler;
    float bitrate;
    int ri;
} cgnb_telemetry_t;

#endif