# LeitoresEscritosSemInanicao
Segundo Trabalho de Implementação da disciplina de Computação Concorrente - 2019/2, lecionada pela professora Silvana Rossetto.
<br>Alunos: Daniel La Rubia e Paula Macedo.

<b>DESCRIÇÃO TRABALHO</b>

O trabalho aborda o problema dos leitores/escritores, um problema clássico de concorrência. 
<br>É esperada a implementação de um programa que respeite as seguintes condições lógicas:
<br>• mais de um leitor pode ler ao mesmo tempo;
<br>• apenas um escritor pode escrever de cada vez;
<br>• não é  permitido ler e escrever ao mesmo tempo.
<br>A implementação deve garantir a ausência de inanição entre as threads.

E implementação de um programa auxiliar que checa a execuação do programa principal, avaliando se a mesma foi realizada com sucesso.

<b>DIVISÃO DO PROJETO</b> 

Programa Principal (main.c) -> Implementado em C, realiza as execuções das threads leitoras e escritoras respeitando as condições lógicas e outras restrições solicitadas pelo trabalho.
<br>Programa Auxiliar (checkOperations.py) -> Implementado em Python, tem como entrada o log gerado pelo programa principal. Responsável por checar se a execução do programa principal foi bem sucedida ou não.
<br>Makefile -> Instrui como compilar e linkar os programas. 


<b>COMO EXECUTAR O PROGRAMA</b>

<br>1)Para compilar digite no terminal
<br>$cd LeitoresEscritosSemInanicao
<br>$make
 
<br>2)Para executar
<br>./main
 
