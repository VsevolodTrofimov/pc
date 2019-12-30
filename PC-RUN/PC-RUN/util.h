#pragma once
#include <stdlib.h>
#include <iostream>

inline void fill(int* M, const size_t& size, const int& seed)
{
    srand(seed);

    for (auto i = 0; i < size; ++i)
    {
        M[i] = rand() % 10;
    }
}

inline void print2d(const int* M, const size_t& h, const size_t& w)
{
    for (auto y = 0; y < h; ++y)
    {
        for (auto x = 0; x < w; ++x)
        {

            std::cout << M[w * y + x] << "\t";
        }
        std::cout << std::endl;
    }
}
