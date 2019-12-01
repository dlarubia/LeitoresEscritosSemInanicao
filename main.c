#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

// controle de threads escritoras realizando escrita
sem_t writeSemaphore;
pthread_mutex_t mutex_r, mutex_w, status;

//decidir se será utilizado semáforo ou variável de condição
pthread_cond_t permissionToWrite, permissionToRead;
sem_t readers;
sem_t writers;
sem_t permissionToContinue;

// boolean ==> threads leitoras não podem ler enquanto está ocorrendo escrita
int writeStatus = 0;

int threadsReading = 0;

// quantidade de leituras e escritas será efetuada
int readings = 100;
int writings = 100;
int sharedVariable = -1;



void *writer(void *id) {
    int tid = *(int *) id;
    printf("A thread %d foi inicializada como ==> ESCRITORA <==.\n", tid);

    //apenas uma escritora por vez
    pthread_mutex_lock(&mutex_w);
    while (writings > 0) {
        writings--;
        pthread_mutex_unlock(&mutex_w);

        sem_wait(&writeSemaphore);
        //bloquear as threads leitoras

        writeStatus = 1;

        while(threadsReading > 0) {
            sem_wait(&readers);
        }

        sharedVariable = tid;
        printf("A thread %d realizou a escrita. \n", tid);

        writeStatus = 0;
        pthread_cond_broadcast(&permissionToRead);
        
        sem_wait(&permissionToContinue);

        sem_post(&writeSemaphore);

        pthread_mutex_lock(&mutex_w);
    }
    pthread_mutex_unlock(&mutex_w);
    printf("A thread %d encerrou como ==> ESCRITORA <==.\n", tid);

    pthread_exit(NULL);
}


void *reader(void *id) {
    int tid = *(int *) id;
    printf("A thread %d foi inicializada como ==> LEITORA <==.\n", tid);

    pthread_mutex_lock(&mutex_r);
    while (readings > 0) {
        readings--;
        pthread_mutex_unlock(&mutex_r);


        pthread_mutex_lock(&status);
        while (writeStatus) {
            // semáforo pode ser substituido por um cond_wait?
            //sem_wait(&writers);
            printf("A thread leitora %d foi pausada para que o processo de escrita fosse realizado.\n", tid);
            pthread_cond_wait(&permissionToRead, &status);
        }
        pthread_mutex_unlock(&status);


        printf("A thread %d realizou a leitura. Valor ==> %d \n", tid, sharedVariable);

        sem_post(&permissionToContinue);

        pthread_mutex_lock(&mutex_r);
    }
    pthread_mutex_unlock(&mutex_r);
    printf("A thread %d encerrou como ==> LEITORA <==.\n", tid);

    pthread_exit(NULL);
}



int main (int argc, char *argv[]) {

    int readersNumber = 5;
    int writersNumber = 5;
    int threadsNumber = readersNumber + writersNumber;
    pthread_t *systemTID;
    int *tid;

    sem_init(&writers, 0, 1);
    sem_init(&readers, 0, 1);
    sem_init(&writeSemaphore, 0, 1);
    sem_init(&permissionToContinue, 0, 0);

    pthread_mutex_init(&mutex_r, NULL);
    pthread_mutex_init(&mutex_w, NULL);
    pthread_mutex_init(&status, NULL);


    systemTID = (pthread_t *) malloc(sizeof(pthread_t) * threadsNumber);
    int *id_w;
    int *id_r;
    int i;
    for (i = 0; i < threadsNumber; i++) {
        if (id_w = malloc(sizeof(int))) {
            *id_w = i;
        }

        if(writersNumber > i) {
            pthread_create(&systemTID[i], NULL, writer, (void *) id_w);
        }
    }

    for (int k = 0; k < threadsNumber; k++) {
        if (id_r = malloc(sizeof(int))) {
            *id_r = k + writersNumber;
        }
    

        if(readersNumber > k) {
            pthread_create(&systemTID[k + writersNumber], NULL, reader, (void*) id_r);
        }
    }


    for(int j = 0; j < threadsNumber; j++) {
        printf("Aguardando encerramento da thread: %d\n", j);
        pthread_join(systemTID[j], NULL);
    }

    return 0;
}
