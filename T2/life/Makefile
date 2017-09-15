# the compiler: gcc for C program, define as g++ for C++
# Makefile for Lifegame
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -pthread -Wall -g

# the build target executable:
TARGET = ./src/main
EXECNAME = life
STRUCT = ./src/structs

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME) $(TARGET).c $(STRUCT).c

clean:
	$(RM) $(TARGET)
