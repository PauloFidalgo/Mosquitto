CC=gcc
CFLAGS=-Wall -std=c99
LIBS=-L/opt/homebrew/Cellar/mosquitto/2.0.18/lib -lmosquitto -lsqlite3 -L/opt/homebrew/Cellar/cjson/1.7.17/lib -lcjson
INCLUDE_PATH=-I/opt/homebrew/Cellar/mosquitto/2.0.18/include -I/opt/homebrew/Cellar/cjson/1.7.17/include/cjson

UTILS_LIB=utils.a
all: CBF CBFMQTT

CBF: CBF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CAF: CAF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CgNBCF: CgNBCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CLISCF: CLISCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CUECF: CUECF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CVCF: CVCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CODRF: CODRF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

CBFMQTT: CBFMQTT.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(UTILS_LIB) $(LIBS)

clean:
	rm -f CBF CBFMQTT


