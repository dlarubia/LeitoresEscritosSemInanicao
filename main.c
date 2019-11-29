#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int variavelCompartilhada = 0;
pthread_mutex_t acessoVariavel;

void escritor(void *threadId){
    int tid = *(int*) threadId;

    pthread_mutex_lock(&acessoVariavel);
    variavelCompartilhada = tid;

    printf("%d %d\n", variavelCompartilhada, tid);

    pthread_mutex_unlock(&acessoVariavel);

}


void leitor(void *threadId){
    int tid = *(int*) threadId;
    printf("%d\n", tid);
}

int main (int argc, char *argv[]){

    int numThreadsLeitoras = 6;
	int numThreadsEscritoras = 10;
	int numLeitura = 0;
	int numEscrita = 0;
    int totalThreads = numThreadsEscritoras + numThreadsLeitoras;
    pthread_t tid[totalThreads];
    int *id[totalThreads], i;

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
    for(i = 0; i < numThreadsLeitoras; i++) {
        if(pthread_create(&tid[i], NULL, leitor, (void *)id[i])){
            exit(-1);
        }
    }
      for(i = 0; i < numThreadsEscritoras; i++) {
        if(pthread_create(&tid[i+ numThreadsLeitoras], NULL, escritor, (void *)id[i])){
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
