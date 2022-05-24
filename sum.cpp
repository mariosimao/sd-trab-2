#include <iostream>     // std::cout, std::cerr, std::endl
#include <vector>       // std::vector
#include <thread>       // std::thread
#include <chrono>       // elapsed time

#include "Random.h"
#include "Lock.h"

void sumParcel(std::vector<int> numbers, int &sum, Lock &lock)
{
    int partialSum = 0;
    for (int number : numbers) {
        partialSum += number;
    }

    lock.acquire();
    sum += partialSum;
    lock.release();
}

int main(int argc, char const *argv[])
{
    unsigned int totalNumbers, totalThreads, numbersPerThread, numbersRemainded;
    std::vector<int> numbers;
    std::vector<std::thread> threads;
    std::chrono::steady_clock::time_point start, end;
    int duration;
    int sum = 0;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <total-numbers> <total-threads>" << std::endl;
        exit(EXIT_FAILURE);
    }

    totalNumbers = atoi(argv[1]);
    totalThreads = atoi(argv[2]);

    Random random;
    numbers = random.integers(totalNumbers, -100, 100);

    numbersPerThread = totalNumbers / totalThreads;
    numbersRemainded = totalNumbers % totalThreads;

    sum = 0;
    int currentIndex = 0;

    start = std::chrono::steady_clock::now();

    Lock lock;
    for (unsigned int i = 0; i < totalThreads; i++) {
        /**
         * Split division remainder among threads
         */
        int numbersOnThread = numbersPerThread;
        if (i < numbersRemainded) {
            numbersOnThread += 1;
        }

        /**
         * Subvector with only numbers to be used on the parcel sum
         */
        std::vector<int> parcelNumbers(
            numbers.begin() + currentIndex,
            numbers.begin() + currentIndex + numbersOnThread
        );
        threads.push_back(std::thread(sumParcel, parcelNumbers, std::ref(sum), std::ref(lock)));

        currentIndex += numbersOnThread;
    }

    /**
     * Synchronize threads
     */
    for (std::thread &thread : threads) {
        thread.join();
    }

    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // std::cout << "Sum: " << sum << std::endl;
    // std::cout << "Time: " << duration << " ms" << std::endl;
    std::cout << duration;

    if (argc > 3 && std::string(argv[3]) == "compare") {
        std::chrono::steady_clock::time_point singleThreadStart, singleThreadEnd;
        int64_t singleThreadDuration;

        singleThreadStart = std::chrono::steady_clock::now();

        int singleThreadSum = 0;
        for (int num : numbers) {
            singleThreadSum += num;
        }

        singleThreadEnd = std::chrono::steady_clock::now();
        singleThreadDuration = std::chrono::duration_cast<std::chrono::milliseconds>(singleThreadEnd - singleThreadStart).count();

        std::cout << std::endl << "Single thread" << std::endl;
        std::cout << "Time: " << singleThreadDuration << " ms" << std::endl;

        if (singleThreadSum == sum) {
            std::cout << "Check: ✅" << std::endl;
        } else {
            std::cout << "Check: ❌" << std::endl;
        }
    }

    return 0;
}

/**
 * References
 *
 * Int size:                  https://www.cplusplus.com/doc/tutorial/variables/
 * Why not use namespace std: https://stackoverflow.com/a/1452738/11591212
 * test_and_set:              https://www.cplusplus.com/reference/atomic/atomic_flag/test_and_set/
 * threads:                   https://www.cplusplus.com/reference/thread/thread/
 * Threads arg by ref:        https://en.cppreference.com/w/cpp/thread/thread/thread#Notes
 */

