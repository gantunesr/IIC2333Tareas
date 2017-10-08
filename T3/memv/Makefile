# the compiler: gcc for C program, define as g++ for C++
# Makefile for Lifegame
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -g

# the build target executable:
TARGET = ./src/main
EXECNAME = mem_simulator

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME) $(TARGET).c

clean:
	$(RM) $(TARGET)
