CFLAGS = -Wall -Wextra -std=c99
TARGET = labirynt

labirynt: graph.c main.c
	$(CC) $(CFLAGS) graph.c main.c -o labirynt

gdb: graph.c main.c
	$(CC) $(CFLAGS) -ggdb graph.c main.c -o labirynt

valgrind: graph.c main.c
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(TARGET)

clean: rm -f labirynt

test:
	./$(TARGET)
