
# estruturas de controle das threads
thread = []
threadParada = []

# quantidade de threads
threadsLeitoras = 0
threadsEscritoras = 0

# identificadores de operação
ESPERA = 0
ESCRITA = 1
LEITURA = 2

# quanti
qnt_leituras = 0
qnt_escritas = 0
leiturasEsperadas = 0
escritasEsperadas = 0
nthreads = 0


def inicializaEstruturas(n_rd, n_wt, n_r, n_w):
    global threadsLeitoras
    global threadsEscritoras
    global leiturasEsperadas
    global escritasEsperadas
    global thread
    global nthreads 
    global threadParada
    
    
    threadsLeitoras = n_rd
    threadsEscritoras = n_wt
    nthreads = n_rd + n_wt

    leiturasEsperadas = n_r
    escritasEsperadas = n_w

    for x in range (nthreads):
        thread.append(0)
        threadParada.append(0)


def quantificadorDeOperacoes(id, operacao):
    global thread, qnt_escritas, qnt_leituras, threadParada
    

    if(operacao == ESCRITA):
        qnt_escritas += 1        
        thread[id] += 1 
    
    elif(operacao == LEITURA):
        qnt_leituras += 1
        thread[id] += 1 

    elif(operacao == ESPERA):
        threadParada[id] += 1


    
def verificaCorretude(): 
    if(leiturasEsperadas != qnt_leituras):
        print("Não foi possível garantir a integridade de operações. \nLeituras detectadas: " + str(qnt_leituras) + "\nLeituras esperadas: " + str(leiturasEsperadas))
        exit()

    elif(escritasEsperadas != qnt_escritas):
        print("Não foi possível garantir a integridade de operações. \nEscritas detectadas: " + str(qnt_escritas) + "\nEscritas esperadas: " + str(escritasEsperadas))
        exit()
    
    for i in range (nthreads):
        if(i < threadsEscritoras):
            porcentagemOperacoes = (thread[i] * 100) / qnt_escritas
            print("A thread escritora " + str(i) + " realizou " + str(round(porcentagemOperacoes,2)) + "% das escritas. Aguardou permissão " + str(threadParada[i]) + " vezes no total.")
        
        elif(i < threadsEscritoras + threadsLeitoras): 
            porcentagemOperacoes = (thread[i] * 100) / qnt_leituras
            print("A thread leitora " + str(i) + " realizou " + str(round(porcentagemOperacoes,2)) + "% das leituras. Aguardou permissão " + str(threadParada[i]) + " vezes no total.")


def imprimeStatus():
    qnt_permissao = 0
    for i in range (nthreads):
        qnt_permissao += threadParada[i]

    print("\nQuantidade de escritas esperadas: " + str(escritasEsperadas))
    print("Quantidade de escritas feitas: " + str(qnt_escritas))
    print("Quantidade de leituras esperadas: " + str(leiturasEsperadas))
    print("Quantidade de leituras feitas: " + str(qnt_leituras))
    print("\nQuantidade de threads escritoras utilizadas: " + str(threadsEscritoras))
    print("Quantidade de threads leitoras utilizadas: " + str(threadsLeitoras))
    print("Quantidade de vezes em que foi utilizada técnica de sincronização (aguardando permissão): " + str(qnt_permissao))

def main():

    f = open("log.txt", "r")
    contents = f.readlines()

    for x in contents:
        exec(x)
    
    verificaCorretude()
    imprimeStatus()

    print("O programa foi executado corretamente. \nExecução encerrada.\n")

if __name__ == "__main__":
    main()