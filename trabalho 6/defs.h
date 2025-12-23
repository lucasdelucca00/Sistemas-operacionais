#ifndef DEFS_H
#define DEFS_H

#include <semaphore.h>

#define SHM_NAME "/jantar_gauleses"
#define M 21 // Quantidade de javalis = RA = 2157217

// Estrutura da memória compartilhada
typedef struct {
    int javalis;
    int esperando;
    sem_t mutex; // (semáforo binário)
    sem_t vazio; // Sinaliza que o pote está vazio (Cozinheiro dorme aqui)
    sem_t cheio; // Sinaliza que há comida (Gauleses dormem aqui se vazio)
} SharedData;

#endif
