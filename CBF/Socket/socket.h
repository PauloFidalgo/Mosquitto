#define SOCKET_PORT 8002
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


#define GET_CONFIG "GET /get_config HTTP/1.1"