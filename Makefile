run:
	gcc -o main main.c  -lm; ./main
gc: 
	clear;	gcc -o main main.c -lm
clean:
	rm main.c;
