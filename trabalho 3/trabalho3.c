#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

volatile sig_atomic_t flag = 1; // controle do loop do filho2

void handler(int sig) {
    flag = 0; // ao receber sinal, quebra o loop
}

int main() {
    pid_t filho1, filho2;
    int status;

    // cria filho1
    filho1 = fork();
    if (filho1 == 0) {
        // exec substitui o processo por outro programa (filho1)
        execl("./filho1", "filho1", NULL);
        perror("execl falhou");
        exit(0);
    }

    // cria filho2
    filho2 = fork();
    if (filho2 == 0) {
        // instala tratador de sinal
        signal(SIGUSR1, handler);
        // loop infinito até sinal
        while (flag) {
            // apenas esperando...
        }
        printf("Filho2 acordou e vai terminar o loop.\n");
        // termina mas não será waitado → vira zumbi
        exit(0);
    }

    // PAI
    // espera filho1
    waitpid(filho1, &status, 0);
    if (WIFEXITED(status)) {
        int resultado = WEXITSTATUS(status);
        // manda sinal ao filho2
        kill(filho2, SIGUSR1);

        // imprime resultado
        printf("Resultado do Filho1 = %d\n", resultado);
    }

    // não dá wait() no filho2 → ele fica zumbi
    // pausa o pai um pouco para você ver no `ps`
    sleep(10);
    printf("Pai terminou.\n");
    return 0;
}

