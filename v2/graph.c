#include "graph.h"
#include <unistd.h>

void print_status(node_t *nodes, int size)
{
	for(int i = 0; i < size*size; i++)
	{
		if(nodes[i]->checked)
			printf("| sprawdzone ");

		else
			printf("| niesprawdzone ");

		printf("| nr: %2d | krawedzie: %d | wierzcholki: ", nodes[i]->nr, nodes[i]->edge); 
	
		for(int j = 0; j < nodes[i]->edge; j++)
		{
			printf(" %2d", nodes[i]->next_node[j]->nr);
		}

		if(nodes[i]->up)
			printf(" | up");
		if(nodes[i]->down)
			printf(" | down");
		if(nodes[i]->left)
			printf(" | left");
		if(nodes[i]->right)
			printf(" | right");

		printf(" |\n");

	}

	printf("\n\n");
}

void print_maze(node_t *nodes, int size, int in)
{
	//int n = size*size;

	printf(" ");

	for(int i = 0; i < in-1; i++)
		printf("_ ");
	printf("  ");

	for(int i = in; i < size; i++)
	       printf("_ ");	
	
	printf("\n");

	for(int i = 0; i < size-1; i++)
	{
		printf("|");
		
		for(int j = 0; j < size-1; j++)
		{	

			if(!nodes[j]->down)
				{
					if(!nodes[j+size]->up)
						printf("_");

					else if(nodes[j+size]->up)
						printf(" ");
				}

			else if(nodes[j]->down)
				printf(" ");
			
			if(!nodes[j]->right)
			{
				if(!nodes[j+1]->left)
					printf("|");
				
				else if(nodes[j+size]->left)	
					printf(" ");
			}

			else if(nodes[j]->right)
				printf(" ");
		}


		if(!nodes[size*(i+1)]->down)
		{
			if(!nodes[size*(i+1)+size-1]->up)
				printf("_|\n");

			else if(nodes[size*(i+1)+size-1]->up)
				printf(" |\n");
		}

		else if(nodes[size*(i+1)]->down)
			printf(" |\n");
	
	}

	printf("|");

	for(int i = size*size-size; i < size*size-1; i++)
	{
		if(!nodes[i]->end) 
		{
			if(!nodes[i]->right)
			{
				if(!nodes[i+1]->left)
					printf("_|");

				else if(nodes[i+1]->left)
					printf("_ ");
			}

			else if(nodes[i]->left)
				printf("_ ");
		}

		else if(nodes[i]->end)
		{
			if(!nodes[i]->right)
			{
				if(!nodes[i+1]->left)
					printf(" |");

				else if(nodes[i+1]->left)
					printf("  ");

			}

                        else if(nodes[i]->right)
                                printf("  ");
		}
	}

	
	if(!nodes[size*size-1]->end)
		printf("_|\n\n");

	else if(nodes[size*size-1]->end)
		printf(" |\n\n");
}

void init_node(node_t *nodes, int size, int out, int index)
{
	int n = size*size;

	
	if(nodes == NULL)
	{
		nodes = malloc(n * sizeof nodes);

		if(nodes == NULL)
		{
			printf("Blad alokacji pamieci\n");
			return;
		}
	}

	node_t node = malloc(sizeof *node);

	node->nr = index+1;
	node->checked = false;
	node->edge = 0;
	node->end = false;
	
	if(index == out - 1)
		node->end = true;

	node->up = true;
	node->down = true;
	node->right = true;
	node->left = true;

	if((node->nr == 1) || node->nr == size || node->nr == ((size-1)*size+1) || node->nr == size*size)
	{
		node->possible_connection = 2;

		if(node->nr == 1)
		{
			node->up = false;
			node->left = false;
		}

		else if(node->nr == size)
		{
			node->up = false;
			node->right = false;
		}

		else if(node->nr == ((size-1)*size+1))
		{
			node->down = false;
			node->left = false;
		}

		else if(node->nr == size*size)
		{
			node->right = false;
			node->down = false;
		}
	}
	else if((node->nr > 1 && node->nr < size) || (node->nr > (size-1)*size+1 && node->nr < size*size) || (node->nr % size <= 1 && (node->nr != 1 || node->nr != (size-1)*size+1 || node->nr != size || node->nr != size*size)))
	{
		node->possible_connection = 3;

		if(node->nr > 1 && node->nr < size)
			node->up = false;

		else if(node->nr > (size-1)*size+1 && node->nr < size*size)
			node->down = false;

		else if(node->nr % size == 1)
			node->left = false;

		else if(node->nr % size == 0)
			node->right = false;
	}

	else
		node->possible_connection = 4;

	nodes[index] = node;	

	index++;

	if(index < n)
		init_node(nodes, size, out, index);

	else return;

}

void correct_possible_connection(node_t *nodes, int size, int out)
{
	if(nodes[out-1]->up)
		nodes[out-size-1]->possible_connection--;

	if(nodes[out-1]->left)
		nodes[out-2]->possible_connection--;

	if(nodes[out-1]->right)
		nodes[out]->possible_connection--;

	nodes[out-1]->up = false;
	nodes[out-1]->down = false;
	nodes[out-1]->right = false;
	nodes[out-1]->left = false;

	return;
}

void connect_graph(node_t *nodes, node_t node, int size, int in)
{
	node->checked = true;	
	int to_connect; //= node->possible_connection;
	
	if(node->up)
	{
		if(!nodes[node->nr-size-1]->checked)
		{
			if(nodes[node->nr-size-1]->possible_connection <= 3)
			{
				nodes[node->nr-size-1]->down = false;
				//nodes[node->nr-size-1]->checked = true;
				node->edge++;
			}

			else
			{
				to_connect = rand() % 2;

				if(to_connect == 0)
				{
					nodes[node->nr-size-1]->possible_connection--;
					node->up = false;
				}
		
					else if(to_connect == 1)
				{
					nodes[node->nr-size-1]->down = false;
					//nodes[node->nr-size-1]->checked = true;
					node->edge++;
				}
			}
		}
	}

	if(node->down)
	{
		if(!nodes[node->nr+size-1]->checked)
		{
		
			if(nodes[node->nr+size-1]->possible_connection <= 3)
			{
				nodes[node->nr+size-1]->up = false;
				//nodes[node->nr+size-1]->checked = true;
				node->edge++;
			}
			
			else
			{
				to_connect = rand() % 2;

				if(to_connect == 0)
				{
					nodes[node->nr+size-1]->possible_connection--;
					node->down = false;
				}
		
				else if(to_connect == 1)
				{
					nodes[node->nr+size-1]->up = false;
					//nodes[node->nr+size-1]->checked = true;
					node->edge++;
				}
			}
		}
	}

	if(node->left)
	{
		if(!nodes[node->nr-2]->checked)
		{
			if(nodes[node->nr-2]->possible_connection <= 3)
			{
				nodes[node->nr-2]->right = false;
				//nodes[node->nr-2]->checked = true;
				node->edge++;
			}
		
	
			else
			{
				to_connect = rand() % 2;

				if(to_connect == 0)
				{
					nodes[node->nr-2]->possible_connection--;
					node->left = false;	
				}
		
				else if(to_connect == 1)
				{
					nodes[node->nr-2]->right = false;
					//nodes[node->nr-2]->checked = true;
					node->edge++;
				}
			}
		}
	}

	if(node->right)
	{
		if(!nodes[node->nr]->checked)
		{
			if(nodes[node->nr]->possible_connection <= 3)
			{
				nodes[node->nr]->left = false;
				//nodes[node->nr]->checked = true;
				node->edge++;
			}
		
			else
			{	
				to_connect = rand() % 2;

				if(to_connect == 0)
				{
					nodes[node->nr]->possible_connection--;
					node->right = false;
				}
			
				else if(to_connect == 1)
				{
					nodes[node->nr]->left = false;
					//nodes[node->nr]->checked = true;
					node->edge++;
				}
			}
		}
	}

	//int counter = 0;
	if(node->edge > 0)
	{
		node->next_node = malloc(sizeof node * node->edge);
		int counter = 0;
	

		if(node->up && counter < node->edge)
		{
			node->next_node[counter] = nodes[node->nr-size-1];
			counter++;
		}

		if(node->down && counter < node->edge)
		{
			node->next_node[counter] = nodes[node->nr+size-1];
			counter++;
		}
	
		if(node->left && counter < node->edge)
		{
			node->next_node[counter] = nodes[node->nr-2];
			counter++;
		}

		if(node->right && counter < node->edge)
		{
			node->next_node[counter] = nodes[node->nr];
			counter++;
		}

	}

	//print_status(nodes, size);	

	if(node->edge > 0)
	{
		for(int i = 0; i < node->edge; i++)
		{
			if(!node->next_node[i]->checked)
				connect_graph(nodes, node->next_node[i], size, in); 
		}
	}

	else if(node->edge < 1 && node->nr == in)
	{
		node->down = true;
		
		if(node->nr == 1)
			node->right = true;

		else if(node->nr == size)
			node->left = true;
		
		else if(node->nr > 1 && node->nr < size)
		{
			node->left = true;
			node->right = true;
		}
	
		connect_graph(nodes, node, size, in);
	}
}


void free_graph(node_t *nodes, int n)
{
	for(int i = 0; i < n; i++)
	{
		if(nodes[i]->edge == 0)
			free(nodes[i]);

		else
		{
			free(nodes[i]->next_node);
			free(nodes[i]);
		}
	}

	//free(nodes);
}
