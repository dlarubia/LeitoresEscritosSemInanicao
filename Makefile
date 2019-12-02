all:
	gcc main.c -o main -Wall -lpthread -lm

clean:
	rm main
