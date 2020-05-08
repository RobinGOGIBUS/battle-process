all: battle

battle: main.o
	gcc -o battle main.o

main.o: main.c struct_members.h
	gcc -o main.o -c main.c 

clean:
	rm -rf *.o

mrproper: clean
	rm -rf battle