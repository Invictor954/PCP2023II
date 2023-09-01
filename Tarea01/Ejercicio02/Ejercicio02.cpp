//Implementa un algoritmo para encontrar todos los numeros primos en un rango grande
//(por ejemplo, 1 a 10, 000, 000) utilizando la criba de Eratostenes.
//Divide el rango en intervalos y utiliza multiples hilos para buscar numeros primos en paralelo

#include <iostream>
#include <vector>
#include <bitset>
#include <chrono>
#include <omp.h>

// last number + 1
#define MAX 10001

void eratosthenes(std::bitset<MAX> &prime);

void eratosthenesParallel(std::bitset<MAX>& prime, int ini, int end);

void printVector(const std::bitset<MAX> &prime, int lim = MAX - 1);

int main() 
{
    std::bitset<MAX> prime;
    prime.set();
    //Sequential
    auto start = std::chrono::high_resolution_clock::now();
    eratosthenes(prime);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    printVector(prime, 999);
    std::cout << "Secuential time : \t" << elapsed_time.count() << " s\n\n\n\n";
    
    prime.set();

    //Parallel
    int block = (MAX - 1) / 10;
    start = std::chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(10)
    {
        int thread = omp_get_thread_num();
        eratosthenesParallel(prime, thread * block, (thread + 1) * block);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed_time = end - start;
    printVector(prime, 999);
    std::cout << "Parallel time : \t" << elapsed_time.count() << " s\n\n\n\n";



    return 0;
}

void eratosthenes(std::bitset<MAX> &prime)
{
    prime[0] = 0;
    prime[1] = 0;
    for (int p = 2; p * p <= MAX - 1; ++p) {
        if (prime[p]) {
            for (int i = p * p; i <= MAX - 1; i += p) {
                prime[i] = false;
            }
        }
    }
}

void eratosthenesParallel(std::bitset<MAX>& prime, int ini, int end)
{
    prime[0] = 0;
    prime[1] = 0;
    if (ini == 0) ini += 2;
    for (int p = ini; p * p <= end; ++p) {
        if (prime[p]) {
            for (int i = p * p; i <= MAX - 1; i += p) {
                prime[i] = false;
            }
        }
    }
}

void printVector(const std::bitset<MAX> &prime, int lim)
{
    for (int i = 2; i <= lim; ++i) {
        if (prime[i]) {
            std::cout << i << '\t';
        }
    }
    std::cout << '\n';
}
