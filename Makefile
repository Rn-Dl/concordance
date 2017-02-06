HEADERS = hashT.h
CC = gcc
CCP = g++
CFLAGS = -c -Wall

all: concordance

cplus: concordance.cpp
	$(CCP) -o concordance concordance.cpp

concordance: concordance.o hashT.o
	$(CC) -o concordance concordance.o hashT.o

concordance.o: concordance.c $(HEADERS)
	$(CC) $(CFLAGS) concordance.c

hashT.o: hashT.c $(HEADERS)
	$(CC) $(CFLAGS) hashT.c

clean:
	rm *.o concordance *.out