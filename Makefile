all:
	gcc main.c -o main -Wall -lpthread -lm && ./main && python3 checkOperations.py

clean:
	rm main