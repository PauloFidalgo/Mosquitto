#include "headers.h"
#include <mosquitto.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

struct mosquitto *mosq = NULL;
bool start = false;
bool end = false;

sqlite3 *database;

int initiate_database() {
    int rc = sqlite3_open("database.db", &database);

    char *error_message = 0;

    if (rc) {
        printf("Error creating Database\n");
        return 1;
    }

    char *drop = "DROP TABLE IF EXISTS CONTROL";

    rc = sqlite3_exec(database, drop, 0, 0, &error_message);
    if (rc != SQLITE_OK) {
        sqlite3_free(error_message);
        return 1;
    }

    char *sql_create_control = "CREATE TABLE IF NOT EXISTS CONTROL ("
                               "Timestamp DATETIME NOT NULL,"
                               "Topic TEXT NOT NULL,"
                               "Publisher TEXT NOT NULL,"
                               "Payload TEXT NOT NULL);";

    rc = sqlite3_exec(database, sql_create_control, 0, 0, &error_message);
    
    if (rc != SQLITE_OK) {
        sqlite3_free(error_message);
        return 1;
    }

    return 0;
}

void insert_data(const struct mosquitto_message *message) {
    char sql_insert[1000];
    char* time;
    char* publisher;
    char* payload;
    
    parse_json((char *)message->payload, &time, &publisher, &payload);

    sprintf(sql_insert, "INSERT INTO CONTROL (Timestamp, Topic, Publisher, Payload) VALUES ('%s', '%s', '%s', '%s');", time, (char *)message->topic, publisher, payload);

    char *error_message = 0;
    int rc = sqlite3_exec(database, sql_insert, 0, 0, &error_message);

    
    free(time);
    free(publisher);
    free(payload);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    } else {
        printf("Data inserted successfully\n");
    }
}

void handle_command(const struct mosquitto_message *message) {
    if (strcmp((char *)message->payload, START_COMMAND) == 0) {
        char *reply = CODRF_READY;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        start = true;
        printf("Received start command\n");
    }

    if (strcmp((char *)message->payload, EXPERIENCE_FINISHED) == 0) {
        char *reply = CODRF_ACK;
        mosquitto_publish(mosq, NULL, COMMAND, strlen(reply), reply, 1, true);
        end = true;
        printf("Received finish command\n");
    }
}

void initial_connection(struct mosquitto *mosq, void *userdata, int rc) {
    if (rc == 0) {
        printf("Connected to COMMAND broker from CODRF\n");
        mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    } else {
        fprintf(stderr, "Failed to connect to MQTT broker: %s\n", mosquitto_connack_string(rc));
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (strcmp(message->topic, COMMAND) == 0) {
        handle_command(message);
    }
    else{
        insert_data(message);
    }
   
}
int initial_config() {
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, initial_connection);
    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Unable to connect to MQTT broker.\n");
        return 1;
    }

    mosquitto_loop_start(mosq);

    return 0;
}

void config() {
    mosquitto_subscribe(mosq, NULL, RECONF, 1);
    mosquitto_subscribe(mosq, NULL, BEAM_CONF, 1);
    mosquitto_subscribe(mosq, NULL, LIS_CONF, 1);
    mosquitto_subscribe(mosq, NULL, GNB_RS, 1);
    mosquitto_subscribe(mosq, NULL, UE_RS, 1);
    mosquitto_subscribe(mosq, NULL, LIS_RS, 1);
    mosquitto_subscribe(mosq, NULL, VIDEO_S, 1);
    mosquitto_subscribe(mosq, NULL, DATA, 1);
    mosquitto_subscribe(mosq, NULL, LIS_RS, 1);
    mosquitto_subscribe(mosq, NULL, COMMAND, 1);
    printf("Connected to ALL topics\n");
}

int run() {
    while (!end);
    return 0;
}

void destroy() {
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int main() {
    if (initiate_database()) return 1;

    if (initial_config()) return 1;

    while (!start);

    config();

    run();

    destroy();

    return 0;
}
