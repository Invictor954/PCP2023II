//---------------------------Ejercicio01.cpp :----------------------------------------
//Realiza la multiplicacion de dos matrices cuadradas grandes(por ejemplo, 1000x1000)
//Tanto de manera secuencial como en paralelo utilizando OpenMP
//Divide el trabajo en filas o bloques y mide el tiempo de ejecucion en ambos casos
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

typedef std::vector<std::vector<int>> Matrix;

void printMatrix(Matrix mxm, int lim);

void fillMatrix(Matrix& mxm);

Matrix multiplyMatrix(Matrix axa, Matrix bxb);

Matrix multiplyMatrixParallel(Matrix axa, Matrix bxb);

int main()
{
	const int row{ 30 };
	const int col{ 30 };

	Matrix mxm(row, std::vector<int>(col));
	fillMatrix(mxm);
	Matrix axa{ mxm };
	Matrix nxn(row, std::vector<int>(col));
	fillMatrix(nxn);
	Matrix bxb{ nxn };
	Matrix rxr;
	Matrix cxc;
	std::cout << "\t\tsize : " << mxm.size() << "x" << mxm[0].size() << "\n\n";
	
	//sequentially
	auto start = std::chrono::high_resolution_clock::now();
	rxr = multiplyMatrix(mxm, nxn);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;
	printMatrix(rxr, 2);
	std::cout << "time : " << elapsed_time.count() << " s\n\n\n\n";

	//parallel
	start = std::chrono::high_resolution_clock::now();
	cxc = multiplyMatrixParallel(axa, bxb);
	end = std::chrono::high_resolution_clock::now();
	elapsed_time = end - start;
	printMatrix(cxc, 2);
	std::cout << "time : " << elapsed_time.count() << " s\n";

	return 0;
}

void fillMatrix(Matrix& mxm)
{
	std::srand(std::time(nullptr));
	for (int i{ 0 }; i < mxm.size(); i++)
	{
		for (int j{ 0 }; j < mxm[0].size(); j++)
		{
			mxm[i][j] = std::rand();
		}
	}
}

Matrix multiplyMatrix(Matrix axa, Matrix bxb)
{
	Matrix cxc(axa.size(), std::vector<int>(bxb[0].size(), 0));
	for (int i{ 0 }; i < axa.size(); i++)
	{
		for (int j{ 0 }; j < bxb[0].size(); j++)
		{
			for (int k = 0; k < axa[0].size(); k++)
			{
				cxc[i][j] += axa[i][k] * bxb[k][j];
			}
		}
	}
	return cxc;
}

Matrix multiplyMatrixParallel(Matrix axa, Matrix bxb)
{
	Matrix cxc(axa.size(), std::vector<int>(bxb[0].size(), 0));
#pragma omp parallel num_threads(omp_get_max_threads())
	{
		int num_threads = omp_get_num_threads();
		int thread_id = omp_get_thread_num();
		int block = axa.size() / num_threads;

		for (int i = thread_id; i < (thread_id + 1) * block; i++)
		{
			for (int j = 0; j < cxc[0].size(); j++)
			{
				for (int k = 0; k < axa[0].size(); k++)
				{
					cxc[i][j] += axa[i][k] * bxb[k][j];
				}
			}
		}
	}
	return cxc;
}

void printMatrix(Matrix mxm, int lim)
{
	for (int i{ 0 }; i < lim; i++)
	{
		for (int j{ 0 }; j < lim; j++)
		{
			std::cout << mxm[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}
