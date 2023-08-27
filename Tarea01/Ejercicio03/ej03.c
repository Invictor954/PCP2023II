//Desarrolla un algoritmo de ordenamiento(como Quicksort o Mergesort)
//Y aplicalo a un arreglo grande de números.
//Implementa versiones secuenciales y paralelas para comparar el rendimiento.
//Puedes paralelizar la subdivision de las particiones y el proceso de mezcla.

#include <stdio.h>
#include <time.h>
#include <omp.h>

#define MAX 10000000
int vector[MAX];

void imprimirVector(int vector[MAX], int limite)
{
	for (int i = 0; i < limite; i++)
	{
		printf("%i\t", vector[i]);
	}
	printf("\n");
}

void llenarVector(int vector[MAX])
{
	for (int i = 0; i < MAX; i++)
	{
		vector[i] = i + 1;
	}
}

void desordenar(int vector[MAX])
{
	for (int i = MAX - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int temp = vector[i];
		vector[i] = vector[j];
		vector[j] = temp;
	}
}

void intercambio(int* a, int* b)
{
	int auxiliar = *a;
	*a = *b;
	*b = auxiliar;
}

int dividir(int vector[MAX], int ini, int fin)
{
	int pivote = vector[fin];
	int i = ini - 1;

	for (int j = ini; j <= fin - 1; j++)
	{
		if (vector[j] < pivote)
		{
			i++;
			intercambio(&vector[i], &vector[j]);
		}
	}
	intercambio(&vector[i + 1], &vector[fin]);
	return i + 1;
}

void quicksort(int vector[MAX], int ini, int fin)
{
	if (ini < fin)
	{
		int pivote = dividir(vector, ini, fin);

		quicksort(vector, ini, pivote - 1);
		quicksort(vector, pivote + 1, fin);
	}
}

void quicksortParalela(int vector[MAX], int ini, int fin)
{
	if (ini < fin)
	{
		int pivote = dividir(vector, ini, fin);

#pragma omp parallel num_threads(1)
		{
			int hilo = omp_get_thread_num();
			if (hilo == 0) {
				quicksortParalela(vector, ini, pivote - 1);
			}
			else {
				quicksortParalela(vector, pivote + 1, fin);
			}
		}
	}
}


int main()
{
	srand(time(NULL));
	double inicio, duracion;

	inicio = omp_get_wtime();


	llenarVector(vector);

	desordenar(vector);

	imprimirVector(vector, 10);

	inicio = omp_get_wtime();
	quicksort(vector, 0, MAX - 1);
	duracion = omp_get_wtime() - inicio;
	imprimirVector(vector, 10);
	printf("\nquicksort secuencial:%lf segundos\n", duracion);
	//Se desordena el vector nuevamente para que pase por el metodo paralelo
	desordenar(vector);
	inicio = omp_get_wtime();
	quicksortParalela(vector, 0, MAX - 1);
	quicksortParalela(vector, 0, MAX - 1);
	quicksortParalela(vector, 0, MAX - 1);
	duracion = omp_get_wtime() - inicio;
	//Imprimir los cantidad de elementos deseados del vector depsues de usar el metodo paralelo
	imprimirVector(vector, 10);
	printf("quicksort paralelo:%lf segundos\n", duracion);

	return 0;
}