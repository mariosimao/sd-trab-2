# Compiler
CC = g++

# Wall = enable compiler warnings
CFLAGS = -Wall -std=c++11
LIBS = -pthread

SUM_INCLUDE = sum.cpp Lock.cpp Random.cpp
SUM_EXECUTABLE = sum

PRIME_INCLUDE = prime.cpp Random.cpp
PRIME_EXECUTABLE = prime

all: sum prime

sum:
	$(CC) $(CFLAGS) $(LIBS) -o $(SUM_EXECUTABLE) $(SUM_INCLUDE)

prime:
	$(CC) $(CFLAGS) $(LIBS) -o $(PRIME_EXECUTABLE) $(PRIME_INCLUDE)

clean:
	$(RM) $(SUM_EXECUTABLE) $(PRIME_EXECUTABLE)
