//Realiza la multiplicación de dos matrices cuadradas grandes(por ejemplo, 1000x1000)
//Tanto de manera secuencial como en paralelo utilizando OpenMP
//Divide el trabajo en filas o bloques y mide el tiempo de ejecución en ambos casos
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Se define las dimensiones de la matriz y sus dimensiones en el heap
#define m 1000
int axa[m][m];
int bxb[m][m];
int res[m][m];

// Funcion para llenar una matriz con numeros aleatorios
void llenarMatrizAleatoria(int matriz[][m], int filas, int columnas)
{
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			matriz[i][j] = rand();
		}
	}
}

// Funcion para imprimir una parte de la matriz
void imprimirMatriz(int matriz[][m], int filas, int columnas)
{
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			printf("%d\t", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Funcion para multiplicar matrices secuencialmente
void multiplicarMatrices(int matriz1[][m], int matriz2[][m], int resultado[][m], int filas, int columnas)
{
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			resultado[i][j] = 0;
			for (int k = 0; k < columnas; k++)
			{
				resultado[i][j] += matriz1[i][k] * matriz2[k][j];
			}
		}
	}
}

// Funcion para multiplicar matrices paralelamente
void multiplicarMatricesParalela(int matriz1[][m], int matriz2[][m], int resultado[][m], int filas, int columnas, int i, int j)
{
	for (i; i < filas; i += 10)
	{
		for (j; j < columnas; j += 10)
		{
			resultado[i][j] = 0;
			for (int k = 0; k < columnas; k++)
			{
				resultado[i][j] += matriz1[i][k] * matriz2[k][j];
			}
		}
	}
}

int main()
{
	srand(time(NULL)); // Inicializar la semilla para números aleatorios

	double inicio, duracion;

	llenarMatrizAleatoria(axa, m, m);
	llenarMatrizAleatoria(bxb, m, m);

	inicio = omp_get_wtime();
	multiplicarMatrices(axa, bxb, res, m, m);
	duracion = omp_get_wtime() - inicio;

	printf("\nMultiplicacion secuencial:\t%lf\n", duracion);
	imprimirMatriz(res, 3, 3);

	inicio = omp_get_wtime();
#pragma omp parallel num_threads(10)
	{
		int hilo = omp_get_thread_num();
		multiplicarMatricesParalela(axa, bxb, res, m, m, hilo, hilo);

	}
	duracion = omp_get_wtime() - inicio;



	printf("\nMultiplicacion paralela:\t%lf\n", duracion);
	imprimirMatriz(res, 3, 3);
	return 0;
}