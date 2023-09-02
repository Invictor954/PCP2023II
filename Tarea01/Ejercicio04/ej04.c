#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define MAX 100000000
#define NUM_THREADS 10

int GenerarAleatorios(int cantidad, int seedmultiply)
{   
    /* Multiplicamos el seed por un factor para garantizar que cada hilos obtenga un set distinto de numeros aleatorios */
    srand(time(NULL)*seedmultiply);
    int puntosCirculo = 0;
    double x,y,z;
    for (int i = 0; i < cantidad; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        z = x * x + y * y;
        if (z <= 1)
        {
            puntosCirculo++;
        }
    }
    return puntosCirculo;
}

int main() {

    double inicio, duracion;

    /** Evaluacion secuencial **/
    inicio = omp_get_wtime();
    double PI = 4.0 * GenerarAleatorios(MAX,1) / MAX;
    duracion = omp_get_wtime() - inicio;

    printf("Valor de pi calculado: %f \n", PI);
    printf("Resultado de la simulacion secuencial: %f \n", duracion);

    /** Evaluacion paralela **/
    double total=0;
    double sumas[NUM_THREADS];
    
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int hilo = omp_get_thread_num();
        sumas[hilo] = GenerarAleatorios(MAX/NUM_THREADS, hilo);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        total += sumas[i];
    }
    PI = 4.0 * total / MAX;
    duracion = omp_get_wtime() - inicio;
    
    printf("Valor de pi calculado: %f \n", PI);
    printf("Resultado de la simulacion paralela: %f \n", duracion);


    return 0;
}
