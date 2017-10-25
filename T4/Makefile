# the compiler: gcc for C program, define as g++ for C++
# Makefile for Lifegame
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g

# the build target executable:
TARGET1 = ./client/main
EXECNAME1 = clnt
TARGET2 = ./server/main
EXECNAME2 = srvr

all:
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME1) $(TARGET1).c -lm
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME2) $(TARGET2).c -lm

clean:
	$(RM) $(TARGET)
