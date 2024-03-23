#ifndef CLISCF_ATTRIBUTES_H_
#define CLISCF_ATTRIBUTES_H_

typedef struct {
    float time_step;
    bool sensing_phase_profile[];
} radio_sensing_config_t;

typedef struct {
    char* pre_config_beam;
    bool custom_phase_profile[];
} radio_communications_config_t;

typedef struct {
   int source_angle[];
   int destination_angle[];
   int beam_width;
} pre_configured_beam_t;

#endif