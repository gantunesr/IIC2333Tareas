# the compiler: gcc for C program, define as g++ for C++
# Makefile for disk_scheduler
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g

# the build target executable:
TARGET = ./src/
EXECNAME = disk_scheduler

all:
	$(CC) $(CFLAGS) $(VM)-o $(EXECNAME) $(TARGET)main.c $(TARGET)structs.c

clean:
	$(RM) $(TARGET)
