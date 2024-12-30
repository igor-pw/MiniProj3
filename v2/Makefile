CFLAGS = -Wall -Wextra -std=c99
TARGET = labirynt

labirynt: func.c graph.c main.c
	$(CC) $(CFLAGS) func.c graph.c main.c -o labirynt

gdb: func.c graph.c main.c
	$(CC) $(CFLAGS) -g func.c graph.c main.c -o labirynt

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(TARGET)

clean: rm -f labirynt

test:
	./$(TARGET)
