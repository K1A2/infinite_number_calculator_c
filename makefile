main.out: main.c input.o error.o calculator.o stack.o number.o
	gcc -o main.out main.c input.o error.o calculator.o stack.o number.o
	make clear

calculator.o: include/calculator.c include/calculator.h
	gcc -c -o calculator.o include/calculator.c

input.o: include/input.c include/input.h
	gcc -c -o input.o include/input.c

stack.o: include/express/stack.c include/express/stack.h
	gcc -c -o stack.o include/express/stack.c

error.o: include/error.c include/error.h
	gcc -c -o error.o include/error.c

number.o: include/express/number.c include/express/number.h
	gcc -c -o number.o include/express/number.c

clear:
	rm *.o