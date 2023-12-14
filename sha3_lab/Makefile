all: 
	make sha3_hex_in
	make sha3

sha3_hex_in: sha3.o sha3_hex.o
	gcc bin/sha3.o bin/sha3_hex.o -o sha3_hex

sha3: sha3.o main.o
	gcc bin/sha3.o bin/main.o -o sha3

test: sha3.o test.o
	gcc bin/sha3.o bin/test.o -o test

sha3.o: sha3.c
	gcc -c sha3.c -o bin/sha3.o

main.o: main.c
	gcc -c main.c -o bin/main.o

sha3_hex.o: sha3_hex.c
	gcc -c sha3_hex.c -o bin/sha3_hex.o

test.o: test.c
	gcc -c test.c -o bin/test.o 

clean:
	rm sha3.o main.o bin/*