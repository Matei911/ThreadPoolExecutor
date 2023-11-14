CC = gcc
CFLAGS = -Wall -g

build: main.o stiva.o coada.o scheduler.o
	$(CC) -o tema2 $^

main.o: main.c  
	$(CC) $(CFLAGS) -c $^
stiva.o: stiva.c
	$(CC) $(CFLAGS) -c $^
coada.o: coada.c
	$(CC) $(CFLAGS) -c $^
scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c $^
run: build
		valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
	./tema2 in_file out_file
clean:
	rm -f *.o tema2
	rm -f valgrind-out.txt
.PHONY: clean run build