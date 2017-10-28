# the compiler: gcc for C program, define as g++ for C++
# Makefile for Lifegame
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g -pthread

# the build target executable:
TARGET2 = ./src/main
EXECNAME2 = srvr

all:
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME2) $(TARGET2).c -lm

clean:
	$(RM) $(TARGET)
