#include "Random.h"
#include <stdlib.h>
#include <time.h>

Random::Random()
{
    // seed rand() so values can be different everytime
    srand(time(NULL));
}

int Random::integer(int min, int max)
{
    return rand() % (max - min + 1) - min;
}

std::vector<int> Random::integers(unsigned int total, int min, int max)
{
    std::vector<int> numbers;
    for (unsigned int i = 0; i < total; i++) {
        numbers.push_back(integer(min, max));
    }

    return numbers;
}
