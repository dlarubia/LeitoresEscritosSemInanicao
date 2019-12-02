all:
	gcc main.c -o main -Wall -lpthread -lm && ./main

clean:
	rm main