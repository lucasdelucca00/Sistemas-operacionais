#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define M 21

int javalis = M;
int esperando = 0;
pthread_mutex_t mutex;
sem_t vazio, cheio;

void* Gaules(void* arg){
    char nome = *(char*)arg;

    while(1){
        pthread_mutex_lock(&mutex);

        if(javalis == 0){
            esperando++;
            printf("Gaules %c acordou cozinheiro e vai esperar\n", nome);
            sem_post(&vazio);
            pthread_mutex_unlock(&mutex);

            sem_wait(&cheio); 
            pthread_mutex_lock(&mutex);
            esperando--; 
        }


        javalis--;
        printf("Gaules %c comendo (%d restantes)\n", nome, javalis);

        pthread_mutex_unlock(&mutex);
        sleep(rand()%2 + 1);
    }

    return NULL;
}

void* Cozinheiro(void* arg){
    while(1){
        sem_wait(&vazio);
        pthread_mutex_lock(&mutex);

        javalis = M;
        printf("\nCozinheiro reabasteceu %d javalis!\n", M);


        for(int i = 0; i < esperando; i++){
            sem_post(&cheio);
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    pthread_t tgaules[N], tcozinheiro;
    char nomes[N] = {'l','u','c','a','s'};

    pthread_mutex_init(&mutex, NULL);
    sem_init(&vazio, 0, 0);
    sem_init(&cheio, 0, 0);

    srand(time(NULL));

    pthread_create(&tcozinheiro, NULL, Cozinheiro, NULL);
    for(int i=0; i<N; i++){
        pthread_create(&tgaules[i], NULL, Gaules, &nomes[i]);
    }

    for(int i=0; i<N; i++) pthread_join(tgaules[i], NULL);
    pthread_join(tcozinheiro, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&vazio);
    sem_destroy(&cheio);

    return 0;
}

