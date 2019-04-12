demo: demo.o stack.o
	gcc -o demo demo.o stack.o

demo.o: demo.c stack.h
	gcc -c demo.c

stack.o: stack.c stack.h
	gcc -c stack.c
