#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t filho1, filho2;

    // Criando o primeiro filho
    filho1 = fork();

    if (filho1 == -1) {
        perror("Erro ao criar filho1");
        exit(0);
    }

    if (filho1 == 0) {
        // Processo Filho 1
        printf("Filho 1 foi criado\n");
        for (int i = 100; i <= 200; i++) {
            printf("[Filho1 - PID:%d | Pai - PID:%d] %d\n", getpid(), getppid(), i);
            sleep(1);
        }
        printf("[Filho1 - PID:%d | Pai - PID:%d] Filho 1 vai morrer\n", getpid(), getppid());
        exit(0);
    } else {
        // Processo Pai
        for (int i = 1; i <= 50; i++) {
            printf("[Pai - PID:%d | Bash - PID:%d] %d\n", getpid(), getppid(), i);
            sleep(2);
        }
        printf("[Pai - PID:%d | Bash - PID:%d] Processo pai vai morrer\n", getpid(), getppid());

        // Pai espera o Filho1 terminar
        waitpid(filho1, NULL, 0);

        // Só depois cria o Filho2
        filho2 = fork();

        if (filho2 == -1) {
            perror("Erro ao criar filho2");
            exit(0);
        }

        if (filho2 == 0) {
            // Processo Filho 2
            for (int i = 51; i <= 99; i++) {
                printf("[Filho2 - PID:%d | Pai - PID:%d] %d\n", getpid(), getppid(), i);
                sleep(1);
            }
            printf("[Filho2 - PID:%d | Pai - PID:%d] Filho 2 vai morrer\n", getpid(), getppid());
            exit(0);
        } else {
            // Pai espera o Filho2 antes de encerrar totalmente
            waitpid(filho2, NULL, 0);
        }
    }

    exit(0); 
    
    //comandos para o terminal "ps -la" e "ps -fu"
}

