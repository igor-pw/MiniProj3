CFLAGS = -Wall -Wextra -std=c99
TARGET = labirynt

labirynt: search.c func.c graph.c main.c
	$(CC) $(CFLAGS) search.c func.c graph.c main.c -o labirynt

gdb: search.c func.c graph.c main.c
	$(CC) $(CFLAGS) -g search.c func.c graph.c main.c -o labirynt

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(TARGET)

clean: rm -f labirynt

test:
	./$(TARGET)
