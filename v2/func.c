#include "graph.h"
#include "func.h"

void connect_node(node_t *nodes, node_t node, int value, bool *direction, bool *opposite_direction, int *mode)
{	
	int to_connect;

	if(!nodes[node->nr+value]->checked)
	{
		if(nodes[node->nr+value]->possible_connection <= *mode && !nodes[node->nr+value]->connected)
		{
			*opposite_direction = false;
			nodes[node->nr+value]->connected = true;
			node->edge++;
		}

		else if(nodes[node->nr+value]->connected)
		{
			nodes[node->nr+value]->possible_connection--;
			*direction = false;

		}
			
		else
		{
			to_connect = rand() % 2;

			if(to_connect == 0)
			{
				nodes[node->nr+value]->possible_connection--;
				*direction = false;
			}

			else if(to_connect == 1)
			{
				*opposite_direction = false;
				nodes[node->nr+value]->connected = true;
				node->edge++;
			}
		}
	}

	else if(nodes[node->nr+value]->checked)
	{
		nodes[node->nr+value]->possible_connection--;
		*direction = false;
	}
}

int rand_values(int mode, int argc, char **argv, int *size, int *in, int *out)
{
	if(mode == 1)
		*size = argc > 2 ? atoi(argv[2]) : 2 + rand() % 12;

	else if(mode == 2)
		*size = argc > 2 ? atoi(argv[2]) : 14 + rand() % (29-14+1);
	
	else if(mode == 3)
		*size = argc > 2 ? atoi(argv[2]) : 30 + rand() % (361-30+1);

	else
		return 1;

	int value_of_size = *size;
	int n = value_of_size*value_of_size;
			
	*in = argc > 3 ? atoi(argv[3]) : 1 + rand() % value_of_size;		
	*out = argc > 4 ? atoi(argv[4]) : n - rand() % value_of_size;
		
	while(*out % value_of_size == *in % value_of_size)
		*out = n - rand() % value_of_size;

	return 0;
}	
