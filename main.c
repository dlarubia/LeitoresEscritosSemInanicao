#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

/*

int variavelCompartilhada = 0;
int statusEscrita = 0;
int lidos = 0;
//statusVariavel vai funcionar como um detector se a thread está sendo escrita ou não. Pode ser desnecessário
int statusVariavel = 0;
pthread_mutex_t acessoVariavel;
pthread_cond_t acessoEscrita, liberaLeitoras;
sem_t controleEntreThreads, controleLeitoras;
*/

// ########################################

// controle de threads escritoras realizando escrita
sem_t writeSemaphore;
pthread_mutex_t mutex_r, mutex_w, status;

//decidir se será utilizado semáforo ou variável de condição
pthread_cond_t permissionToWrite, permissionToRead;
sem_t readers;
sem_t writers;


// boolean ==> threads leitoras não podem ler enquanto está ocorrendo escrita
int writeStatus = 0;

int threadsReading = 0;

// quantidade de leituras e escritas será efetuada
int readings = 1000000000;
int writings = 1000000000;
int sharedVariable = -1;



void *writer(void *id) {
    int tid = *(int *) id;

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
        sem_post(&writeSemaphore);

        pthread_mutex_lock(&mutex_w);
    }
    pthread_mutex_unlock(&mutex_w);

}


void *reader(void *id) {
    int tid = *(int *) id;

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


        pthread_mutex_lock(&mutex_r);
    }
    pthread_mutex_unlock(&mutex_r);



}

/*
void escritor(void *tid){
    int tid = *(int*) tid;

    // semáforo binário para exclusão mútua ==> 
    sem_wait(&controleEntreThreads);

    //lock variavel ==> statusEscrita
    pthread_mutex_lock(&acessoVariavel);

    if(statusEscrita){
        printf("Thread escritora %d: bloqueada...\n", tid);
        pthread_cond_wait(&acessoEscrita, &acessoVariavel);
    }

    //variavel que define a quantidade de escritas que já ocorreram, funciona como uma condicional (escritas == 0, fim da execução)
    statusEscrita++;

    //variavel compartilhada ==> recebe o ID da thread
    variavelCompartilhada = tid;
    printf("Thread escritora %d rodou e variavel = %d\n", variavelCompartilhada, tid);

    //libera o semáforo (++)
    sem_post(&controleLeitoras);

    //unlock variavel ==> statusEscrita
    pthread_mutex_unlock(&acessoVariavel);

    // talvez um erro lógico aqui * * * * * * * * *
    statusEscrita--;


}



void leitor(void *tid){
    int tid = *(int *) tid;

    // aguarda liberação por parte da thread ESCRITORA
    pthread_cond_wait(&liberaLeitoras, &acessoVariavel);

    // semáforo para controle de threads leitoras? 
    sem_wait(&controleLeitoras);
    printf("Thread leitora %d: fazendo leitura... valor leitura %d\n", tid, variavelCompartilhada);
    lidos++;
    if(lidos == 1) {
            pthread_cond_broadcast(&liberaLeitoras);

    }
    sem_post(&controleEntreThreads);
}

*/

int main (int argc, char *argv[]) {

    int readersNumber = 5;
    int writersNumber = 5;
    int threadsNumber = readersNumber + writersNumber;
    pthread_t *systemTID;
    int *tid;


    systemTID = (pthread_t *) malloc(sizeof(pthread_t) * threadsNumber);
    if(systemTID == NULL) {
        printf("--ERROR: malloc()\n");
        exit(-1);
    }

    
    for(int id = 0; id < threadsNumber; id++) {
        tid = malloc(sizeof(int));
        if(tid == NULL) {
            printf("--ERRO: malloc()\n");
            exit(-1);
        }

        *tid = id;
        if(pthread_create(&systemTID[id], NULL, writer, (void *) tid)) {
            printf("--ERRO: pthread_create( writers )\n");
            exit(-1);
        }

        int reader_id = id + writersNumber;
        *tid = id + writersNumber;
        if(pthread_create(&systemTID[reader_id], NULL, reader, (void *) tid)) {
            printf("--ERRO: pthread_create( reader )\n");
            exit(-1);
        }
    }

    /*
    int numThreadsLeitoras = 6;
	int numThreadsEscritoras = 10;
	int numLeitura = 0;
	int numEscrita = 0;
    int totalThreads = numThreadsEscritoras + numThreadsLeitoras;
    pthread_t tid[totalThreads];
    int *id[totalThreads], i;

    sem_init(&controleEntreThreads, 0, 1);
    sem_init(&controleLeitoras, 0, 1);
 

    //Leitura entrada padrão
	printf("Entre com o numero de threads escritoras\n");
	scanf("%d", &numThreadsEscritoras);
	printf("Entre com o numero de threads leitoras\n");
	scanf("%d", &numThreadsLeitoras);
	printf("Entre com o numero de escritas\n");
	scanf("%d", &numEscrita);
	printf("Entre com o numero de leituras\n");
	scanf("%d", &numLeitura);
    



    for(i = 0; i<totalThreads; i++) {
        if((id[i] = malloc(sizeof(int))) == NULL) {
            pthread_exit(NULL);
            return 1;
        }
        *id[i] = i+1;
    }
    for(i = 0; i < numThreadsEscritoras; i++) {
        if(pthread_create(&tid[i+ numThreadsLeitoras], NULL, escritor, (void *)id[i])){
            exit(-1);
        }
    }
    for(i = 0; i < numThreadsLeitoras; i++) {
        if(pthread_create(&tid[i], NULL, leitor, (void *)id[i])){
            exit(-1);
        }
    }


    for (i=0; i<totalThreads; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }

*/
    return 0;
}
