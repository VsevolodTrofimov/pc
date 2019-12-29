#include <iostream>
#include <array>

template <
    std::size_t H,
    std::size_t W>
std::array<std::array<int, W>, H> generate(H h, W w, int seed)
{
    srand(seed);
    auto M = new std::array<std::array<int, W>, H>();

    for (auto y = 0; y < h; ++y)
    {
        for (auto x = 0; x < w; ++x)
        {
            M[y][x] = rand()
        }
    }

    return M;
}

template <
    std::size_t H,
    std::size_t W>
void print2d(H h, W w, std::array<std::array<int, W>, H> M)
{
    for (auto y = 0; y < h; ++y)
    {
        for (auto x = 0; x < w; ++x)
        {
            cout << M[y][x] << "\t";
        }
        cout << endl;
    }

    return M;
}