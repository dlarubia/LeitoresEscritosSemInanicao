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
