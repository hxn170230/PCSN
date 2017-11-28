simulator: main.o utils.o
	gcc -o simulator main.o utils.o

main.o: main.c utils.h
	gcc -c -Wall main.c -I.

utils.o: utils.c utils.h
	gcc -c -Wall utils.c -I.

clean:
	rm -rf main.o utils.o simulator
