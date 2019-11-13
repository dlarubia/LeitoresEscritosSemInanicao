import java.util.Arrays;

class Buffer {
    /*
    int[] vetor = new int[10];
    ou
    int vetor[];
    vetor = new int[10]
    */
    private int buffer[];
    private int tamanhoBuffer;
    private int in = 0, out = 0, count = 0;

    public Buffer(int tamanhoBuffer) {
        this.tamanhoBuffer = tamanhoBuffer;
        this.buffer = new int[this.tamanhoBuffer];

        for(int i = 0; i < tamanhoBuffer; i++) {
            buffer[i] = 0;
        }
    }


    public synchronized void insere(int id, int item) {
        try {
            while(count == tamanhoBuffer) {
                System.out.println("\nThread " + id + " tentou realizar inserção com o buffer cheio.");
                wait();
            }
            count++;
            buffer[in] = item;
            in = (in + 1) % tamanhoBuffer;
            System.out.println("\nO valor " + item + " foi inserido no buffer.");
            System.out.println(Arrays.toString(buffer));
            notifyAll();
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }

    public synchronized void remove(int id) {
        try {
            while(count == 0) {
                System.out.println("\nThread " + id + " tentou realizar remoção com o buffer vazio.");
                wait();
            }
            count--;
            System.out.println("\nA thread " + id + " realizou a remoção do item " + buffer[out]);
            buffer[out] = 0;
            out = (out + 1) % tamanhoBuffer;
            System.out.println(Arrays.toString(buffer));
            notify();
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}