/*
 * File that defines headers for use case 1
 * Change IP and Port according to your needs
 */
#define MQTT_HOST "172.20.10.8"
#define MQTT_PORT 8443

#define DELAY_US 1000000
#define EXPERIMENT_TIME_S 5
#define ACK_TIMEOUT 10

/*
 * Messages
 */
#define START_COMMAND "START"
#define FINISH_COMMAND "FINISH"
#define RESET_COMMAND "reset"


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

/*
 * CBF States
 */
#define STATE_IDLE 0
#define STATE_SCHEDULED 1
#define STATE_ON_TIME 2
#define STATE_SEND_CONFIG_TO_MODULES 3
#define STATE_SEND_STR_CMD 4
#define STATE_RUNNING 5
#define STATE_RUNNING_TIME_EXP 6
#define STATE_WAIT_FNSH_ACK 7
#define STATE_SEND_FNSH_DB_CMD 8
#define STATE_WAIT_FNSH_DB_ACK 9
#define STATE_RESET 10
#define STATE_FINISHED -1

/*
 * CBF Events
 */
#define ACCEPTED 0
#define SCHEDULE_RECEIVED 1
#define ERROR 1


/*
 * CBF ACK States
 */
#define ACK_START_EXPERIMENT "ack_start"
#define ACK_FINISH_EXPERIMENT "ack_finish"
#define ACK_RESET "ack_reset"

#define ERROR_START_EXPERIMENT "error_start_experiment"
#define ERROR_FINISH_EXPERIMENT "error_finish_experiment"
#define ERROR_RESET "error_reset"


