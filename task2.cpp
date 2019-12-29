#include <iostream>
#include <omp.h>

inline void fill(int *M, const size_t &size, const int &seed)
{
    srand(seed);

    for (auto i = 0; i < size; ++i)
    {
        M[i] = rand() % 100;
    }
}

inline void print2d(const int *M, const size_t &h, const size_t &w)
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

inline int multiplyP(int *A, int *B, int *C, const size_t &ah, const size_t &aw, const size_t &bh, const size_t &bw)
{
    const int cw = bw;
    const int ch = ah;
    const int size = ch * cw;

    if (aw != bh)
    {
        throw "Matrix dimensions must agree";
    }

#pragma omp parallel for shared(C) schedule(static)
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

inline int multiply(int *A, int *B, int *C, const size_t &ah, const size_t &aw, const size_t &bh, const size_t &bw)
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

inline bool equals(const int *A, const int *B, const size_t &size)
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

int main()
{
    const int h = 20000;
    const int m = 100;
    const int w = 6000;

    srand(999);
    const auto seed = rand();
    clock_t begin, end;

    double avgSeq = 0;
    double avgPar = 0;
    int res = 0;
    for (auto i = 0; i < 100; ++i)
    {
        std::cout << "\r" << i + 1 << std::flush;

        const int h = rand() % 300 + 300;
        const int m = rand() % 300 + 300;
        const int w = rand() % 300 + 300;

        const int ah = h;
        const int aw = m;
        const int bh = m;
        const int bw = w;

        auto A = new int[ah * aw];
        auto B = new int[bh * bw];
        auto C = new int[h * w];
        fill(A, ah * aw, rand());
        fill(B, bh * bw, rand());

        begin = clock();
        res = multiplyP(A, B, C, ah, aw, bh, bw);
        end = clock();
        avgPar += (double)(end - begin);

        begin = clock();
        res = multiply(A, B, C, ah, aw, bh, bw);
        end = clock();
        avgSeq += (double)(end - begin);
    }

    std::cout << "\r"
              << "S:" << avgSeq / 100 << "\n"
              << "P:" << avgPar / 100 << std::endl;
}