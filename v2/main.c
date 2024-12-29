#include "graph.h"
#include "func.h"

int main(int argc, char **argv)
{
	srand(time(NULL));

	int mode = argc > 1 ? atoi(argv[1]) : 2;
	int size, in, out;

	int correct_mode = rand_values(mode, argc, argv, &size, &in, &out);

	if(correct_mode != 0)
	{
		printf("Wybrano nieproprawny tryb generowania labiryntu\n");
		return 0;
	}

	int n = size*size;

	node_t *nodes = malloc(n * sizeof nodes);
	init_node(nodes, size, out, 0);
	correct_possible_connection(nodes, size, out);
	connect_graph(nodes, nodes[in-1], size, in, &mode);
	
	for(int i = 0; i < n; i++)
	{
		if(!nodes[i]->checked)
		{
			free_graph(nodes, n);
			init_node(nodes, size, out, 0);
			correct_possible_connection(nodes, size, out);
			connect_graph(nodes, nodes[in-1], size, in, &mode);
			i = -1;
		}	
	}
	
	print_status(nodes, size, in);
	print_maze(nodes, size, in);
	printf("Rozmiar labiryntu: %dx%d\n", size, size);
	free_graph(nodes, n);
	free(nodes);
		
}
