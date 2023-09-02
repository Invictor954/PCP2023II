#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <math.h>

int SIZE = 100000000;
int NUM_THREADS = 10;

void initArray(bool *arr){
    for(int i=0; i<SIZE; i++)
    {
        arr[i]=1;
    }
}

void show(bool *arr)
{
    /* Valor 1 indica que es primo, valor 0 indica que no es primo */
    printf("Array: \n[");
    for(int i=0; i<SIZE; i++)
    {
        if(arr[i]==1){
            printf(" %i ", i);
        }
    }
    printf("]\n");
}

void sieve(bool *arr, int inicio, int fin)
{
    arr[0]=0;
    arr[1]=0;
    int n = (int) sqrt(fin);
	{
        for(int i=inicio; i<=n; i++)
        {
            if(arr[i]==1)
            {
                for(int j=i*i; j<SIZE; j+=i)
                {
                    arr[j]=0;
                }
            }
        }
	}
}
void sieve_par(bool *arr, int inicio, int fin)
{
    arr[0]=0;
    arr[1]=0;
    int n = (int) sqrt(fin);
    #pragma omp parallel num_threads(NUM_THREADS)
	{
        #pragma omp for schedule(dynamic)
        for(int i=inicio; i<=n; i++)
        {
            if(arr[i]==1)
            {
                for(int j=i*i; j<SIZE; j+=i)
                {
                    arr[j]=0;
                }
            }
        }
	}
}


int main()
{
    SIZE++;
    double inicio, duracion;
    bool *arr = (bool*)calloc(SIZE, sizeof(int));
    
    
    /*** Secuencial ***/
    initArray(arr);
    inicio = omp_get_wtime();
    sieve(arr, 0, SIZE);
    duracion = omp_get_wtime() - inicio;
    printf( "Duracion secuencial = %lf\n", duracion );
    
    //show(arr);
    
    
    /*** Paralelo ***/
    initArray(arr);
    inicio = omp_get_wtime();
    sieve_par(arr,0,SIZE);
	duracion = omp_get_wtime() - inicio;
    printf( "Duracion paralela = %lf\n", duracion );
	
	//show(arr);
	
    free(arr);
    return 0;
}

