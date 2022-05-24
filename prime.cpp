#include <iostream>
#include <unistd.h>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <vector>
#include <chrono>
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
    std::vector<int> &buffer,
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
    std::vector<int> &buffer,
    int bufferSize,
    int &counter,
    std::chrono::steady_clock::time_point start,
    bool verbose
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

        sem_post(&bufferFree);
        sem_post(&isEmpty);

        bool prime = isPrime(number);

        sem_wait(&counterFree);
        counter++;
        if (counter > 100000) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << duration;
            exit(EXIT_SUCCESS);
        }

        if (verbose) {
            if (prime) {
                std::cout << number << "\t\tprime" << std::endl;
            } else {
                std::cout << number << "\t\tnot prime" << std::endl;
            }
        }
        sem_post(&counterFree);
    }
}

int main(int argc, char const *argv[])
{
    unsigned int producerTotalThreads, consumerTotalThreads;
    std::vector<std::thread> producerThreads, consumerThreads;
    sem_t bufferFree, isEmpty, isNotEmpty, counterFree;
    std::chrono::steady_clock::time_point start;
    int counter = 0;
    bool verbose = true;

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <buffer-size> <producer-threads> <consumer-threads> [no-output]" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (argc >= 5 && std::string(argv[4]) == "no-output") {
        verbose = false;
    }

    Random random;

    int bufferSize = atoi(argv[1]);
    producerTotalThreads = atoi(argv[2]);
    consumerTotalThreads = atoi(argv[3]);

    // Populate buffer with empty values (in this case zeros)
    std::vector<int> buffer;
    for (int i = 0; i < bufferSize; i++) {
        buffer.push_back(0);
    }

    sem_init(&bufferFree, false, 1);       // If buffer is not being used
    sem_init(&isEmpty, false, bufferSize); // Empty buffer space
    sem_init(&isNotEmpty, false, 0);       // Occupied buffer space
    sem_init(&counterFree, false, 1);      // If counter is not being used

    start = std::chrono::steady_clock::now();

    for (unsigned int i = 0; i < producerTotalThreads; i++) {
        producerThreads.push_back(std::thread(
            producer,
            std::ref(bufferFree),
            std::ref(isEmpty),
            std::ref(isNotEmpty),
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
            std::ref(counter),
            start,
            verbose
        ));
    }

    for (std::thread &thread : producerThreads) {
        thread.join();
    }

    for (std::thread &thread : consumerThreads) {
        thread.join();
    }
}
