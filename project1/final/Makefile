# Harshavardhan Nalajala
# hxn170230
q_simulator: main.o utils.o
	gcc -o q_simulator main.o utils.o

main.o: main.c utils.h
	gcc -c -Wall main.c -I.

utils.o: utils.c utils.h
	gcc -c -Wall utils.c -I.


clean:
	rm -rf q_simulator
	rm -rf main.o
	rm -rf utils.o
