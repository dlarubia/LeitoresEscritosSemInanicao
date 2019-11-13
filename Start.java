import java.util.Arrays;

class Buffer {
    private int[] buffer;
    private int tamanhoBuffer;
    private int in = 0, out = 0, count = 0;

    public Buffer(int n) {
        this.tamanhoBuffer = n;
        this.buffer = new int[this.tamanhoBuffer];

        for(int i = 0; i < tamanhoBuffer; i++) {
            buffer[i] = 0;
        }
    }


    public synchronized void insere(int id, int item) {
        try {
            while(count == tamanhoBuffer) {
                wait();
                System.out.println("Thread " + id + " tentou realizar inserção com o buffer cheio.");
            }
            count++;
            buffer[in] = item;
            in = (in + 1) % tamanhoBuffer;
            System.out.println("O valor " + item + " foi inserido no buffer.");
            System.out.println(Arrays.toString(buffer));
            notifyAll();
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }

    public synchronized void remove(int id) {
        try {
            while(count == 0) {
                wait();
                System.out.println("Thread " + id + " tentou realizar remoção com o buffer vazio.");
            }
            count--;
            System.out.println("A thread " + id + " realizou a remoção do item " + buffer[out]);
            buffer[out] = 0;
            System.out.println(Arrays.toString(buffer));
            notify();
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}


class Escritor {
    Buffer buffer;
    int id, delay, item;

    public Escritor(int thread_id, Buffer buffer, int delay) {
        this.id = thread_id;
        this.buffer = buffer;
        this.delay = delay;

        geraItem(id);
    }

    public void geraItem(int n) {
        this.item = n;
    }
    
    public void run(){
        try{ 
            this.buffer.insere(id, item);
            sleep(delay);
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}


class Consumidor {
    Buffer buffer;
    int id, delay, item;

    public Consumidor(int thread_id, Buffer buffer, int delay) {
        this.id = thread_id;
        this.buffer = buffer;
        this.delay = delay;
    }

    public void consumeItem(int n) {
        this.item = n;
    }

    public void run() {
        try  {
            this.buffer.remove(id);
            sleep(delay);
        } catch (InterruptedException e) {
            System.out.println(e);
        }
    }
}