#include "graph.h"

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	int size = argc > 1 ? atoi(argv[1]) : 3;
	int in = argc > 2 ? atoi(argv[2]) : 1 + rand() % size;
	int out = argc > 3 ? atoi(argv[3]) : size*size - rand() % size;
	int n = size*size;

	node_t *nodes = malloc(n * sizeof nodes);
	init_node(nodes, size, out, 0);
	correct_possible_connection(nodes, size, out);
	connect_graph(nodes, nodes[in-1], size, in);
	
	for(int i = 0; i < n; i++)
	{
		if(!nodes[i]->checked)
		{
			free_graph(nodes, n);
			init_node(nodes, size, out, 0);
			correct_possible_connection(nodes, size, out);
			connect_graph(nodes, nodes[in-1], size, in);
			i = -1;
		}	
	}
	
	print_status(nodes, size);
	print_maze(nodes, size, in);
	free_graph(nodes, n);
	free(nodes);
		
}
