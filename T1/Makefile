# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
MATH = -lm
VM = -std=c99

# the build target executable:
TARGET = ./src/simulator
EXECNAME = simulator

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(VM) -o $(EXECNAME) $(TARGET).c $(MATH)

clean:
	$(RM) $(TARGET)
