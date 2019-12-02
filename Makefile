all:
	gcc main.c -o main -Wall -lpthread && python3 checkOperations.py

clean:
	rm main
