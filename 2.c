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

void *escritora(void *thread_id) {
    int tid = *(int *) thread_id;

    while (1) {
        pthread_mutex_lock(&mutex);
        escritor++;
        if(escritor == 1) {
            sem_wait(&leitura);
        }
        pthread_mutex_unlock(&mutex);
        
        sem_wait(&escrita);
        variavelCompartilhada = tid;
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

    while (1) {
        sem_wait(&leitura);
        pthread_mutex_lock(&mutex2);
        if(leitor == 1) {
            sem_wait(&escrita);
        }
        pthread_mutex_unlock(&mutex2);

        sem_post(&leitura);
        printf("A thread %d realizou a leitura do valor: %d\n", tid, variavelCompartilhada);

        pthread_mutex_lock(&mutex2);
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