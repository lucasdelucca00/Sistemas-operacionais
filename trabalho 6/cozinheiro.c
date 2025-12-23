#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include "defs.h"

SharedData *dados;
int shm_fd;

// Função para limpar memória ao encerrar (Ctrl+C)
void cleanup(int sig) {
    printf("\nCozinheiro encerrando e limpando memória...\n");
    munmap(dados, sizeof(SharedData));
    close(shm_fd);
    shm_unlink(SHM_NAME); // Remove o objeto da memória do sistema
    exit(0);
}

int main() {
    signal(SIGINT, cleanup); // Captura Ctrl+C

    // 1. Cria a memória compartilhada
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Erro no shm_open");
        exit(1);
    }

    // 2. Define o tamanho da memória - tamanho da struct
    ftruncate(shm_fd, sizeof(SharedData));

    // 3. Mapeia a memória RAM para o ponteiro
    dados = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (dados == MAP_FAILED) {
        perror("Erro no mmap");
        exit(1);
    }

    // 4. Inicializa variáveis e semáforos
    // O segundo parâmetro do mutex (1) indica que o semáforo é compartilhado entre PROCESSOS
    sem_init(&dados->mutex, 1, 1); // Mutex inicia liberado (1)
    sem_init(&dados->vazio, 1, 0); // Vazio inicia bloqueado (0)
    sem_init(&dados->cheio, 1, 0); // Cheio inicia bloqueado (0)
    
    dados->javalis = M; // 21
    dados->esperando = 0;

    printf("Cozinheiro: Caldeirão pronto com %d javalis.\n", M);

    while(1) {
        // Espera ser acordado (quando javalis == 0)
        sem_wait(&dados->vazio);

        sem_wait(&dados->mutex);
        
        printf("\nCozinheiro: Acordei! Reabastecendo...\n");
        dados->javalis = M;
        printf("Cozinheiro: %d javalis prontos!\n", M);

        // Acorda todos os gauleses que estavam esperando
        int acordar = dados->esperando;
        for(int i = 0; i < acordar; i++) {
            sem_post(&dados->cheio);
        }
        
        sem_post(&dados->mutex);
    }

    return 0;
}
