#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <ctime>

std::string randomString(const int& size, const int& seed)
{

    std::string result;

    const std::string VALID_CHARS = "a b c d e f ghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-";
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, VALID_CHARS.size() - 1);

    std::generate_n(std::back_inserter(result), size, [&]() {
        return VALID_CHARS[distribution(generator)];
        });

    return result;
}

std::string randomSubstr(const std::string& s, const int& seed)
{
    srand(seed);
    const auto from = rand() % s.length();
    const auto to = rand() % (s.length() - from) + from;

    return s.substr(from, to);
}

inline bool maybeMatch(const std::string& s, const std::string& t, int pos)
{

    const auto tEnd = t.length() + pos;
    if (tEnd > s.length())
    {
        return false;
    }

    for (auto i = pos; i < tEnd; ++i)
    {
        if (s[i] != t[i - pos])
        {
            return false;
        }
    }

    return true;
}

std::vector<int> search(const std::string& s, const std::string& t)
{
    auto results = std::vector<int>();
    const int end = s.length();

    for (auto i = 0; i < end; ++i)
    {
        if (s[i] == t[0])
        {
            const auto out = maybeMatch(s, t, i);
            if (out == -1)
            {
                results.push_back(i);
            }
        }
    }

    return results;
}

std::vector<int> searchP(const std::string& s, const std::string& t)
{
    auto results = std::vector<int>();
    const int end = s.length();

#pragma omp parallel for
    for (auto i = 0; i < end; ++i)
    {
        if (s[i] == t[0])
        {
            const auto out = maybeMatch(s, t, i);
            if (out == -1)
            {
#pragma omp critical
                results.push_back(i);
            }
        }
    }

    return results;
}

int task5()
{
    srand(123213);
    const auto seed = rand();
    clock_t begin, end;

    double tSeq = 0;
    double tPar = 0;
    std::vector<int> res;
    for (auto i = 0; i < 20; ++i)
    {
        const int iterSeed = rand();
        std::cout << "\r" << i + 1 << std::flush;
        const auto s = randomString((rand() % 40000000) + 4 * 1e7, iterSeed);
        const auto t = randomSubstr(s, iterSeed);

        begin = clock();
        for (auto r = 0; r < 5; ++r)
        {
            res = searchP(s, t);
        }
        end = clock();
        tPar += (double)(end - begin);

        begin = clock();
        for (auto r = 0; r < 5; ++r)
        {
            res = search(s, t);
        }
        end = clock();
        tSeq += (double)(end - begin);
    }

    std::cout << "\r"
        << "S:" << tSeq / 1000 << "\n"
        << "P:" << tPar / 1000 << std::endl;
}