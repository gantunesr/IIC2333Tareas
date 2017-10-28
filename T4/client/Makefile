# the compiler: gcc for C program, define as g++ for C++
# Makefile for Lifegame
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g -pthread

# the build target executable:
TARGET1 = ./src/main
EXECNAME1 = clnt
EXTRA = ./src/board.c ./src/requests.c

all:
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME1) $(TARGET1).c $(EXTRA) -lm

clean:
	$(RM) $(TARGET)
