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

