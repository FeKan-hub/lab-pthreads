#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N;
int num_threads;
float *A, *B, *C;

void* soma_bloco(void* arg) {
    long id = (long)arg;
    long tamanho_bloco = N / num_threads;
    long inicio = id * tamanho_bloco;
    long fim = (id == num_threads - 1) ? N : inicio + tamanho_bloco;

    for (long i = inicio; i < fim; i++) {
        C[i] = A[i] + B[i];
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

    A = malloc(N * sizeof(float));
    B = malloc(N * sizeof(float));
    C = malloc(N * sizeof(float));

    // Inicialização pseudo-aleatória rápida
    for (long i = 0; i < N; i++) {
        A[i] = i * 1.5f;
        B[i] = i * 2.5f;
    }

    pthread_t threads[num_threads];
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, soma_bloco, (void*)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Soma Vetores Finalizada | Tempo: %.4f s\n", tempo);

    free(A); free(B); free(C);
    return 0;
}
