CC=gcc
CFLAGS=-Wall -std=c99
LIBS=-L/opt/homebrew/Cellar/mosquitto/2.0.18/lib -lmosquitto -lsqlite3 -L/opt/homebrew/Cellar/cjson/1.7.17/lib -lcjson
INCLUDE_PATH=-I/opt/homebrew/Cellar/mosquitto/2.0.18/include -I/opt/homebrew/Cellar/cjson/1.7.17/include/cjson -I./Utils -I./Mosquitto
INCLUDE_PATHI=-I/opt/homebrew/Cellar/mosquitto/2.0.18/include -I/opt/homebrew/Cellar/cjson/1.7.17/include/cjson -I./Utils -I./Mosquitto -I./Socket

UTILS_LIBI=./Utils/utils.a ./Mosquitto/CBFMQTT.a ./Socket/http_server.a
UTILS_LIB=./Utils/utils.a ./Mosquitto/CBFMQTT.a
all: CBF CgNBCF

CBF: Modules/CBF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATHI) -o $@ $< $(UTILS_LIBI) $(LIBS)

CgNBCF: Modules/CgNBCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CLISCF: Modules/CLISCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CUECF: Modules/CUECF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CVCF: Modules/CVCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CBFMQTT: CBFMQTT.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)
	
clean:
	rm -f CBF CgNBCF


