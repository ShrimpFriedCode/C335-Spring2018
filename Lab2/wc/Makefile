#mywc Makefile
#Ethan Anderson
#1/18/18

CC = gcc
CFLAGS = -g
LDFLAGS = -g

mywc: mywc.o
	$(CC) $(LDFLAGS) -o mywc mywc.o

mywc.o: mywc.c
	$(CC) $(CFLAGS) -c mywc.c
