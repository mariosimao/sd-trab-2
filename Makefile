# Compiler
CC = g++

# Wall = enable compiler warnings
CFLAGS = -Wall -std=c++11
LIBS = -pthread

SUM_INCLUDE = sum.cpp Lock.cpp Random.cpp
SUM_EXECUTABLE = sum

all: sum

sum:
	$(CC) $(CFLAGS) $(LIBS) -o $(SUM_EXECUTABLE) $(SUM_INCLUDE)

clean:
	$(RM) $(SUM_EXECUTABLE)
