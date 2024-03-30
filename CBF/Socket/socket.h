#define SOCKET_PORT 8001
#define SERVER_IP "127.0.0.1"

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

// GETTERS
#define GNB_PLACEMENT_STATUS "GET /gnb/placement-status/%s HTTP/1.1"
#define GNB_RADIO_COMMUNICATIONS_STATUS "GET /gnb/radiocommunications-status/%s HTTP/1.1"
#define GNB_RADIO_SENSING_STATUS "GET /gnb/radiosensing-status/%s HTTP/1.1"
#define GNB_VIDEO_SENSING_STATUS "GET /gnb/videosensing-status/%s HTTP/1.1"
#define GNB_XAPP_STATUS "GET /gnb/xapp-status/%s HTTP/1.1"

#define UE_PLACEMENT_STATUS "GET /UE/placement-status/%s HTTP/1.1"
#define UE_RADIO_COMMUNICATIONS_STATUS "GET /UE/radiocommunications-status/%s HTTP/1.1"
#define UE_RADIO_SENSING_STATUS "GET /UE/radiosensing-status/%s HTTP/1.1"
#define UE_VIDEO_SENSING_STATUS "GET /UE/videosensing-status/%s HTTP/1.1"

#define LIS_PLACEMENT_STATUS "GET /LIS/placement-status/%s HTTP/1.1"
#define LIS_RADIO_COMMUNICATIONS_STATUS "GET /LIS/radiocommunications-status/%s HTTP/1.1"
#define LIS_RADIO_SENSING_STATUS "GET /LIS/radiosensing-status/%s HTTP/1.1"
#define LIS_VIDEO_SENSING_STATUS "GET /LIS/videosensing-status/%s HTTP/1.1"

#define CTCF_STATUS "GET /CTCF/status/%s HTTP/1.1"

#define _3D_S_STATUS "GET /3DS/status HTTP/1.1"

#define VRS_STATUS "GET /VRS/status HTTP/1.1"

#define NETS_STATUS "GET /NETS/status HTTP/1.1"

#define ML_MODEL_DOWNLOAD "GET /ML/download HTTP/1.1"

#define ODR_DATASET_DOWNLOAD "GET /ODR/download HTTP/1.1"

#define EXPERIMENT_STATUS "GET /CBF/status HTTP/1.1"