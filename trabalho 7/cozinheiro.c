#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "defs.h"

mqd_t q_javalis, q_garcom;

void cleanup(int sig) {
    printf("\nCozinheiro encerrando e removendo filas...\n");
    mq_close(q_javalis);
    mq_close(q_garcom);
    mq_unlink(FILA_JAVALIS);
    mq_unlink(FILA_GARCOM);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    // Configuração dos atributos da fila
    struct mq_attr attr_javalis;
    attr_javalis.mq_flags = 0;
    attr_javalis.mq_maxmsg = M;      // Máximo de javalis no pote
    attr_javalis.mq_msgsize = MSG_SIZE;
    attr_javalis.mq_curmsgs = 0;

    struct mq_attr attr_garcom;
    attr_garcom.mq_flags = 0;
    attr_garcom.mq_maxmsg = 1;       // Apenas 1 pedido por vez (evita flood)
    attr_garcom.mq_msgsize = MSG_SIZE;
    attr_garcom.mq_curmsgs = 0;

    // 1. Criação das Filas
    // Cozinheiro cria com permissões de leitura/escrita
    q_javalis = mq_open(FILA_JAVALIS, O_CREAT | O_WRONLY, 0666, &attr_javalis);
    q_garcom  = mq_open(FILA_GARCOM,  O_CREAT | O_RDONLY, 0666, &attr_garcom);

    if (q_javalis == (mqd_t)-1 || q_garcom == (mqd_t)-1) {
        perror("Erro ao criar filas (verifique se M <= 10)");
        exit(1);
    }

    printf("Cozinheiro: Filas criadas. Enchendo o pote inicial...\n");
    
    // Enche o pote pela primeira vez
    char javali = 'J';
    for(int i=0; i<M; i++) {
        mq_send(q_javalis, &javali, sizeof(char), 0);
    }
    printf("Cozinheiro: Pote cheio (%d javalis). Indo dormir.\n", M);

    char buffer[MSG_SIZE];
    while(1) {
        // 2. Espera um pedido (Bloqueia aqui até um gaulês chamar)
        ssize_t bytes_read = mq_receive(q_garcom, buffer, MSG_SIZE, NULL);
        
        if (bytes_read >= 0) {
            printf("\nCozinheiro: Recebi um chamado! Reabastecendo...\n");
            
            // Reabastece o pote
            for(int i=0; i<M; i++) {
                // Tenta enviar. Se a fila estiver cheia, não trava (apenas ignora ou espera)
                // Aqui assumimos que se ele acordou, é porque estava vazio.
                mq_send(q_javalis, &javali, sizeof(char), 0);
            }
            printf("Cozinheiro: %d javalis servidos!\n", M);
        }
    }
    return 0;
}
