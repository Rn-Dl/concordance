HEADERS = hashT.h
CC = gcc
CFLAGS = -c -Wall

all: concordance

concordance: concordance.o hashT.o
	$(CC) -o concordance concordance.o hashT.o

concordance.o: concordance.c $(HEADERS)
	$(CC) $(CFLAGS) concordance.c

hashT.o: hashT.c $(HEADERS)
	$(CC) $(CFLAGS) hashT.c

clean:
	rm *.o concordance