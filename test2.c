#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "graph2.h"

int main(int argc, char **argv)
{
	srand(time(NULL));

	int size = argc > 1 ? atoi(argv[1]) : 4;
	int in = argc > 2 ? atoi(argv[2]) : size;
	int out = argc > 3 ? atoi (argv[3]) : size*size;
	int counter = 0;

	node_t *temp;
	temp = init_graph(size, in, out);
	//print_graph(temp, size, in, out);

	rand_graph(temp[in-1], size, in, out);
	//print_graph(temp, size, in, out);


	//connect_graph(temp, size, in, out);
	check_graph(temp[in-1]);
	
	for(int i = 0; i < size*size; i++)
	{
		if(!temp[i]->checked)
		{
			free_graph(temp, size);
			i = 0;
			temp = init_graph(size, in, out);
			rand_graph(temp[in-1], size, in, out);
			//connect_graph(temp, size, in, out);
			check_graph(temp[in-1]);
		}
	}

	print_graph(temp, size, in, out);

	free_graph(temp, size);
}
