#ifndef _SOCKET_H_
#define _SOCKET_H_

#define SOCKET_PORT 8001
#define SERVER_IP "172.20.10.8"

typedef enum {
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501
} HTTP_status;

typedef struct {
    char *body;
    HTTP_status status;
} HTTP_response;

// METHOD
#define POST "POST"
#define GET "GET"

// MODULES
#define GNB "gnb"
#define UE "ue"
#define LIS "lis"

// TYPE
#define PLACEMENT_SETUP "placement-setup"
#define RADIO_COMMUNICATIONS_SETUP "placement-setup"
#define RADIO_SENSING_SETUP "radio_sensing_setup"
#define VIDEO_SENSING_SETUP "video_sensing_setup"
#define X_APP_SENSING_SETUP "x_app_setup"


// GETTERS
#define GNB_PLACEMENT_STATUS "/gnb/placement/"
#define GNB_RADIO_COMMUNICATIONS_STATUS "/gnb/radio_communications_status/%s"
#define GNB_RADIO_SENSING_STATUS "/gnb/radio_sensing_status/%s"
#define GNB_VIDEO_SENSING_STATUS "/gnb/video_sensing_status/%s"
#define GNB_X_APP_STATUS "/gnb/x_app_status/%s"

#define UE_PLACEMENT_STATUS "GET/UE/placement_status/%s"
#define UE_RADIO_COMMUNICATIONS_STATUS "GET/UE/radio_communications_status/%s"
#define UE_RADIO_SENSING_STATUS "GET/UE/radio_sensing_status/%s"
#define UE_VIDEO_SENSING_STATUS "GET/UE/video_sensing_status/%s"

#define LIS_PLACEMENT_STATUS "GET/LIS/placement-status/%s"
#define LIS_RADIO_COMMUNICATIONS_STATUS "GET/LIS/radio_communications_status/%s"
#define LIS_RADIO_SENSING_STATUS "GET/LIS/radio_sensing_status/%s"
#define LIS_VIDEO_SENSING_STATUS "GET/LIS/video_sensing_status/%s"

#define CTCF_STATUS "GET/CTCF/status/%s"

#define _3D_S_STATUS "GET/3DS/status"

#define VRS_STATUS "GET/VRS/status"

#define NETS_STATUS "GET/NETS/status"

#define ML_MODEL_DOWNLOAD "GET/ML/download"

#define ODR_DATASET_DOWNLOAD "GET/ODR/download"

#define EXPERIMENT_STATUS "GET/CBF/status"

#endif /* _SOCKET_H_ */

