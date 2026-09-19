#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long N;
int num_threads;
long pontos_no_circulo_global = 0;
pthread_mutex_t mutex_pi = PTHREAD_MUTEX_INITIALIZER;

void* monte_carlo(void* arg) {
    long id = (long)arg;
    long pontos_por_thread = N / num_threads;
    
    // Tratamento de resto da divisão
    if (id < (N % num_threads)) pontos_por_thread++;

    unsigned int seed = time(NULL) + id; 
    long pontos_locais = 0;

    for (long i = 0; i < pontos_por_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            pontos_locais++;
        }
    }

    pthread_mutex_lock(&mutex_pi);
    pontos_no_circulo_global += pontos_locais;
    pthread_mutex_unlock(&mutex_pi);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    N = atol(argv[1]);
    num_threads = atoi(argv[2]);

    pthread_t threads[num_threads];
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, monte_carlo, (void*)i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * pontos_no_circulo_global / N;
    printf("Pi Estimado: %.5f | Tempo: %.4f s\n", pi, tempo);

    pthread_mutex_destroy(&mutex_pi);
    return 0;
}
