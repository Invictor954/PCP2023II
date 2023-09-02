/*
*   Multiplicacion de matrices cuadradas secuencial simple, secuencial optimizada y paralela
*   Multiplica las matrices MatrixA(m,n) y MatrixB(n,k) y almacena el resultado en MatrixC(m,k)
*   donde m, n y k seran iguales debido a que para el ejercicio se usara matrices cuadradas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE  1000
#define NUM_THREADS 10
    
typedef struct matrix
{
    int rows,cols;
    int size;
    int *data;
} matrix;

void initMatrix(matrix *mat, int col, int row)
{
    mat->rows=row;
    mat->cols=col;
    mat->size = col*row;
    mat->data = (int*) malloc(mat->size*sizeof(int));
}

void fillMatrix(matrix *mat, int val)
{
    for(int i=0; i<mat->size; i++)
    {
        mat->data[i]=val;
    }
}

void fillMatrixRandom(matrix *mat)
{
    for(int i=0; i<mat->size; i++)
    {
        mat->data[i]=rand()%10;
    }
}


void show(matrix *mat)
{
    printf("Rows: %i\n",mat->rows);
    printf("Cols: %i\n",mat->cols);
    printf("Size: %i\n",mat->size);
    for(int i=0; i<mat->rows; i++)
    {
        printf("[");
        for(int j=0; j<mat->cols; j++)
        {
            printf("  %i  ", mat->data[i*mat->rows+j]);
        }
        printf("]\n");
    }
}

void multiplicar_secuencial_simple(matrix *matA, matrix *matB, matrix *matC)
{
    for(int i=0; i<matC->rows; i++)
    {
        for(int j=0; j<matC->cols; j++)
        {
            for(int idx=0; idx<matA->rows; idx++)
            {
                matC->data[i*matC->cols + j] += matA->data[i*matA->cols + idx] * matB->data[idx*matB->cols + j];
            }
        }
    }
}

void ComputeTile(matrix *matA, matrix *matB, matrix *matC, int x, int y, const int TILE)
{
    for (int i = x; i < x + TILE; ++i)
    {
        for (int j = y; j < y + TILE; ++j)
        {
            for (int idx = 0; idx < matA->rows; ++idx)
            {
                matC->data[i*matC->cols + j] += matA->data[i*matA->cols + idx] * matB->data[idx*matB->cols + j];
            }
        }
    }
}

void multiplicar_paralelo_tiling(matrix *matA, matrix *matB, matrix *matC)
{
    const int TILE = SIZE/NUM_THREADS;
    #pragma omp parallel for num_threads(NUM_THREADS) collapse(2)
        for (int i = 0; i < matC->rows; i += TILE)
        {
            for (int j = 0; j < matC->cols; j += TILE)
            {
                ComputeTile(matA, matB, matC, i, j, TILE);   
            }
        }
}

int main()
{
    srand(time(NULL));
    double inicio, duracion;

    matrix *matA = malloc (sizeof(matrix));
    initMatrix(matA, SIZE, SIZE);
    fillMatrixRandom(matA);
    printf("Matriz A lista \n");

    matrix *matB = malloc (sizeof(matrix));
    initMatrix(matB, SIZE, SIZE);
    fillMatrixRandom(matB);
    printf("Matriz B lista \n");

    matrix *matC = malloc (sizeof(matrix));
    initMatrix(matC, matA->rows, matB->cols);
    fillMatrix(matC, 0);
    printf("Matriz C lista \n");

    /** Evaluacion secuencial **/
    
    inicio = omp_get_wtime();
    multiplicar_secuencial_simple(matA, matB, matC);
	duracion = omp_get_wtime() - inicio;
	
	printf("Matriz C \n");
    if(SIZE<=10)
    {
        show(matC);
    }
    printf( "Duracion del producto secuencial = %lf\n", duracion );
    
    printf("**************************\n");
    
    /** Evaluacion paralela **/
    /* Rellenar matriz c para volver a calcular con metodo optimizado */
    fillMatrix(matC, 0);
    
    inicio = omp_get_wtime();
    multiplicar_paralelo_tiling(matA, matB, matC);
	duracion = omp_get_wtime() - inicio;
    
    printf("Matriz C \n");
    if(SIZE<=10)
    {
        show(matC);
    }
    printf( "Duracion del producto en paralelo = %lf\n", duracion );
    
    printf("**************************\n");

    /* Dealocar memoria */
    free(matA->data);
    free(matB->data);
    free(matC->data);
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
