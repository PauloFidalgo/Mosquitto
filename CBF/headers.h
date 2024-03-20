/*
* File that defines headers for use case 1
* Change IP and Port according to your needs
*/
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883

#define DELAY_US 1000000
#define EXPERIENCE_TIME_S 15


/*  
 * Configuration Topics
 */
#define CGNBCF_SETUP "cgnb_setup" 
#define CLISCF_SETUP "lis_setup"
#define CVCF_SETUP "cvcf_setup"
#define CORDF_SETUP "cordf_setup"
#define CUECF_SETUP "cuecf_setup"
#define COMMAND "command"
#define ERRORS "errors"
#define RECONF "reconfiguration"
#define BEAM_CONF "beam_config"
#define LIS_CONF "lis_config"
#define GNB_RS "gNB_radio_sensing"
#define UE_RS "ue_radio_sensing"
#define LIS_RS "lis_radio_sensing"
#define VIDEO_S "video_sensing"
#define DATA "data"
#define CAF_ERR "caf_error"
#define CAF_SCHEDULE "caf_schedule"

/*
* Messages
*/

#define START_COMMAND "START"
#define FINISH_COMMAND "FINISH"
#define EXPERIENCE_FINISHED "EXP_FINISHED"
#define RESET "reset"
/*
 *  Success reply from COMMAND topic 
 *  Due to Setup Publish
 */
#define GNB_SETUP_READY "gnb_setup_ok"
#define UE_SETUP_READY "ue_setup_ok"
#define LIS_SETUP_READY "lis_setup_ok"
#define VC_SETUP_READY "vc_setup_ok"
#define CODRF_SETUP_READY "codrf_setup_ok"
#define CAF_SETUP_READY "caf_setup_ok"

/*
 *  Error reply from COMMAND topic
 *  Due to Setup Publish
 */
#define GNB_SETUP_ERROR "gnb_setup_error"
#define UE_SETUP_ERROR "ue_setup_error"
#define LIS_SETUP_ERROR "lis_setup_error"
#define VC_SETUP_ERROR "vc_setup_error"
#define CODRF_SETUP_ERROR "codrf_setup_error"
#define CAF__SETUP_ERROR "caf_setup_error"

/*
 *  Success reply from COMMAND topic
 *  Due to Finish Publish
 */
#define GNB_FINISH_SUCCESS "gnb_finish_success"
#define UE_FINISH_SUCCESS "ue_finish_success"
#define LIS_FINISH_SUCCESS "lis_finish_success"
#define VC_FINISH_SUCCESS "vc_finish_success"
#define CODRF_FINISH_SUCCESS "codrf_finish_success"
#define CAF_FINISH_SUCCESS "caf_finish_success"


/*
*  Success reply from COMMAND topic
*  Due to RESET Publish
*/

#define GNB_RESET_SUCCESS "gnb_reset_success"
#define UE_RESET_SUCCESS "ue_reset_success"
#define LIS_RESET_SUCCESS "lis_reset_success"
#define VC_RESET_SUCCESS "vc_reset_success"
#define CODRF_RESET_SUCCESS "codrf_reset_success"
#define CAF_RESET_SUCCESS "caf_reset_success"


/*
* Modules
*/
#define CAF "CAF"
#define CBF "CBF"
#define CgNBCF "CGNBCF"
#define CLISCF "CLISCF"
#define CVCF "CVCF"
#define CUECF "CUECF"
#define CODRF "CODRF"

// CBF States
#define STATE_IDLE 0
#define STATE_SCHEDULED 1
#define STATE_ON_TIME 2
#define STATE_DB_CONFIG_QUERY 3
#define STATE_SEND_CONFIG_TO_MODULES 4
#define STATE_WAIT_CONFIG_ACK 5
#define STATE_SEND_STR_CMD 6
#define STATE_WAIT_STR_ACK 7
#define STATE_RUNNING 8
#define STATE_RUNNING_TIME_EXP 9
#define STATE_SEND_FNSH_CMD 10
#define STATE_WAIT_FNSH_ACK 11
#define STATE_SEND_FNSH_DB_CMD 12
#define STATE_WAIT_FNSH_DB_ACK 13
#define STATE_RESET 14
#define STATE_FINISHED -1

// CBF Events
#define ACCEPTED 0
#define SCHEDULE_RECEIVED 1
#define ERROR 1
