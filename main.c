#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "graph.h"
#include "func.h"

#define TIME 15 
#define MAX_SIZE 361
#define MIN_SIZE 3

int main(int argc, char **argv)
{
	srand(time(NULL));
	system("clear");

	int size, in, out, mode;

	size = argc > 1 ? check_value(argv, MIN_SIZE, MAX_SIZE, 0) : MIN_SIZE + rand() % MAX_SIZE;
	in = argc > 2 ? check_value(argv, 1, size, 1) : 1 + rand() % size;
	out = argc > 3 ? check_value(argv, 1, size, 2) : 1 + rand() % size; 
	mode = argc > 4 ? atoi(argv[4]) : select_mode(size, MAX_SIZE);

	out += size*size - size;

	check_mode(&mode, size, MAX_SIZE);

	int n = size*size;

	printf("\nGenerowanie labiryntu...\n");

	node_t *nodes = malloc(n * sizeof nodes);
	init_node(nodes, size, out, 0);
	correct_possible_connection(nodes, size, out);
	connect_graph(nodes, nodes[in-1], size, in, mode);

	pthread_t timer_thread;
	int duration = 0;

	bool first = true;

	for(int i = 0; i < n; i++)
	{
		if(!nodes[i]->checked)
		{	
			free_graph(nodes, n);

			if(duration == 0)
			{
				int choice = 1;

				if(!first)
				{
					pthread_join(timer_thread, NULL);
					choice = change_mode(size, &mode, MAX_SIZE);
					printf("\nGenerowanie labiryntu...\n");
				}

				if(first)
				{
					first = false;
					duration = TIME;
					pthread_create(&timer_thread, NULL, timer, &duration);
				}


				if(choice == 0)
				{	
					duration = TIME;
					pthread_create(&timer_thread, NULL, timer, &duration);
				}
			}

			init_node(nodes, size, out, 0);
			correct_possible_connection(nodes, size, out);
			connect_graph(nodes, nodes[in-1], size, in, mode);
			i = -1;
		}	
	}
	

	print_maze(nodes, size, in);
	printf("Rozmiar labiryntu: %dx%d\n", size, size);
	save_status(nodes, size);	

	free_graph(nodes, n);
	free(nodes);

	pthread_cancel(timer_thread);	
	pthread_join(timer_thread, NULL);
}
