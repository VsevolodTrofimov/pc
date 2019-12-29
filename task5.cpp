#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <omp.h>

std::string randomString(const int &size, const int &seed)
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

std::string randomSubstr(const std::string &s, const int &seed)
{
    srand(seed);
    const auto from = rand() % s.length();
    const auto to = rand() % (s.length() - from) + from;

    return s.substr(from, to);
}

// -1 did match
// -2 impossible
// otherwise position of next possible start
int maybeMatch(const std::string &s, const std::string &t, int pos)
{

    const auto tEnd = t.length() + pos;
    if (tEnd > s.length())
    {
        return -2;
    }

    auto lastStart = pos;
    for (auto i = pos; i < tEnd; ++i)
    {
        if (s[i] == t[0])
        {
            lastStart = i;
        }

        if (s[i] != t[i - pos])
        {
            return lastStart;
        }
    }

    return -1;
}

int search(const std::string &s, const std::string &t)
{
    const int end = s.length();
    for (auto i = 0; i < end; ++i)
    {
        if (s[i] == t[0])
        {
            const auto out = maybeMatch(s, t, i);
            if (out == -2)
            {
                break;
            }

            if (out == -1)
            {
                return i;
            }

            if (out != i)
            {
                i = out - 1;
            }
        }
    }

    return -1;
}

int searchP(const std::string &s, const std::string &t)
{
    auto res = -1;
    const int end = s.length();

#pragma omp parallel
    {
#pragma omp for
        for (auto i = 0; i < end; ++i)
        {
            if (s[i] == t[0])
            {
                const auto out = maybeMatch(s, t, i);
                if (out == -1)
                {
#pragma omp critical
                    {
                        if (res == -1 || res < i)
                        {
                            res = i;
                        }
                    }
                }
            }
        }
    }

    return res;
}

int main()
{
    const auto s = randomString(18, 2);
    const auto t = randomSubstr(s, 2);

    std::cout << "In \"" << s << "\" find \"" << t << "\"" << std::endl;

    const auto res = search(s, t);
    std::cout << res << std::endl;

    const auto resP = searchP(s, t);
    std::cout << resP << std::endl;
}