#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


// boolean ==> threads leitoras não podem ler enquanto está ocorrendo escrita
int writeStatus = 0;

int threadsReading = 0;
int threadsWriting = 0;

// quantidade de leituras e escritas será efetuada
int readings = 10;
int writings = 10;
int sharedVariable = -1;

int readersNumber = 5;
int writersNumber = 5;

pthread_mutex_t mutex_r, mutex_w;
sem_t writers, readers;



pthread_mutex_t writingsControl, readingsControl, turnProtection;
pthread_cond_t permissionToWrite, permissionToRead;
int turn = 0;  //Turn = 0: Escritoras -- Turn = 1: Leitoras


void *writer(void *id) {
    int tid = *(int *) id;
    printf("A thread %d foi inicializada como ==> ESCRITORA <==\n", tid);

    pthread_mutex_lock(&writingsControl);
    while (writings > 0) {
        writings--;
        pthread_mutex_unlock(&writingsControl);
        printf("A thread %d escreveu\n", tid);

    }
    pthread_mutex_unlock(&writingsControl);
    printf("A thread %d encerrou como ==> ESCRITORA <==.\n", tid);

    pthread_exit(NULL);
}


void *reader(void *id) {
    int tid = *(int *) id;
    printf("A thread %d foi inicializada como ==> LEITORA <==.\n", tid);

    pthread_mutex_lock(&readingsControl);
    while (readings > 0) {
        readings--;
        pthread_mutex_unlock(&readingsControl);
        printf("A thread %d leu\n", tid);
    }
    pthread_mutex_unlock(&mutex_r);
    printf("A thread %d encerrou como ==> LEITORA <==.\n", tid);

    pthread_exit(NULL);
}



void *reader2(void *id) {
    int tid = *(int *) id;
    printf("A thread %d iniciou como LEITORA.\n", tid);

    while(1) {
        pthread_mutex_lock(&readingsControl);
        if(readings > 0) {
            readings--;
            pthread_mutex_unlock(&readingsControl);
        }
        else {
            pthread_mutex_unlock(&readingsControl);
            break;
        }

        pthread_mutex_lock(&turnProtection);
        while (turn != 1) {
            pthread_cond_wait(&permissionToRead, &turnProtection);
        }
        turn = (turn + 1) % 2;
        pthread_mutex_unlock(&turnProtection);
        
        printf("A thread %d leu a variável compartilhada com o valor -> %d\n", tid, sharedVariable);

        pthread_cond_signal(&permissionToWrite);

        while(writings == 0 && readings > 0) {
            pthread_cond_signal(&permissionToWrite);
        }
    }
    printf("A thread %d foi encerrada.\n", tid);
}


void *writer2(void *id) {
    int tid = *(int *) id;
    printf("A thread %d iniciou como ESCRITORA.\n", tid);

    while(1) {

        //sem_wait(&singleWriters);

        pthread_mutex_lock(&writingsControl);
        if(writings > 0) {
            writings--;
            pthread_mutex_unlock(&writingsControl);
        }
        else {
            pthread_mutex_unlock(&writingsControl);
            break;
        }

        pthread_mutex_lock(&turnProtection);
        while (turn != 0) {
            pthread_cond_wait(&permissionToWrite, &turnProtection);
        }
        turn = (turn + 1) % 2;
        pthread_mutex_unlock(&turnProtection);

        sharedVariable = tid;

        pthread_cond_broadcast(&permissionToRead);
        //sem_post(&singleWriters);

        while(writings == 0 && readings > 0) {
            pthread_cond_broadcast(&permissionToRead);
        }

    }
    printf("A thread %d foi encerrada.\n", tid);
}




int main (int argc, char *argv[]) {

    //Instanciação de variáveis
    int threadsNumber = readersNumber + writersNumber;
    pthread_t *systemTID;
    int *tid;

    //Inicialização de semáforos e mutex
    pthread_mutex_init(&readingsControl, NULL);
    pthread_mutex_init(&writingsControl, NULL);


    //Inicialização de threads
    systemTID = (pthread_t *) malloc(sizeof(pthread_t) * threadsNumber);
    int *id_w;
    int *id_r;
    int i;
    for (i = 0; i < threadsNumber; i++) {
        if (id_w = malloc(sizeof(int))) {
            *id_w = i;
        }

        if(writersNumber > i) {
            pthread_create(&systemTID[i], NULL, writer2, (void *) id_w);
        }
    }

    for (int k = 0; k < threadsNumber; k++) {
        if (id_r = malloc(sizeof(int))) {
            *id_r = k + writersNumber;
        }
    

        if(readersNumber > k) {
            pthread_create(&systemTID[k + writersNumber], NULL, reader2, (void*) id_r);
        }
    }


    //Encerramento de threads
    for(int j = 0; j < threadsNumber; j++) {
        pthread_join(systemTID[j], NULL);
    }

    return 0;
}
