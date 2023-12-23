#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <chrono>

using namespace std;
// размер матриц
#define N 20 //50
#define M 40 //400
int t, ptime;

//заполнение матрицы случайными значениями
void matrixFilling(double (matrix)[N][M] ) {
    for (int i = 0; i<N;i ++ )
        for (int j = 0; j <M; j++)
        matrix[i][j] = ((double)(rand()) /RAND_MAX)*100;
}

//вывод значений матриц в консоли
void output(double(matrix)[N][M]) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << " \t " << matrix[i][j]<< " \t ";
        }
        cout << "\n";
    }   
    cout << "\n";
}

// суммирование мариц
void mmatrixSum(double(a)[N][M], double(b)[N][M], double(c)[N][M]) {
   
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
   // printf("%dth row is counting \n", i);
    }

}
// распараллеливание суммирования мариц
void mmatrixParallSum(double(a)[N][M], double(b)[N][M], double(c)[N][M]) {
   
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {   
            c[i][j] = a[i][j] + b[i][j];
        } 
    //printf("%dth row is counting by %dth thread\n", i, omp_get_thread_num());
    //printf("%dth row is counting \n", i);
    }

}

int main()
{
    double A[N][M];
    double B[N][M];
    double C[N][M];
    double D[N][M];

    matrixFilling(A);
   // output(A);
    matrixFilling(B);
   // output(B);
    mmatrixParallSum (A, B, C);
    
    // Последовательное суммирование

        
    auto begin = std::chrono::steady_clock::now();//начало отсчета
    mmatrixSum(A, B, C);    
    auto end = std::chrono::steady_clock::now();// конец отсчета
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);// подсчет времени выполнения фрагмента программы
    cout << "The time: " << elapsed_ms.count() << " ms\n";
    
    

    // Параллельное суммирование
  
    auto beginp = std::chrono::steady_clock::now();  //начало отсчета
    
    mmatrixParallSum(A, B, D);

    auto endp = std::chrono::steady_clock::now(); // конец отсчета
    auto elapsed_msp = std::chrono::duration_cast<std::chrono::milliseconds>(endp - beginp);// подсчет времени выполнения фрагмента программы
    std::cout << "The ptime: " << elapsed_msp.count() << " ms\n";

    cout << "Res: \n";
   // output(C); 
   
}
