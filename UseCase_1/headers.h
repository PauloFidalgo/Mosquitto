/*
* File that defines headers for use case 1
* Change IP and Port according to your needs
*/
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883


/*
* Topics to be used in Use Case 1
* 
*/
#define COMMAND "command"
#define RECONF "reconfiguration"
#define BEAM_CONF "beam_config"
#define LIS_CONF "lis_config"
#define GNB_RS "gNB_radio_sensing"
#define UE_RS "ue_radio_sensing"
#define LIS_RS "lis_radio_sensing"
#define VIDEO_S "video_sensing"
#define DATA "data"
#define CAF_ERR "caf_error"


#define START_COMMAND "START"
#define FINISH_COMMAND "FINISH"
#define EXPERIENCE_FINISHED "EXP_FINISHED"


#define GNB_READY "gnb_ok"
#define UE_READY "ue_ok"
#define LIS_READY "lis_ok"
#define VC_READY "vc_ok"
#define CODRF_READY "codrf_ok"
#define CAF_READY "caf_ok"

#define GNB_ACK "gnb_ack"
#define UE_ACK "ue_ack"
#define LIS_ACK "lis_ack"
#define VC_ACK "vc_ack"
#define CODRF_ACK "codrf_ack"
#define CAF_ACK "caf_ack"

#define CAF "caf"
#define CBF "cbf"
#define CgNBCF "cgnbcf"
#define CLISCF "cliscf"
#define CVCF "cvcf"
#define CUECF "cuecf"
#define CODRF "codrf"
