#By Ethan Anderson and Seth Baugh
# 1/18/18
# Poly Makefile

#Set flags
CC = gcc
CFLAGS = -g
LDFLAGS = -g

#make poly with BOTH poly.o and dump.o 
poly: poly.o dump.o
	$(CC) $(LDFLAGS) -o poly poly.o dump.o
#make dump.o
dump.o: dump.c
	$(CC) $(CFLAGS) -c dump.c
#make poly.o
poly.o: poly.c
	$(CC) $(CFLAGS) -c poly.c
