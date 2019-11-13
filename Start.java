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


class Escritor extends Thread {
    Buffer buffer;
    int id, delay, item;

    public Escritor(int thread_id, Buffer buffer, int delay) {
        this.id = thread_id;
        this.buffer = buffer;
        this.delay = delay;

        geraItem(id);
    }

    public void geraItem(int n) {
        this.item = n+1;
    }
    
    public void run(){
        try{ 
            for(;;) {
                this.buffer.insere(id, item);
                sleep(delay);
            }
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}


class Leitor extends Thread {
    Buffer buffer;
    int id, delay, item;

    public Leitor(int thread_id, Buffer buffer, int delay) {
        this.id = thread_id;
        this.buffer = buffer;
        this.delay = delay;
    }

    public void leItem(int n) {
        this.item = n;
    }

    public void run() {
        try  {
            for(;;) {
                this.buffer.remove(id);
                sleep(delay);
            }
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}

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