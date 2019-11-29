#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int variavelCompartilhada = 0;
pthread_mutex_t acessoVariavel;
pthread_cond_t acessoEscrita, liberaLeitoras;
int statusEscrita = 0;
sem_t controleEntreThreads, controleLeitoras;
int lidos=0;

void escritor(void *threadId){

    int tid = *(int*) threadId;
    sem_wait(&controleEntreThreads);
    pthread_mutex_lock(&acessoVariavel);
    if(statusEscrita == 1){
        printf("Thread escritora %d: bloqueada...\n", tid);
        pthread_cond_wait(&acessoEscrita, &acessoVariavel);
    }
    statusEscrita = 1;
    variavelCompartilhada = tid;
    printf("Thread escritora %d rodou e variavel = %d, numEscrita = %d\n", variavelCompartilhada, tid);
    sem_post(&controleLeitoras);

    pthread_mutex_unlock(&acessoVariavel);
    statusEscrita = 0;


}


void leitor(void *threadId){
    int tid = *(int*) threadId;
    pthread_cond_wait(&liberaLeitoras, &acessoVariavel);
    sem_wait(&controleLeitoras);
    printf("Thread leitora %d: fazendo leitura... valor leitura %d\n", tid);
    pthread_cond_broadcast(&liberaLeitoras);
    sem_post(&controleEntreThreads);
}

int main (int argc, char *argv[]){

    int numThreadsLeitoras = 9 ;
	int numThreadsEscritoras = 3;
	int numLeitura = 1;
	int numEscrita = 1;
    int totalThreads = numThreadsEscritoras + numThreadsLeitoras;
    pthread_t tid[totalThreads];
    int *id[totalThreads], i;

    sem_init(&controleEntreThreads, 0, 1);
    sem_init(&controleLeitoras, 0, 1);
	/**
    //Leitura entrada padrão
	printf("Entre com o numero de threads escritoras\n");
	scanf("%d", &numThreadsEscritoras);
	printf("Entre com o numero de threads leitoras\n");
	scanf("%d", &numThreadsLeitoras);
	printf("Entre com o numero de escritas\n");
	scanf("%d", &numEscrita);
	printf("Entre com o numero de leituras\n");
	scanf("%d", &numLeitura);
    **/



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

    return 0;
}
