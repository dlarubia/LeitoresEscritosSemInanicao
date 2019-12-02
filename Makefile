all:
	gcc main.c -o main -Wall -lpthread && ./main && python3 checkOperations.py

clean:
	rm main
