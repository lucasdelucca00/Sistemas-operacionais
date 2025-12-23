#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define N_TERMS 1000000000L  // 1 bilhao de termos
#define MAX_THREADS 16 // num max de threads


double results[8*MAX_THREADS]; // vetor global para guardar os resultados parciais de cada thread
int n_threads; // numero atual de threads para cada operacao

// ===== Série de Leibniz =====
void* leibniz(void* arg) {
    long id = (long)(intptr_t)arg; // identificador da thread
    long start = id * (N_TERMS / n_threads); // indice inicial
    long end   = (id + 1) * (N_TERMS / n_threads); // indice final

    double sum = 0.0; // soma parcial
    for (long k = start; k < end; k++) {
        double termo = (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0);
        sum += termo; // soma parcial dos termos de cada thread
    }

    results[8*id] = 4.0 * sum; // resultado parcial, depois na main sera somado 3
    return NULL;
}

// ===== Série de Nilakantha =====
void* nilakantha(void* arg) {
    long id = (long)(intptr_t)arg;
    long start = id * (N_TERMS / n_threads);
    long end   = (id + 1) * (N_TERMS / n_threads); 

    double sum = 0.0; 
    for (long k = start; k < end; k++) {
        double termo = (k % 2 == 0 ? 1.0 : -1.0) * (4.0 / ((2.0*(k+1))*(2.0*(k+1)+1)*(2.0*(k+1)+2)));
        sum += termo; 
    }

    results[8*id] = sum; // resultado parcial
    return NULL;
}

double run_series(void* (*func)(void*)) {
    pthread_t threads[MAX_THREADS]; // array de threads
    double pi = 0.0; // acumolo final das threads

    // for para criar e iniciar as threads
    for (long i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, func, (void*)(intptr_t)i);
    }

    // for para aguardar o termino das threads e somar os resultados parciais
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
        pi += results[8*i]; 
    }
    return pi; // soma dos resultados parciais
}

int main() {
    struct timespec start, end; // variaveis para medir o tempo
    double pi_approx, elapsed; // variaveis para o valor aproximado de pi e tempo decorrido

    int thread_counts[] = {1, 2, 4, 8, 16}; // numeros de threads
    int n_cases = sizeof(thread_counts)/sizeof(thread_counts[0]); // numero de elememtos no array

    for (int i = 0; i < n_cases; i++) {
        n_threads = thread_counts[i]; // atualiza o numero de threads

        printf("\n=== Usando %d threads ===\n", n_threads);

        // Leibniz
        clock_gettime(CLOCK_MONOTONIC, &start); // inicio da medicao de tempo
        pi_approx = run_series(leibniz); // calcula a aproximacao de pi
        clock_gettime(CLOCK_MONOTONIC, &end); // fim da medicao de tempo
        elapsed = (end.tv_sec - start.tv_sec) +  // calcula o tempo decorrido
                  (end.tv_nsec - start.tv_nsec)/1e9;
        printf("Leibniz: π ≈ %.15f (tempo %.3fs, erro %.15f)\n", 
               pi_approx, elapsed, fabs(M_PI - pi_approx));

        // Nilakantha
        clock_gettime(CLOCK_MONOTONIC, &start);
        pi_approx = 3.0 + run_series(nilakantha);
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed = (end.tv_sec - start.tv_sec) + 
                  (end.tv_nsec - start.tv_nsec)/1e9;
        printf("Nilakantha: π ≈ %.15f (tempo %.3fs, erro %.15f)\n",
               pi_approx, elapsed, fabs(M_PI - pi_approx));
    }

    exit (0);
}

