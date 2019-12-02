#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int escritas = 100;
int leituras = 100;
int nthreads = 10;
int nthreads_escritoras = 3;
int nthreads_leitoras = 3;

int escritor = 0, leitor = 0;

sem_t leitura, escrita;
pthread_mutex_t mutex, mutex2;
int variavelCompartilhada = -1;
double leituras_restantes = 100;

void *escritora(void *thread_id) {
    int tid = *(int *) thread_id;

    while (escritas > 0) {
        pthread_mutex_lock(&mutex);
        escritor++;
        if(escritor == 1) {
            sem_wait(&leitura);
        }
        pthread_mutex_unlock(&mutex);
        
        sem_wait(&escrita);
        escritas--;
        variavelCompartilhada = tid;
        printf("A thread %d realizou a ESCRITA do valor -> %d\n", tid, variavelCompartilhada);
        sem_post(&escrita);

        pthread_mutex_lock(&mutex);
        escritor--;
        if(escritor == 0) {
            sem_post(&leitura);
        }
        pthread_mutex_unlock(&mutex);

    }
    printf("escrita terminada\n");

}



void *leitora(void * thread_id) {
    int tid = *(int *) thread_id;

    while (leituras_restantes > 0) {
        sem_wait(&leitura);
        pthread_mutex_lock(&mutex2);
        leitor++;
        if(leitor == 1) {
            sem_wait(&escrita);
        }
        pthread_mutex_unlock(&mutex2);

        sem_post(&leitura);
        printf("A thread %d realizou a LEITURA do valor: %d\n", tid, variavelCompartilhada);
        leituras_restantes--;

        pthread_mutex_lock(&mutex2);
        leitor--;
        if(leitor == 0) {
            sem_post(&escrita);
        }
        pthread_mutex_unlock(&mutex2);

    }
    printf("leitura terminada\n");
}




int main (int argc, char *argv[]) {
    pthread_t *systemTID;
    int *tid;
    int i, k, j;

    sem_init(&leitura, 0, 1);
    sem_init(&escrita, 0, 1);

    systemTID = (pthread_t *) malloc (sizeof(pthread_t *) * nthreads);

    for(i = 0; i < nthreads_escritoras; i++) {
        if(tid = malloc(sizeof(int)))
            *tid = i;
        pthread_create(&systemTID[i], NULL, escritora, (void *) tid);
    }

    for(k = 0; k < nthreads_leitoras; k++) {
        if(tid = malloc(sizeof(int)))
            *tid = i + k;
        pthread_create(&systemTID[i], NULL, leitora, (void *) tid);
    }

    for(j = 0; j < nthreads; j++) {
        pthread_join(systemTID[j], NULL);
    }
   

    return 0;
}