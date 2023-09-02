#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#define MAX 10000000

int monteCarlo(int ini, int fin, unsigned int *semilla)
{
    int puntosCirculo = 0;
    for (int i = ini; i < fin; i++)
    {
        double x = (double)rand_r(semilla) / RAND_MAX;
        double y = (double)rand_r(semilla) / RAND_MAX;
        if (x * x + y * y <= 1)
        {
            puntosCirculo++;
        }
    }
    return puntosCirculo;
}

int main() {

    double inicio, duracion;
    unsigned int semilla = time(NULL);

    inicio = omp_get_wtime();
    double aproxPi = 4.0 * monteCarlo(0, MAX, &semilla) / MAX;
    duracion = omp_get_wtime() - inicio;

    std::cout << "Estimacion de Pi en secuencial : " << aproxPi << std::endl;
    std::cout << "La duracion es = " << duracion << " segundos" << std::endl;

    int bloque = MAX / 10;
    int sumas[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    inicio = omp_get_wtime();
#pragma omp parallel num_threads(10)
    {
        int hilo = omp_get_thread_num();
        unsigned int semillaLocal = semilla + hilo;
        sumas[hilo] = monteCarlo(hilo * bloque, (hilo + 1) * bloque, &semillaLocal);
    }
    double total = 0.0;
    for (int i = 0; i < 10; i++)
    {
        total += sumas[i];
    }
    aproxPi = 4.0 * total / MAX;
    duracion = omp_get_wtime() - inicio;

    std::cout << "Estimacion de Pi en paralelo: " << aproxPi << std::endl;
    std::cout << "La duracion es = " << duracion << " segundos" << std::endl;
    return 0;
}
