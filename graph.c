#include "graph.h"
#include "func.h"

void save_status(node_t *nodes, int size)
{
	FILE *out = fopen("status.txt", "w");
	
	for(int i = 0; i < size*size; i++)
	{
		fprintf(out, "| nr: %2d | waga: %.3g | polaczenia:", nodes[i]->nr, nodes[i]->wage); 
	
		if(nodes[i]->edge > 0)
			for(int j = 0; j < nodes[i]->edge; j++)
			{
				fprintf(out, " %2d", nodes[i]->next_node[j]->nr);
			}

		else if(nodes[i]->edge == 0)
			fprintf(out, " brak");

		fprintf(out, " |\n");	
	}

	fclose(out);
}

void print_maze(node_t *nodes, int size, int in)
{
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

			if(!nodes[j+i*size]->down)
				{
					if(!nodes[j+i*size+size]->up)
						printf("_");

					else if(nodes[j+i*size+size]->up)
						printf(" ");
				}

			else if(nodes[j+i*size]->down)
				printf(" ");
			
			if(!nodes[j+i*size]->right)
			{
				if(!nodes[j+i*size+1]->left)
					printf("|");
				
				else if(nodes[j+i*size+1]->left)	
					printf(" ");
			}

			else if(nodes[j+i*size]->right)
				printf(" ");
		}


		if(!nodes[size*(i+1)-1]->down)
		{
			if(!nodes[size*(i+1)+size-1]->up)
				printf("_|\n");

			else if(nodes[size*(i+1)+size-1]->up)
				printf(" |\n");
		}

		else if(nodes[size*(i+1)-1]->down)
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

			else if(nodes[i]->right)
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
	node->edge = 0;
	node->wage = generate_wage();	
	node->checked = false;
	node->end = false;
	node->connected = false;

	if(index == out - 1)
		node->end = true;

	node->up = true;
	node->down = true;
	node->right = true;
	node->left = true;
	
	correct_directions(node, size);

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

void connect_graph(node_t *nodes, node_t node, int size, int in, int mode)
{
	node->checked = true;	
	int value;

	if(node->up)
	{
		value = -size-1;
		connect_node(nodes, node, value, &node->up, &nodes[node->nr+value]->down, mode);
	}
	

	if(node->down)
	{
		value = size-1;
		connect_node(nodes, node, value, &node->down, &nodes[node->nr+value]->up, mode);
	}
	
	
	if(node->left)
	{
		value = -2;
		connect_node(nodes, node, value, &node->left, &nodes[node->nr+value]->right, mode);
	}
	
	if(node->right)
	{
		value = 0;
		connect_node(nodes, node, value, &node->right, &nodes[node->nr+value]->left, mode);
	}

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

	if(node->edge > 0)
	{
		for(int i = 0; i < node->edge; i++)
		{
			if(!node->next_node[i]->checked)
				connect_graph(nodes, node->next_node[i], size, in, mode); 
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
	
		connect_graph(nodes, node, size, in, mode);
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
}
