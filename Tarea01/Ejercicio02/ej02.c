//Implementa un algoritmo para encontrar todos los números primos en un rango grande
//(por ejemplo, 1 a 10, 000, 000) utilizando la criba de Eratóstenes.
//Divide el rango en intervalos y utiliza múltiples hilos para buscar números primos en paralelo
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>

#define MAX 10000000
// Crear vector y llenarlo
bool primo[MAX];
//funciones

void cribaEratostenes(bool vector[MAX])
{
	for (int i = 2; i < MAX; i++)
	{
		for (int j = 2; i * j < MAX; j++)
		{
			vector[i * j] = false;
		}
	}
}

void cribaEratostenesParalela(bool vector[MAX], int ini, int fin)
{
	for (int i = ini + 2; i < fin; i++)
	{
		for (int j = 2; i * j < MAX; j++)
		{
			vector[i * j] = false;
		}
	}
}

void llenarVector(bool vector[MAX])
{
	for (int i = 0; i < MAX; i++)
	{
		vector[i] = true;
	}
}

void imprimirVector(bool vector[MAX], int limite)
{
	for (int i = 2; i < limite; i++)
	{
		if (vector[i])
		{
			printf("%i\t", i);
		}
	}
}

int main()
{
	double inicio, duracion;

	llenarVector(primo);
	inicio = omp_get_wtime();
	cribaEratostenes(primo);
	duracion = omp_get_wtime() - inicio;

	imprimirVector(primo, 1000);

	printf("\nCriba secuencial:\t%lf\n\n", duracion);

	llenarVector(primo);
	inicio = omp_get_wtime();
	int bloque = MAX / 10;
#pragma omp parallel num_threads(10)
	{
		int hilo = omp_get_thread_num();
		cribaEratostenesParalela(primo, hilo * bloque, (hilo + 1) * bloque);

	}
	duracion = omp_get_wtime() - inicio;
	imprimirVector(primo, 1000);
	printf("\nCriba paralela:\t%lf", duracion);

	return 0;
}