all: CLISCF SUB PUB
	echo "Build finish!"

CLISCF: CLISCF.c
	gcc -Wall CLISCF.c -o CLISCF -lmosquitto

SUB: SUB.c
	gcc -Wall SUB.c -o SUB -lmosquitto

PUB: PUB.c
	gcc -Wall PUB.c -o PUB -lmosquitto

clean:
	rm CLISCF SUB PUB