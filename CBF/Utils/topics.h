/*  
 * Configuration Topics
 */
#define CGNBCF_SETUP "cgnbcf/setup" 
#define CLISCF_SETUP "cliscf/setup"
#define CVCF_SETUP "cvcf/setup"
#define CORDF_SETUP "cordf/setup"
#define CUECF_SETUP "cuecf/setup"
#define COMMAND "command"
#define ERRORS "errors"
#define RECONF "reconfiguration"
#define BEAM_CONF "beam/config"
#define LIS_CONF "cliscf/config"
#define GNB_RS "CgNBCF/radio_sensing"
#define UE_RS "ue/radio_sensing"
#define LIS_RS "cliscf/radio_sensing"
#define VIDEO_S "video_sensing"
#define DATA "data"
#define CAF_ERR "caf/error"
#define CAF_SCHEDULE "caf/schedule"

/*
 * GET topics
 */
#define GET_GNB_PLACEMENT_ST_TP "cgnbcf/get_placement_tp"
#define GET_GNB_RADIO_COMM_ST_TP "cgnbcf/get_radio_comm_tp"
#define GET_GNB_RADIO_SENSING_ST_TP "cgnbcf/get_radio_sensing_tp"
#define GET_GNB_VIDEO_SENSING_ST_TP "cgnbcf/get_video_sensing_tp"
#define GET_GNB_X_APP_ST_TP "cgnbcf/get_x_app_tp"

#define GET_UE_PLACEMENT_ST_TP "ue/get_placement_tp"
#define GET_UE_RADIO_COMM_ST_TP "ue/get_radio_comm_tp"
#define GET_UE_RADIO_SENSING_ST_TP "ue/get_radio_sensing_tp"
#define GET_UE_VIDEO_SENSING_ST_TP "ue/get_video_sensing_tp"

#define GET_LIS_PLACEMENT_ST_TP "LIS/get_placement_tp"
#define GET_LIS_RADIO_COMM_ST_TP "LIS/get_radio_comm_tp"
#define GET_LIS_RADIO_SENSING_ST_TP "LIS/get_radio_sensing_tp"
#define GET_LIS_VIDEO_SENSING_ST_TP "LIS/get_video_sensing_tp"

#define GET_CTCF_ST_TP "CTCF/get_st_tp"
#define GET_3D_S_ST_TP "3ds/get_st_tp"
#define GET_VRS_ST_TP "vrs/get_st_tp"
#define GET_NET_ST_TP "NET/get_st_tp"
#define GET_ML_DW_TP "ML/get_dw_tp"
#define GET_ODR_DW_TP "odr/get_dw_tp"
#define GET_EXP_ST_TP "cbf/get_exp_st_tp"

/*
 * POST topics
 */
#define POST_GNB_PLACEMENT_TP "cgnbcf/post_placement_tp"
#define POST_GNB_RADIO_COMM_TP "cgnbcf/post_radio_comm_tp"
#define POST_GNB_RADIO_SENSING_TP "cgnbcf/post_radio_sensing_tp"
#define POST_GNB_VIDEO_SENSING_TP "cgnbcf/post_video_sensing_tp"
#define POST_GNB_X_APP_TP "cgnbcf/post_x_app_tp"

#define POST_UE_PLACEMENT_TP "ue/post_placement_tp"
#define POST_UE_RADIO_COMM_TP "ue/post_radio_comm_tp"
#define POST_UE_RADIO_SENSING_TP "ue/post_radio_sensing_tp"
#define POST_UE_VIDEO_SENSING_TP "ue/post_video_sensing_tp"

#define POST_LIS_PLACEMENT_TP "LIS/post_placement_tp"
#define POST_LIS_RADIO_COMM_TP "LIS/post_radio_comm_tp"
#define POST_LIS_RADIO_SENSING_TP "LIS/post_radio_sensing_tp"
#define POST_LIS_VIDEO_SENSING_TP "LIS/post_video_sensing_tp"

#define POST_CTCF_TP "CTCF/post_tp"
#define POST_3D_S_TP "3ds/post_tp"
#define POST_VRS_TP "vrs/post_tp"
#define POST_NET_TP "NET/post_tp"
#define POST_ML_DW_TP "ML/post_dw_tp"
#define POST_ODR_DW_TP "odr/post_dw_tp"
#define POST_EXP_TP "cbf/post_exp_tp"


/*
 * Get Acknowledge
 */
#define GET_GNB_PLACEMENT_ST_ACK_TP "cgnbcf/get_placement_st_ack_tp"
#define GET_GNB_RADIO_COMM_ST_ACK_TP "cgnbcf/get_radio_comm_st_ack_tp"
#define GET_GNB_RADIO_SENSING_ST_ACK_TP "cgnbcf/get_radio_sensing_st_ack_tp"
#define GET_GNB_VIDEO_SENSING_ST_ACK_TP "cgnbcf/get_video_sensing_st_ack_tp"
#define GET_GNB_X_APP_ST_ACK_TP "cgnbcf/get_x_app_st_ack_tp"

#define GET_UE_PLACEMENT_ST_ACK_TP "ue/get_placement_st_ack_tp"
#define GET_UE_RADIO_COMM_ST_ACK_TP "ue/get_radio_comm_st_ack_tp"
#define GET_UE_RADIO_SENSING_ST_ACK_TP "ue/get_radio_sensing_st_ack_tp"
#define GET_UE_VIDEO_SENSING_ST_ACK_TP "ue/get_video_sensing_st_ack_tp"

#define GET_LIS_PLACEMENT_ST_ACK_TP "LIS/get_placement_st_ack_tp"
#define GET_LIS_RADIO_COMM_ST_ACK_TP "LIS/get_radio_comm_st_ack_tp"
#define GET_LIS_RADIO_SENSING_ST_ACK_TP "LIS/get_radio_sensing_st_ack_tp"
#define GET_LIS_VIDEO_SENSING_ST_ACK_TP "LIS/get_video_sensing_st_ack_tp"

#define GET_CTCF_ST_ACK_TP "CTCF/get_st_ack_tp"
#define GET_3D_S_ST_ACK_TP "3ds/get_st_ack_tp"
#define GET_VRS_ST_ACK_TP "vrs/get_st_ack_tp"
#define GET_NET_ST_ACK_TP "NET/get_st_ack_tp"
#define GET_ML_DW_ACK_TP "ML/get_dw_ack_tp"
#define GET_ODR_DW_ACK_TP "odr/get_dw_st_ack_tp"
#define GET_EXP_ST_ACK_TP "cbf/get_exp_st_ack_tp"


/*
 * Post Acknowledge
 */
#define POST_GNB_PLACEMENT_ACK_TP "cgnbcf/post_placement_ack_tp"
#define POST_GNB_RADIO_COMM_ACK_TP "cgnbcf/post_radio_comm_ack_tp"
#define POST_GNB_RADIO_SENSING_ACK_TP "cgnbcf/post_radio_sensing_ack_tp"
#define POST_GNB_VIDEO_SENSING_ACK_TP "cgnbcf/post_video_sensing_ack_tp"
#define POST_GNB_X_APP_ACK_TP "cgnbcf/post_x_app_ack_tp"

#define POST_UE_PLACEMENT_ACK_TP "ue/post_placement_st_ack_tp"
#define POST_UE_RADIO_COMM_ACK_TP "ue/post_radio_comm_st_ack_tp"
#define POST_UE_RADIO_SENSING_ACK_TP "ue/post_radio_sensing_ack_tp"
#define POST_UE_VIDEO_SENSING_ACK_TP "ue/post_video_sensing_ack_tp"

#define POST_LIS_PLACEMENT_ACK_TP "LIS/post_placement_ack_tp"
#define POST_LIS_RADIO_COMM_ACK_TP "LIS/post_radio_comm_ack_tp"
#define POST_LIS_RADIO_SENSING_ACK_TP "LIS/post_radio_sensing_ack_tp"
#define POST_LIS_VIDEO_SENSING_ACK_TP "LIS/post_video_sensing_ack_tp"

#define POST_CTCF_ACK_TP "CTCF/post_ack_tp"
#define POST_3D_S_ACK_TP "3ds/post_ack_tp"
#define POST_VRS_ACK_TP "vrs/post_ack_tp"
#define POST_NET_ACK_TP "NET/post_ack_tp"
#define POST_ML_DW_ACK_TP "ML/post_dw_ack_tp"
#define POST_ODR_DW_ACK_TP "odr/post_dw_ack_tp"
#define POST_EXP_ACK_TP "cbf/post_exp_ack_tp"

