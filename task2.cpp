#include <iostream>
#include <array>

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

inline int multiply(int *A, int *B, int *C, const size_t &ah, const size_t &aw, const size_t &bh, const size_t &bw)
{
    const int cw = bw;
    const int ch = ah;

    if (aw != bh)
    {
        throw "Matrix dimensions must agree";
    }

    for (auto y = 0; y < ah; ++y)
    {
        for (auto x = 0; x < bw; ++x)
        {
            int sum = 0;
            for (auto i = 0; i < bh; ++i)
            {
                sum += A[aw * y + i] * B[bw * i + x];
            }
            C[y * cw + x] = sum;
        }
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

    const int ah = h;
    const int aw = m;
    const int bh = m;
    const int bw = w;

    auto A = new int[ah * aw];
    auto B = new int[bh * bw];
    fill(A, ah * aw, 40);
    fill(B, bh * bw, 20);

    auto C = new int[h * w];
    auto CP = new int[h * w];
    multiply(A, B, C, ah, aw, bh, bw);
    multiplyP(A, B, CP, ah, aw, bh, bw);

    std::cout << equals(C, CP, h * w);

    delete A, B, C, CP;
}