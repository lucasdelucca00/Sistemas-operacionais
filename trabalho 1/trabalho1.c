// trabalho1.c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static volatile sig_atomic_t contador_tstp = 0;

void SIGTSTP_handler() {
    contador_tstp++;
    printf("[handler] SIGTSTP recebido. Contador = %d\n", contador_tstp);
    fflush(stdout);

    if (contador_tstp >= 12) {
        printf("[handler] Contador chegou a 12. Encerrando com SIGTERM...\n");
        fflush(stdout);
        kill(getpid(), SIGTERM); // ação default = terminar processo
    }
}

void SIGPIPE_handler() {
    contador_tstp = 0;
    printf("[handler] SIGPIPE recebido. Contador resetado para 0.\n");
    fflush(stdout);
}

static unsigned int dormir_total(unsigned int segundos) {
    // dorme 'segundos' mesmo que seja interrompido por sinais
    while (segundos) segundos = sleep(segundos);
    return 0;
}


int main() 
{
    pid_t pid = getpid();

    printf("PID do processo: %ld\n", (long)pid);
    printf("Ações:\n");
    printf("  - SIGINT (CTRL+C): ignorado\n");
    printf("  - SIGTSTP (CTRL+Z ou kill -TSTP <PID>): contador++\n");
    printf("  - SIGPIPE (kill -PIPE <PID>): zera contador\n");
    printf("Gerando SIGTSTP automaticamente a cada 10 segundos...\n\n");
    fflush(stdout);

    // Instala tratadores no estilo "signal"
    signal(SIGINT, SIG_IGN);         // SIGINT é ignorado
    signal(SIGTSTP, SIGTSTP_handler);
    signal(SIGPIPE, SIGPIPE_handler);

    // Loop eterno
    for (;;) {
        dormir_total(10);                   // espera 10s (robusto contra interrupções)
        printf("[main] Gerando SIGTSTP automático...\n");
        fflush(stdout);
        kill(getpid(), SIGTSTP);            // gera SIGTSTP em si mesmo
    }
}
