/**
	Trabalho 2: Leitores e escritores sem inanição
	Descrição:
	Autores: Paula Macedo e Daniel La Rubia
	Data:
**/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


int variavelUnica = 0;// variável que as threads poderam ler ou modificar seu valor
sem_t controle;//semaforo para controlar a ordem execução thread escritora e thread leitora
mutex_t acessoVariavel; //permite que uma thread leitora altere por vez o valor da variávelUnica 

void escreve (FILE *arquivo, int tipo){}

void leitor(){
	
	//wait controle
	int id = (void) tid;
	FILE *arquivo;
	arquivo = fopen( id.txt, a);
	


}

void escritor( void *tid){
		
	int id = (void) tid;
	
	//wait controle
	//pthread_mutex_lock(acessoVariavel);
	variavelUnica = id;
	// escreve no arquivo de log
	// thread %i, id escreveu 
	//Vê a lógica implementada pra saber 
	//post controle e deixa 
	//pthread_mutex_unlock(acessoVaraiavel)
	
}


int main(int argc, char *argv[]){
	

	int numThreadsLeitoras;
	int numThreadsEscritoras;
	int numLeitura;
	int numEscrita;

	printf("Entre com o numero de threads escritoras\n");
	scanf("%d", &numThreadsEscritoras);
	printf("Entre com o numero de threads leitoras\n");
	scanf("%d", &numThreadsLeitoras);
	printf("Entre com o numero de escritas\n");
	scanf("%d", &numEscrita);
	printf("Entre com o numero de leituras\n");
	scanf("%d", &numLeitura);


	//Inicializa semaforo de controle de fluxo
	sem_init(&scontrole, 0, 0);
    
    //Acertar isso aqui
	pthread_t tid[];
    int i, *id;

    //Alocar espaço para as threads

  
  

    for(i = 0; i < numThreadsLeitoras; i++) {
        if(pthread_create(&tid[i], NULL, leitor, (void *)id[i])){
            exit(-1);
        }
    }
      for(i = 0; i < numThreadsEscritoras; i++) {
        if(pthread_create(&tid[i], NULL, escritor, (void *)id[i])){
            exit(-1);
        }
    }


    //int pthread_join (pthread_t thread, void **thread_return);

    
	


	return 0;
}
