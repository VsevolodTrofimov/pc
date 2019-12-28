#include <iostream>
#include <unistd.h>

inline double f(double x)
{
    return 4.0 / (1 + x * x);
}

double run(const double &from, const double &to, const int &steps, const double &step)
{
    double res = 0;

    for (int i = 0; i < steps; ++i)
    {
        res += f(from + step * i) * step;
    }

    return res;
}

double runP(const double &from, const double &to, const int &steps, const double &step)
{
    double res = 0;
#pragma omp parallel
    {
        double tres = 0;

#pragma omp for schedule (static)
        for (int i = 0; i < steps; ++i)
        {

            tres += f(from + step * i) * step;
        }

#pragma omp critical
        {
            res += tres;
        }
    }

    return res;
}

double runPR(const double &from, const double &to, const int &steps, const double &step)
{
    double res = 0;

#pragma omp parallel for shared(from, steps, step) reduction(+: res)
    for (auto i = 0; i < steps; ++i)
    {

        res += f(from + step * i) * step;
    }

    return res;
}

int main()
{
    const double from = 0;
    const double to = 1;
    const int steps = 1e9;
    const double step = (to - from) / steps;
    clock_t begin, end;
    double res, time_spent;

    begin = clock();
    res = runP(from, to, steps, step);
    end = clock();
    time_spent = (double)(end - begin);
    std::cout << "P\tres=" << res << "\tt=" << time_spent << " micro seconds" << std::endl;

    sleep(5);

    begin = clock();
    res = runPR(from, to, steps, step);
    end = clock();
    time_spent = (double)(end - begin);
    std::cout << "PR\tres=" << res << "\tt=" << time_spent << " micro seconds" << std::endl;

    sleep(5);

    begin = clock();
    res = run(from, to, steps, step);
    end = clock();

    time_spent = (double)(end - begin);
    std::cout << "S\tres=" << res << "\tt=" << time_spent << " micro seconds" << std::endl;
}