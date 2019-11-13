public class Start {
    static int tamanhoBuffer = 20;
    static int n_escritoras = 5;
    static int n_leitoras = 4;
    static int delay = 1000;
    public static void main (String[] args) {
        Buffer buffer = new Buffer(tamanhoBuffer);
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