#include <iostream>

inline void fill(int *M, const size_t &size, const int &seed)
{
    srand(seed);

    for (auto i = 0; i < size; ++i)
    {
        M[i] = rand() % 1000000;
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

inline int minmaxP(int *M, const size_t &h, const size_t &w)
{
    int max = 0;

#pragma omp parallel
    {
#pragma omp for
        for (auto y = 0; y < h; ++y)
        {
            int strMin = INT32_MAX;
            for (auto x = 0; x < w; ++x)
            {
                const int val = M[w * y + x];
                if (val < strMin)
                {
                    strMin = val;
                }
            }
#pragma omp critical
            {
                if (strMin > max)
                {
                    max = strMin;
                }
            }
        }
    }

    return max;
}

inline int minmax(int *M, const size_t &h, const size_t &w)
{

    int max = 0;

    for (auto y = 0; y < h; ++y)
    {
        int strMin = INT32_MAX;
        for (auto x = 0; x < w; ++x)
        {
            const int val = M[w * y + x];
            if (val < strMin)
            {
                strMin = val;
            }
        }

        {
            if (strMin > max)
            {
                max = strMin;
            }
        }
    }

    return max;
}

int main()
{
    const int h = 5000;
    const int w = 3000;

    auto M = new int[h * w];
    fill(M, h * w, 40);

    std::cout << minmax(M, h, w) << "|" << minmaxP(M, h, w) << std::endl;

    delete M;
}