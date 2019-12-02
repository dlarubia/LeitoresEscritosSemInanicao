#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


// boolean ==> threads leitoras não podem ler enquanto está ocorrendo escrita
int writeStatus = 0;

int threadsReading = 0;
int threadsWriting = 0;

// quantidade de leituras e escritas será efetuada
int readings = 20;
int writings = 20;
int sharedVariable = -1;

int readersNumber = 2;
int writersNumber = 3;

pthread_mutex_t mutex_r, mutex_w;
sem_t writers, readers;


// Barreira
pthread_mutex_t bar;
pthread_cond_t cond_bar;

pthread_mutex_t writingsControl, readingsControl, turnProtection, printControl, flag;
pthread_cond_t permissionToWrite, permissionToRead;
int turn = 0;  //Turn = 0: Escritoras -- Turn = 1: Leitoras
int busyState = 0;
int counter = 0;
int w_end = 0;
int flag_ = 0;

void barreira();

void *writer(void *id) {
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
        printf("A thread %d está aguardando permissão para escrita.\n", tid);
        while (turn != 0) {
            pthread_cond_wait(&permissionToWrite, &turnProtection);
        }
        turn = (turn + 1) % 2;

        sharedVariable = tid;
        //pthread_mutex_lock(&printControl);
        printf("A thread %d escreveu o valor -> %d <-\n", tid, sharedVariable);
        //pthread_mutex_unlock(&printControl);
        pthread_mutex_unlock(&turnProtection);

        pthread_cond_broadcast(&permissionToRead);
        //sem_post(&singleWriters);

        while(writings == 0 && readings > 0) {
            pthread_cond_broadcast(&permissionToRead);
            //printf("Leituras restantes: %d", readings);
        }

    }
    printf("A thread %d foi encerrada.\n", tid);

    pthread_exit(NULL);
}


void *reader(void *id) {
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
        printf("A thread %d está aguardando permissão para leitura.\n", tid);
        while (turn != 1) {
            pthread_cond_wait(&permissionToRead, &turnProtection);
        }
        //pthread_mutex_lock(&printControl);
        printf("A thread %d leu a variável compartilhada com o valor -> %d\n", tid, sharedVariable);
        //pthread_mutex_lock(&printControl);
        turn = (turn + 1) % 2;
        pthread_mutex_unlock(&turnProtection);

        pthread_cond_signal(&permissionToRead);
        pthread_cond_signal(&permissionToWrite);

        while(readings == 0 && writings > 0) {
            pthread_cond_signal(&permissionToWrite);
            //printf("Escritas restantes: %d", writings);
        }
    }
    printf("A thread %d foi encerrada.\n", tid);

    pthread_exit(NULL);
}


/*
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
        printf("A thread %d está aguardando permissão para leitura.\n", tid);
        while (turn == 0) {
            pthread_cond_wait(&permissionToRead, &turnProtection);
        }
        //pthread_mutex_lock(&printControl);
        //pthread_mutex_lock(&printControl);
        turn = (turn + 1) % readersNumber;
        pthread_mutex_unlock(&turnProtection);
        
        barreira();
        printf("A thread %d leu a variável compartilhada com o valor -> %d\n", tid, sharedVariable);

        sleep(1);

        pthread_cond_signal(&permissionToRead);
        pthread_cond_signal(&permissionToWrite);

        while(readings == 0 && writings > 0) {
            pthread_cond_signal(&permissionToWrite);
            //printf("Escritas restantes: %d", writings);
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
        printf("A thread %d está aguardando permissão para escrita.\n", tid);
        while (turn != 0) {
            pthread_cond_wait(&permissionToWrite, &turnProtection);
        }
        turn = (turn + 1) % 2;

        sharedVariable = tid;

        //pthread_mutex_lock(&printControl);
        printf("A thread %d escreveu o valor -> %d <-\n", tid, sharedVariable);
        //pthread_mutex_unlock(&printControl);
        pthread_mutex_unlock(&turnProtection);

        pthread_cond_broadcast(&permissionToRead);
        //sem_post(&singleWriters);

        while(writings == 0 && readings > 0) {
            pthread_cond_broadcast(&permissionToRead);
            //printf("Leituras restantes: %d", readings);
        }

    }
    printf("A thread %d foi encerrada.\n", tid);
}
*/

void barreira() {
    pthread_mutex_lock(&bar);
    counter++;
    if(counter < readersNumber) {
        pthread_cond_wait(&cond_bar, &bar);
    }

    else {
        counter = 0;
        pthread_cond_broadcast(&cond_bar);
    }
    pthread_mutex_unlock(&bar);
}


int main (int argc, char *argv[]) {

    //Instanciação de variáveis
    int threadsNumber = readersNumber + writersNumber;
    pthread_t *systemTID;

    //Inicialização de semáforos e mutex
    pthread_mutex_init(&readingsControl, NULL);
    pthread_mutex_init(&writingsControl, NULL);
    pthread_mutex_init(&turnProtection, NULL);
    pthread_mutex_init(&flag, NULL);
    pthread_mutex_init(&bar, NULL);
    pthread_cond_init(&cond_bar, NULL);
    
    /**
    int readersNumber;
    int writersNumber;
    int writings, readings;
    
    //Leitura entrada padrão
	printf("Entre com o numero de threads escritoras\n");
	scanf("%d", &writersNumber);
	printf("Entre com o numero de threads leitoras\n");
	scanf("%d", &readersNumber);
	printf("Entre com o numero de escritas\n");
	scanf("%d", &writings);
	printf("Entre com o numero de leituras\n");
	scanf("%d", &readings);
    **/


    //Inicialização de threads
    systemTID = (pthread_t *) malloc(sizeof(pthread_t) * threadsNumber);
    int *id_w;
    int *id_r;
    int i;
    for (i = 0; i < threadsNumber; i++) {
        if ((id_w = malloc(sizeof(int)))) {
            *id_w = i;
        }

        if(writersNumber > i) {
            pthread_create(&systemTID[i], NULL, writer, (void *) id_w);
        }
    }

    for (int k = 0; k < threadsNumber; k++) {
        if ((id_r = malloc(sizeof(int)))) {
            *id_r = k + writersNumber;
        }
    

        if(readersNumber > k) {
            pthread_create(&systemTID[k + writersNumber], NULL, reader, (void*) id_r);
        }
    }


    //Encerramento de threads
    for(int j = 0; j < threadsNumber; j++) {
        pthread_join(systemTID[j], NULL);
    }

    printf("\nFim da execução do programa principal.\n\n\n");

    return 0;
}
