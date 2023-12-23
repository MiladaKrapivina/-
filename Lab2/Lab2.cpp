#include <iostream>
#include <omp.h>
#include <chrono>


using namespace std;

const int N = 1000; // количество шагов
const double h = 0.01; // размер шага
const int threads = 5;

// Функция, описывающая систему дифференциальных уравнений
void f(double t, const double* y, double* dy)
{
    dy[0] = y[1];
    dy[1] = -y[0];
}

// Функция для вычисления шага методом Рунге-Кутта
void step(double t, double* y, double* dy)
{
    double k1[2], k2[2], k3[2], k4[2];

    f(t, y, k1);

#pragma omp parallel for num_threads(threads) 
    for (int i = 0; i < 2; ++i)
    {
        double temp[2];
        temp[0] = y[0] + k1[0] / 2;
        temp[1] = y[1] + k1[1] / 2;
        f(t + h / 2, temp, k2);
    }

#pragma omp parallel for num_threads(threads) 
    for (int i = 0; i < 2; ++i)
    {
        double temp[2];
        temp[0] = y[0] + k2[0] / 2;
        temp[1] = y[1] + k2[1] / 2;
        f(t + h / 2, temp, k3);
    }

#pragma omp parallel for num_threads(threads) 
    for (int i = 0; i < 2; ++i)
    {
        double temp[2];
        temp[0] = y[0] + k3[0];
        temp[1] = y[1] + k3[1];
        f(t + h, temp, k4);
    }

#pragma omp parallel for num_threads(threads) 
    for (int i = 0; i < 2; ++i)
    {
        dy[i] = (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) * h / 6;
    }
}

int main()
{

    auto beginp = std::chrono::steady_clock::now();  //начало отсчета
    double t = 0;
    double y[2] = { 0, 1 };

    for (int i = 0; i < N; ++i)
    {
        double dy[2];

        step(t, y, dy);

    #pragma omp parallel for 
        for (int j = 0; j < 2; ++j)
        {
            y[j] += dy[j];
        }

        t += h;
    }

    cout << "Solution: y[0] = " << y[0] << ", y[1] = " << y[1] << endl;
    auto endp = std::chrono::steady_clock::now(); // конец отсчета
    auto elapsed_msp = std::chrono::duration_cast<std::chrono::milliseconds>(endp - beginp);// подсчет времени выполнения фрагмента программы
    std::cout << "Therads cout: " << threads << "\n";
    std::cout << "The time: " << elapsed_msp.count() << " ms\n";

    return 0;
}
