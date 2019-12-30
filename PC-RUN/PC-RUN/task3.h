#pragma once
#include <iostream>
#include <omp.h>
#include <chrono>
#include "util.h"


inline void mult(int* A, int* b, int* c, const size_t& ah, const size_t& aw, const size_t& bl)
{
    if (aw != bl)
    {
        throw "Matrix dimensions must agree";
    }

    for (auto y = 0; y < ah; ++y)
    {
        auto shift = aw * y;
        auto sum = 0;
        for (auto x = 0; x < aw; ++x) {
            sum += A[shift + x] * b[x];
        }
        c[y] = sum;
    }
}

// best especitally for a row based matrix
inline void multRow(int* A, int* b, int* c, const size_t& ah, const size_t& aw, const size_t& bl)
{
    if (aw != bl)
    {
        throw "Matrix dimensions must agree";
    }

#pragma omp parallel for
    for (auto y = 0; y < ah; ++y)
    {
        auto shift = aw * y;
        auto sum = 0;
        for (auto x = 0; x < aw; ++x) {
            sum += A[shift + x] * b[x];
        }
        c[y] = sum;
    }
}

// of couse bad for row based matrix, but bad for parallel anyway
inline void multCol(int* A, int* b, int* c, const size_t& ah, const size_t& aw, const size_t& bl)
{
    if (aw != bl)
    {
        throw "Matrix dimensions must agree";
    }

    for (auto i = 0; i < ah; ++i) {
        c[i] = 0;
    }

#pragma omp parallel for
    for (auto x = 0; x < aw; ++x)
    {
        auto bVal = b[x];
        for (auto y = 0; y < ah; ++y) {
            c[y] += A[aw * y + x] * bVal;
        }
    }
}

// only good if Matrix has a proper interface and we are not sure if it is row or col based
inline void multBlock(int* A, int* b, int* c, const size_t& ah, const size_t& aw, const size_t& bl)
{
    if (aw != bl)
    {
        throw "Matrix dimensions must agree";
    }

    for (auto i = 0; i < ah; ++i) {
        c[i] = 0;
    }

    
#pragma omp parallel
    {

        auto threads = omp_get_num_threads();
        // actually insanely suboptimal way to subdivide a matrix (but it is likely to be optimized into an almost row based approach)
        auto blockHeight = ah / threads;
        auto blockWidth = aw / threads;
        auto total = threads;



#pragma omp for
        for (auto block = 0; block < total; ++block) {
            auto blockY = block / threads;
            auto blockX = block % threads;

            auto yStart = blockY * blockHeight;
            auto yEnd = block == total - 1 ? ah : yStart + blockHeight;

            auto xStart = blockY * blockWidth;
            auto xEnd = block == total - 1 ? aw : xStart + blockWidth;

            for (int y = yStart; y < yEnd; ++y) {
                auto shift = aw * y;
                for (int x = xStart; x < xEnd; ++x) {
                    c[y] += A[shift + x] * b[x];
                }
            }
       
        }
    }
}


int task3(int seed = 0, int threads = 2)
{
    std::cout << "\n---" << seed << "---" << threads << "---\n\n";

    srand(seed);
    auto begin = std::chrono::system_clock::now(), end = std::chrono::system_clock::now();
    double tRow = 0;
    double tCol = 0;
    double tBlk = 0;
    double tSeq = 0;

    omp_set_num_threads(threads);

    for (auto i = 0; i < 20; ++i)
    {
        const int iterSeed = rand();
        std::cout << "\r" << i + 1 << std::flush;

        const size_t ah = rand() % 1000 + 1000;
        const size_t aw = rand() % 1000 + 1000;
        const size_t bl = aw;
        const size_t cl = ah;

        auto A = new int[ah * aw];
        auto b = new int[bl];
        auto c = new int[cl];

        fill(A, ah * aw, rand());
        fill(b, bl, rand());
        
        // seq
        begin = std::chrono::system_clock::now();
        for (auto r = 0; r < 5; ++r)
        {
            mult(A, b, c, ah, aw, bl);
        }
        end = std::chrono::system_clock::now();
        tSeq += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        // row
        begin = std::chrono::system_clock::now();
        for (auto r = 0; r < 5; ++r)
        {
            multRow(A, b, c, ah, aw, bl);
        }
        end = std::chrono::system_clock::now();
        tRow += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        // col
        begin = std::chrono::system_clock::now();
        for (auto r = 0; r < 5; ++r)
        {
            multCol(A, b, c, ah, aw, bl);
        }
        end = std::chrono::system_clock::now();
        tCol += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        // block
        begin = std::chrono::system_clock::now();
        for (auto r = 0; r < 5; ++r)
        {
            multBlock(A, b, c, ah, aw, bl);
        }
        end = std::chrono::system_clock::now();
        tBlk += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    }

    std::cout << "\r"
        << "S:" << tSeq / 1000 * 100 << "\n" 
        << "R:" << tRow / 1000 * 100 << "\n" 
        << "C:" << tCol / 1000 * 100 << "\n" 
        << "B:" << tBlk / 1000 * 100 << "\n" << std::endl;

    if (seed == 0) {
        return task3(777, threads);
    }

    if (seed == 777) {
        return task3(421512, threads);
    }

    if (seed == 421512) {
        if (threads == 2) {
            return task3(0, 4);
        }

        if (threads == 4) {
            return task3(0, 6);
        }

        if (threads == 6) {
            return task3(0, 8);
        }

        if (threads == 8) {
            return task3(0, 12);
        }
    }
}