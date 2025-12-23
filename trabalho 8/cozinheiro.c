#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include "defs.h"

int fd_javalis, fd_garcom;

void cleanup(int sig) {
    printf("\nCozinheiro encerrando e removendo pipes...\n");
    close(fd_javalis);
    close(fd_garcom);
    unlink(FIFO_JAVALIS); // Deleta o arquivo do pipe
    unlink(FIFO_GARCOM);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    // 1. Cria os Named Pipes (FIFOs) se não existirem
    if (mkfifo(FIFO_JAVALIS, 0666) == -1) {
        // Ignora erro se arquivo já existe
    }
    if (mkfifo(FIFO_GARCOM, 0666) == -1) {
        // Ignora erro se arquivo já existe
    }

    printf("Cozinheiro: Pipes criados.\n");

    // 2. Abre os pipes
    // Importante: Abrir pipe de leitura bloqueia até alguém abrir para escrita e vice-versa.
    // O Cozinheiro abre JAVALIS para escrita e GARCOM para leitura.
    
    // Dica: Usamos O_RDWR no pipe_javalis para o cozinheiro não receber SIGPIPE 
    // ou fechar o pipe se todos os gauleses saírem.
    fd_javalis = open(FIFO_JAVALIS, O_WRONLY);
    fd_garcom  = open(FIFO_GARCOM, O_RDWR); // RDWR evita bloqueio eterno se ninguem abrir

    if (fd_javalis == -1 || fd_garcom == -1) {
        perror("Erro ao abrir pipes");
        exit(1);
    }

    // Enche o pote pela primeira vez
    char token = 'J';
    printf("Cozinheiro: Servindo a primeira rodada...\n");
    for(int i=0; i<M; i++) {
        write(fd_javalis, &token, 1);
    }
    printf("Cozinheiro: %d javalis servidos. Dormindo...\n", M);

    char buffer;
    while(1) {
        // 3. Bloqueia esperando aviso do garçom
        // O read vai travar aqui até um gaulês escrever no fifo_garcom
        ssize_t bytes = read(fd_garcom, &buffer, 1);

        if (bytes > 0) {
            printf("\nCozinheiro: Ouvi o chamado! Reabastecendo...\n");
            for(int i=0; i<M; i++) {
                write(fd_javalis, &token, 1);
            }
            printf("Cozinheiro: %d javalis servidos!\n", M);
        }
    }

    return 0;
}
