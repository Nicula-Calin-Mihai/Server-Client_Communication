CC=g++
CFLAGS=-I.

client: main.cpp requests.cpp helpers.cpp buffer.cpp
	$(CC) -o main main.cpp requests.cpp helpers.cpp buffer.cpp -Wall

run: client
	./main

clean:
	rm -f *.o main
