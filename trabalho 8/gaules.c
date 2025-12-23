#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h> 
#include <time.h>
#include <errno.h>
#include "defs.h"

// Função auxiliar para mudar flags do arquivo (Bloqueante <-> NonBlocking)
void set_blocking(int fd, int is_blocking) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (is_blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <Nome do Gaules>\n", argv[0]);
        return 1;
    }
    char nome = argv[1][0];
    srand(time(NULL) + getpid());

    int fd_javalis = open(FIFO_JAVALIS, O_RDONLY | O_NONBLOCK);
    int fd_garcom  = open(FIFO_GARCOM, O_WRONLY);

    if (fd_javalis == -1 || fd_garcom == -1) {
        perror("Erro ao abrir pipes");
        exit(1);
    }

    printf("Gaules %c entrou na fila.\n", nome);
    char javali;
    char aviso = '!';
    int restantes = 0; // Variável para guardar a contagem

    while(1) {
        // Tenta pegar 1 javali
        ssize_t bytes = read(fd_javalis, &javali, 1);

        if (bytes > 0) {
            // SUCESSO: Comeu
            

            // Pergunta ao Kernel quantos bytes ainda tem no pipe
            if (ioctl(fd_javalis, FIONREAD, &restantes) == -1) {
                restantes = -1; // Em caso de erro
            }
            
            printf("Gaules %c: Nhac! Comi um javali. Restam: %d\n", nome, restantes);


            sleep(rand() % 3 + 1);
        } 
        else {
            // FALHA: Pipe vazio
            if (errno == EAGAIN) {
                printf("Gaules %c: Pote vazio! Gritando para o cozinheiro...\n", nome);
                
                write(fd_garcom, &aviso, 1);
                
                // Muda para bloqueante e espera
                set_blocking(fd_javalis, 1);
                printf("Gaules %c: Esperando reposicao...\n", nome);
                
                read(fd_javalis, &javali, 1);
                
                // --- NOVO TRECHO TAMBÉM AQUI ---
                if (ioctl(fd_javalis, FIONREAD, &restantes) == -1) restantes = -1;
                printf("Gaules %c: Finalmente! Comi apos esperar. Restam: %d\n", nome, restantes);
                // -------------------------------

                set_blocking(fd_javalis, 0);
                sleep(rand() % 3 + 1);
            } else {
                perror("Erro inesperado");
                break;
            }
        }
    }

    close(fd_javalis);
    close(fd_garcom);
    return 0;
}
