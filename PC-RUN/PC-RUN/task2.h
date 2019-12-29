#include <iostream>
#include <omp.h>
#include <chrono>
#include "util.h"

inline int multiplyP(int* A, int* B, int* C, const size_t& ah, const size_t& aw, const size_t& bh, const size_t& bw)
{
    const int cw = bw;
    const int ch = ah;
    const int size = ch * cw;

    if (aw != bh)
    {
        throw "Matrix dimensions must agree";
    }

#pragma omp parallel for
    for (auto idx = 0; idx < size; ++idx)
    {
        const int y = idx / cw;
        const int x = idx % cw;

        int sum = 0;
        for (auto i = 0; i < bh; ++i)
        {
            sum += A[aw * y + i] * B[bw * i + x];
        }

        C[idx] = sum;
    }
}

inline int multiply(int* A, int* B, int* C, const size_t& ah, const size_t& aw, const size_t& bh, const size_t& bw)
{
    const int cw = bw;
    const int ch = ah;
    const int size = ch * cw;

    if (aw != bh)
    {
        throw "Matrix dimensions must agree";
    }

    for (auto idx = 0; idx < size; ++idx)
    {
        const int y = idx / cw;
        const int x = idx % cw;

        int sum = 0;
        for (auto i = 0; i < bh; ++i)
        {
            sum += A[aw * y + i] * B[bw * i + x];
        }

        C[idx] = sum;
    }
}

inline bool equals(const int* A, const int* B, const size_t& size)
{
    for (auto i = 0; i < size; ++i)
    {
        if (A[i] != B[i])
        {
            return false;
        }
    }

    return true;
}

int task2()
{
    omp_set_num_threads(6);

    const int h = 20000;
    const int m = 100;
    const int w = 6000;

    srand(999);
    const auto seed = rand();
    auto begin = std::chrono::system_clock::now(), end = std::chrono::system_clock::now();

    double avgSeq = 0;
    double avgPar = 0;
    int res = 0;
    for (auto i = 0; i < 20; ++i)
    {
        std::cout << "\r" << i + 1 << std::flush;


        const size_t h = rand() % 300 + 300;
        const size_t m = rand() % 300 + 300;
        const size_t w = rand() % 300 + 300;

        const size_t ah = h;
        const size_t aw = m;
        const size_t bh = m;
        const size_t bw = w;

        auto A = new int[ah * aw];
        auto B = new int[bh * bw];
        auto C = new int[h * w];
        fill(A, ah * aw, rand());
        fill(B, bh * bw, rand());

        begin = std::chrono::system_clock::now();
        res = multiplyP(A, B, C, ah, aw, bh, bw);
        end = std::chrono::system_clock::now();
        avgPar += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        begin = std::chrono::system_clock::now();
        res = multiply(A, B, C, ah, aw, bh, bw);
        end = std::chrono::system_clock::now();
        avgSeq += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    }

    std::cout << "\r"
        << "S:" << avgSeq / 100 << "\n"
        << "P:" << avgPar / 100 << std::endl;
}