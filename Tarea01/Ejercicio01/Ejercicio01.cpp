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
//Donwload Eigen to use this library from :
//											http://eigen.tuxfamily.org/index.php?title=Main_Page#Download
//
#include <Eigen/Dense>

typedef std::vector<std::vector<int>> Matrix;

void printMatrix(const Matrix &mxm, int lim);

void fillMatrix(Matrix &mxm);

inline Matrix multiplyMatrix(const Matrix &axa, const Matrix &bxb);

inline Matrix multiplyMatrixParallel(const Matrix &axa, const Matrix &bxb);

Matrix addMatrix(const Matrix &axa, const Matrix &bxb);

Matrix subMatrix(const Matrix &axa, const Matrix &bxb);

Matrix strassenMultiply(const Matrix &axa, const Matrix &bxb);



int main()
{
	const int row{ 1000 };
	const int col{ 1000 };

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
	std::cout << "Secuential time : \t" << elapsed_time.count() << " s\n\n\n\n";

	//parallel
	start = std::chrono::high_resolution_clock::now();
	cxc = multiplyMatrixParallel(axa, bxb);
	end = std::chrono::high_resolution_clock::now();
	elapsed_time = end - start;
	printMatrix(cxc, 2);
	std::cout << "Parallel time : \t" << elapsed_time.count() << " s\n\n\n\n";

	//Eigen
	
	Eigen::MatrixXd oxo(row, col);
	Eigen::MatrixXd pxp(row, col);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			oxo(i, j) = mxm[i][j];
			pxp(i, j) = nxn[i][j];
		}
	}
	
	Eigen::MatrixXd qxq(row, col);
	start = std::chrono::high_resolution_clock::now();
	qxq = oxo * pxp;
	end = std::chrono::high_resolution_clock::now();
	elapsed_time = end - start;
	std::cout << qxq.block(0, 0, 2, 2) << "\n\n";
	std::cout << "Eigen time : \t" << elapsed_time.count() << " s\n";

	//strassen
	//start = std::chrono::high_resolution_clock::now();
	//cxc = strassenMultiply(axa, bxb);
	//end = std::chrono::high_resolution_clock::now();
	//elapsed_time = end - start;
	//printMatrix(cxc, 2);
	//std::cout << "time : " << elapsed_time.count() << " s\n";

	return 0;
}

void fillMatrix(Matrix& mxm)
{
	std::srand(std::time(nullptr));
	for (int i{ 0 }; i < mxm.size(); i++)
	{
		for (int j{ 0 }; j < mxm[0].size(); j++)
		{
			mxm[i][j] = std::rand()% 100;
		}
	}
}

inline Matrix multiplyMatrix(const Matrix &axa, const Matrix &bxb)
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

inline Matrix multiplyMatrixParallel(const Matrix &axa, const Matrix &bxb)
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

Matrix addMatrix(const Matrix &axa, const Matrix &bxb)
{
	int n = axa.size();
	Matrix cxc(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cxc[i][j] = axa[i][j] + bxb[i][j];
		}
	}
	return cxc;
}

Matrix subMatrix(const Matrix& axa, const Matrix& bxb)
{
	int n = axa.size();
	Matrix cxc(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cxc[i][j] = axa[i][j] - bxb[i][j];
		}
	}
	return cxc;
}

Matrix strassenMultiply(const Matrix& axa, const Matrix& bxb)
{
	int n = axa.size();
	if (n == 1) {
		Matrix cxc(1, std::vector<int>(1, 0));
		cxc[0][0] = axa[0][0] * bxb[0][0];
		return cxc;
	}

	int halfSize = n / 2;

	Matrix A11(halfSize, std::vector<int>(halfSize, 0));
	Matrix A12(halfSize, std::vector<int>(halfSize, 0));
	Matrix A21(halfSize, std::vector<int>(halfSize, 0));
	Matrix A22(halfSize, std::vector<int>(halfSize, 0));

	Matrix B11(halfSize, std::vector<int>(halfSize, 0));
	Matrix B12(halfSize, std::vector<int>(halfSize, 0));
	Matrix B21(halfSize, std::vector<int>(halfSize, 0));
	Matrix B22(halfSize, std::vector<int>(halfSize, 0));

	for (int i = 0; i < halfSize; ++i) {
		for (int j = 0; j < halfSize; ++j) {
			A11[i][j] = axa[i][j];
			A12[i][j] = axa[i][j + halfSize];
			A21[i][j] = axa[i + halfSize][j];
			A22[i][j] = axa[i + halfSize][j + halfSize];

			B11[i][j] = bxb[i][j];
			B12[i][j] = bxb[i][j + halfSize];
			B21[i][j] = bxb[i + halfSize][j];
			B22[i][j] = bxb[i + halfSize][j + halfSize];
		}
	}

	Matrix P1 = strassenMultiply(A11, subMatrix(B12, B22));
	Matrix P2 = strassenMultiply(addMatrix(A11, A12), B22);
	Matrix P3 = strassenMultiply(addMatrix(A21, A22), B11);
	Matrix P4 = strassenMultiply(A22, subMatrix(B21, B11));
	Matrix P5 = strassenMultiply(addMatrix(A11, A22), addMatrix(B11, B22));
	Matrix P6 = strassenMultiply(subMatrix(A12, A22), addMatrix(B21, B22));
	Matrix P7 = strassenMultiply(subMatrix(A11, A21), addMatrix(B11, B12));

	Matrix C11 = subMatrix(addMatrix(addMatrix(P5, P4), P6), P2);
	Matrix C12 = addMatrix(P1, P2);
	Matrix C21 = addMatrix(P3, P4);
	Matrix C22 = subMatrix(subMatrix(addMatrix(P5, P1), P3), P7);

	Matrix cxc(n, std::vector<int>(n, 0));
	for (int i = 0; i < halfSize; ++i) {
		for (int j = 0; j < halfSize; ++j) {
			cxc[i][j] = C11[i][j];
			cxc[i][j + halfSize] = C12[i][j];
			cxc[i + halfSize][j] = C21[i][j];
			cxc[i + halfSize][j + halfSize] = C22[i][j];
		}
	}

	return cxc;
}

void printMatrix(const Matrix &mxm, int lim)
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
