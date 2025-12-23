#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "defs.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <Nome do Gaules>\n", argv[0]);
        return 1;
    }
    char nome = argv[1][0];
    srand(time(NULL) + getpid());

    // Abrimos javalis como NONBLOCK
    mqd_t q_javalis = mq_open(FILA_JAVALIS, O_RDONLY | O_NONBLOCK); 
    mqd_t q_garcom  = mq_open(FILA_GARCOM,  O_WRONLY | O_NONBLOCK);

    if (q_javalis == (mqd_t)-1 || q_garcom == (mqd_t)-1) {
        perror("Erro ao abrir filas");
        exit(1);
    }

    printf("Gaules %c entrou na fila.\n", nome);
    char javali;
    char pedido = '!';
    struct mq_attr estado_fila; // Estrutura para guardar info da fila

    while(1) {
        // Tenta pegar um javali
        if (mq_receive(q_javalis, &javali, MSG_SIZE, NULL) >= 0) {
            

            // Consulta quantos javalis sobraram na fila
            mq_getattr(q_javalis, &estado_fila);
            
            // estado_fila.mq_curmsgs contem o numero atual de mensagens
            printf("Gaules %c: Nhac! Comi um javali. Restam: %ld\n", nome, estado_fila.mq_curmsgs);
            // --------------------

            sleep(rand() % 3 + 1);

        } else {
            // Se falhou (fila vazia)
            if (errno == EAGAIN) {
                printf("Gaules %c: Pote vazio! Acordando cozinheiro...\n", nome);
                
                // Avisa o cozinheiro
                mq_send(q_garcom, &pedido, sizeof(char), 0);

                // Muda para bloqueante para esperar comida
                struct mq_attr attr_temp;
                mq_getattr(q_javalis, &attr_temp);
                attr_temp.mq_flags = 0; // Remove NONBLOCK
                mq_setattr(q_javalis, &attr_temp, NULL); 

                printf("Gaules %c: Esperando servirem...\n", nome);
                mq_receive(q_javalis, &javali, MSG_SIZE, NULL);
                
                // (quando acorda e come) ---
                mq_getattr(q_javalis, &estado_fila);
                printf("Gaules %c: Finalmente! Comi apos esperar. Restam: %ld\n", nome, estado_fila.mq_curmsgs);
                // --------------------------------------------------

                // Retorna para modo Non-Blocking
                attr_temp.mq_flags = O_NONBLOCK;
                mq_setattr(q_javalis, &attr_temp, NULL);

                sleep(rand() % 3 + 1);
            } else {
                perror("Erro inesperado");
                break;
            }
        }
    }
    return 0;
}

/*
gcc cozinheiro.c -o cozinheiro -lrt -lpthread
gcc gaules.c -o gaules -lrt -lpthread
chmod +x run.sh
./run.sh
*/
