//Realiza una simulacion de Monte Carlo para estimar el area de un circulo
//Utilizando puntos aleatorios dentro de un cuadrado.
//Calcula el valor de ? basandote en el area estimada.
//Implementa una version secuencial y paralela utilizando multiples hilos para generar puntos aleatorios.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX 100000000

int monteCarlo(int ini, int fin)
{
    srand(time(NULL));

    int puntosCirculo = 0;
    for (int i = ini; i < fin; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1)
        {
            puntosCirculo++;
        }
    }
    return puntosCirculo;
}

int main() {

    double inicio, duracion;

    inicio = omp_get_wtime();
    double aproxPi = 4.0 * monteCarlo(0, MAX) / MAX;
    duracion = omp_get_wtime() - inicio;

    printf("Estimacion de Pi: %lf\n", aproxPi);
    printf("La duracion es = %lf segundos\n", duracion);

    int bloque = MAX / 10;
    int sumas[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    inicio = omp_get_wtime();
#pragma omp parallel num_threads(10)
    {
        int hilo = omp_get_thread_num();
        sumas[hilo] = monteCarlo(hilo * bloque, (hilo + 1) * bloque);

    }
    double total = 0.0;
    for (int i = 0; i < 10; i++)
    {
        total += sumas[i];
    }
    aproxPi = 4.0 * total / MAX;
    duracion = omp_get_wtime() - inicio;

    printf("Estimacion de Pi: %lf\n", aproxPi);
    printf("La duracion es = %lf segundos\n", duracion);
    return 0;
}