CC=gcc
CFLAGS=-Wall -std=c99
LIBS=-L/opt/homebrew/Cellar/mosquitto/2.0.18/lib -lmosquitto
INCLUDE_PATH=-I/opt/homebrew/Cellar/mosquitto/2.0.18/include

all: CgNBCF CLISCF CUECF PUB SUB

CgNBCF: CgNBCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(LIBS)

CLISCF: CLISCF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(LIBS)

CUECF: CUECF.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(LIBS)

PUB: PUB.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(LIBS)

SUB: SUB.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $< $(LIBS)

clean:
	rm -f CgNBCF CLISCF CUECF PUB SUB


