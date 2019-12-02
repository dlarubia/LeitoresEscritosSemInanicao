thread[10] = []
ESCRITA = 0
LEITURA = 1
ESPERA_P_ESCREVER = 2
ESPERA_P_LER = 3

qnt_leituras = 0
qnt_escritas = 0
qnt_espera_p_escrever = 0
qnt_espera_p_ler = 0

leiturasEsperadas = 10
escritasEsperadas = 10
nthreads= 10

def quantificadorDeOperacoes(tid, operacao, valor):
    thread[tid] = 1

    if(operacao == ESCRITA):
        qnt_escritas += 1        
    
    elif(operacao == LEITURA):
        qnt_leituras += 1

    elif(operacao == ESPERA_P_ESCREVER):
        qnt_espera_p_escrever += 1

    elif(operacao == ESPERA_P_LER):
        qnt_espera_p_ler += 1
    
def verificaCorretude(): 
    if(leiturasEsperadas != qnt_leituras):
        print("Não foi possível garantir a integridade de operações. \nLeituras detectadas: " + qnt_leituras + "\nLeituras esperadas: " + leiturasEsperadas)
        return 0

    elif(escritasEsperadas != qnt_escritas):
        print("Não foi possível garantir a integridade de operações. \nEscritas detectadas: " + qnt_leituras + "\nEscritas esperadas: " + leiturasEsperadas)
        return 0
    
    for i in range (nthreads):
        porcentagemOperacoes = thread[tid] / 
