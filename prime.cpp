#include <iostream>
#include <unistd.h>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <vector>
#include "Random.h"

bool isPrime(int n)
{
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

void producer(
    sem_t &bufferFree,
    sem_t &isEmpty,
    sem_t &isNotEmpty,
    sem_t &counterFree,
    int buffer[],
    int bufferSize,
    Random &random
) {
    while (1) {
        int number = random.integer(1, 10000);
        sem_wait(&isEmpty);
        sem_wait(&bufferFree);

        // Write on first empty space
        for(int i = 0; i < bufferSize; i++) {
            if (buffer[i] == 0) {
                buffer[i] = number;
                break;
            }
        }

        sem_post(&bufferFree);
        sem_post(&isNotEmpty);
    }
}

void consumer(
    sem_t &bufferFree,
    sem_t &isEmpty,
    sem_t &isNotEmpty,
    sem_t &counterFree,
    int buffer[],
    int bufferSize,
    int &counter
) {
    while (1) {
        int number;
        sem_wait(&isNotEmpty);
        sem_wait(&bufferFree);

        // Read first non-empty space and free its position on buffer
        for(int i = 0; i < bufferSize; i++) {
            if (buffer[i] != 0) {
                number = buffer[i];
                buffer[i] = 0;
                break;
            }
        }

        counter++;
        sem_post(&bufferFree);
        sem_post(&isEmpty);

        // bool prime = isPrime(number);
        // sem_wait(&counterFree);
        if (counter > 100000) {
            exit(EXIT_SUCCESS);
        }
        if (counter % 10000 == 0) {
            // std::cout << counter << std::endl;
        }
        // sem_post(&counterFree);

        // char result[100];
        // if (prime) {
        //     sprintf(result, "%d\t\tprime\n", number);
        // } else {
        //     sprintf(result, "%d\t\tnot prime\n", number);
        // }
        // std::cout << result;
    }
}

int main(int argc, char const *argv[])
{
    unsigned int producerTotalThreads, consumerTotalThreads;
    std::vector<std::thread> producerThreads, consumerThreads;
    sem_t bufferFree, isEmpty, isNotEmpty, counterFree;
    int counter = 0;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "<buffer-size> <producer-threads> <consumer-threads>" << std::endl;
        exit(EXIT_FAILURE);
    }

    Random random;

    producerTotalThreads = atoi(argv[1]);
    consumerTotalThreads = atoi(argv[2]);
    const int bufferSize = 4;

    // std::vector<int> buffer;
    // for (int i = 0; i < bufferSize; i++) {
    //     buffer.push_back(0);
    // }
    int buffer[bufferSize] = {0}; // Initially populate withe zeros. Zero means that buffer space is empty.

    sem_init(&bufferFree, false, 1);       // If buffer is being used
    sem_init(&isEmpty, false, bufferSize); // Empty buffer space
    sem_init(&isNotEmpty, false, 0);       // Occupied buffer space
    sem_init(&counterFree, false, 1);      // If counter is being used

    for (unsigned int i = 0; i < producerTotalThreads; i++) {
        producerThreads.push_back(std::thread(
            producer,
            std::ref(bufferFree),
            std::ref(isEmpty),
            std::ref(isNotEmpty),
            std::ref(counterFree),
            std::ref(buffer),
            bufferSize,
            std::ref(random)
        ));
    }

    for (unsigned int i = 0; i < consumerTotalThreads; i++) {
        consumerThreads.push_back(std::thread(
            consumer,
            std::ref(bufferFree),
            std::ref(isEmpty),
            std::ref(isNotEmpty),
            std::ref(counterFree),
            std::ref(buffer),
            bufferSize,
            std::ref(counter)
        ));
    }

    for (std::thread &thread : producerThreads) {
        thread.join();
    }

    for (std::thread &thread : consumerThreads) {
        thread.join();
    }
}
