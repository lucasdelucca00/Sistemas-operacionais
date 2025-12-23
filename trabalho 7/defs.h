#ifndef DEFS_H
#define DEFS_H

#include <mqueue.h>

#define FILA_JAVALIS "/q_javalis" // O Pote
#define FILA_GARCOM  "/q_garcom"  // O "Sino" para chamar o cozinheiro

#define M 10         // Capacidade do pote (Linux padrão max é 10)
#define MSG_SIZE 1   // Tamanho da mensagem (basta 1 byte pra representar o javali)

#endif
