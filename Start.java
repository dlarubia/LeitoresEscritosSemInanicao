import java.util.Scanner;

public class Start {
    static int tamanhoBuffer = 20;
    static int n_escritoras = 5;
    static int n_leitoras = 4;
    static int delay = 1000;
    static int qtd_leituras, qnt_escritas;
    static String arquivo;
    static Scanner scanner = new Scanner(System.in);
    public static void main (String[] args) {
        Buffer buffer = new Buffer(tamanhoBuffer);
/*
        System.out.println("Digite o nome do arquivo de log: ");
        arquivo = scanner.nextline();
        System.out.println("Digite o número de threads leitoras: ");
        n_leitoras = scanner.nextInt();
        System.out.println("Digite o número de threads escritoras: ");
        n_escritoras = scanner.nextInt();
        System.out.println("Digite o número de leituras que serão efetuadas: ");
        qtd_leituras = scanner.nextInt();
        System.out.println("Digite o número de leituras que serão efetuadas: ");
        qtd_escritas = scanner.nextInt();
  */
        Escritor[] escritor = new Escritor[n_escritoras];
        Leitor[] leitor = new Leitor[n_leitoras];

        for(int i = 0; i < n_escritoras; i++) {
            escritor[i] = new Escritor(i, buffer, delay);
            escritor[i].start();
        }

        for(int i = 0; i < n_leitoras; i++) {
            leitor[i] = new Leitor(i, buffer, delay);
            leitor[i].start();
        }
    }
    
}