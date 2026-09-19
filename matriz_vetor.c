#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N;
int num_threads;
float *M, *V, *R;

void* mult_linhas(void* arg) {
    long id = (long)arg;
    long linhas_por_thread = N / num_threads;
    long inicio = id * linhas_por_thread;
    long fim = (id == num_threads - 1) ? N : inicio + linhas_por_thread;

    for (long i = inicio; i < fim; i++) {
        float soma_parcial = 0.0f;
        for (long j = 0; j < N; j++) {
            soma_parcial += M[i * N + j] * V[j];
        }
        R[i] = soma_parcial; // Escrita independente por thread
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    N = atol(argv[1]);
    num_threads = atoi(argv[2]);

    M = malloc(N * N * sizeof(float));
    V = malloc(N * sizeof(float));
    R = malloc(N * sizeof(float));

    for (long i = 0; i < N; i++) V[i] = 1.0f;
    for (long i = 0; i < N * N; i++) M[i] = 2.0f;

    pthread_t threads[num_threads];
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, mult_linhas, (void*)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Matriz x Vetor Finalizado | Tempo: %.4f s\n", tempo);

    free(M); free(V); free(R);
    return 0;
}
