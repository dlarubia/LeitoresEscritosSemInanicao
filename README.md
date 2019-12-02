# LeitoresEscritosSemInanicao
Segundo Trabalho de Implementação da disciplina de Computação Concorrente - 2019/2, lecionada pela professora Silvana Rossetto.
Alunos: Daniel La Rubia e Paula Macedo.

DESCRIÇÃO TRABALHO

O trabalho aborda o problema dos leitores/escritores, um problema clássico de concorrência. É esperada a implementação de um programa que respeite as seguintes condições lógicas:
• mais de um leitor pode ler ao mesmo tempo;
• apenas um escritor pode escrever de cada vez;
• não é  permitido ler e escrever ao mesmo tempo.
A implementação deve garantir a ausência de inanição entre as threads.

E implementação de um programa auxiliar que checa a execuação do programa principal, avaliando se a mesma foi realizada com sucesso.

DIVISÃO DO PROJETO 

Programa Principal (main.c) -> Implementado em C, realiza as execuções das threads leitoras e escritoras respeitando as condições lógicas e outras restrições solicitadas pelo trabalho.
Programa Auxiliar (checkOperations.py) -> Implementado em Python, tem como entrada o log gerado pelo programa principal. Responsável por checar se a execução do programa principal foi bem sucedida ou não.
Makefile -> Instrui como compilar e linkar os programas. 


COMO EXECUTAR O PROGRAMA

PROGRAMA PRINCIPAL (MAIN.C)
1)Para compilar digite no terminal

 $cd LeitoresEscritosSemInanicao
 $make
 
2)Para executar

./main
 
