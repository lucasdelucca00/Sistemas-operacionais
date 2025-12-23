#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include "defs.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <Nome do Gaules>\n", argv[0]);
        return 1;
    }
    
    char nome = argv[1][0]; // Pega o primeiro char do argumento
    srand(time(NULL) + getpid()); // Seed aleatória baseada no PID

    // 1. Abre a memória compartilhada (apenas leitura/escrita, sem O_CREAT)
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Erro ao abrir shm (O cozinheiro esta rodando?)");
        exit(1);
    }

    // 2. Mapeia a memória
    SharedData *dados = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (dados == MAP_FAILED) {
        perror("Erro no mmap");
        exit(1);
    }

    printf("Gaules %c entrou na fila.\n", nome);

    while(1) {
        sem_wait(&dados->mutex); // Lock - so processo atual vai mexer

        if (dados->javalis == 0) {
            dados->esperando++;
            printf("Gaules %c: Acordou cozinheiro e vai esperar.\n", nome);
            sem_post(&dados->vazio); // Acorda o cozinheiro
            sem_post(&dados->mutex); // Libera o mutex antes de dormir - evita deadlock
            
            sem_wait(&dados->cheio); // Dorme esperando comida
            
            sem_wait(&dados->mutex); // Readquire o mutex para comer
            dados->esperando--;
        }

        dados->javalis--;
        printf("Gaules %c: Comendo (%d restantes).\n", nome, dados->javalis);
        
        sem_post(&dados->mutex); // Unlock

        sleep(rand() % 3 + 1); // Come e descansa um pouco
    }

    return 0;
}

/*
gcc cozinheiro.c -o cozinheiro -lrt -lpthread
gcc gaules.c -o gaules -lrt -lpthread
chmod +x run.sh
./run.sh
*/
