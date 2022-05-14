#ifndef RANDOM_H
#define RANDOM_H

#include <vector>

class Random
{
    public:
        Random();
        int integer(int min, int max);
        std::vector<int> integers(unsigned int total, int min, int max);
};


#endif