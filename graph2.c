#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "graph2.h"

void print_status(node_t* temp, int size, int in, int out)
{
	for (int i = 0; i < size * size; i++)
	{
		if (temp[i]->checked)
			printf("| sprawdzone ");

		else
			printf("| niesprawdzone |");
		printf("| adres: %p | nr:  %2d | polaczenia: %d | dostepne wierzcholki: ", temp[i], temp[i]->nr, temp[i]->edge);

		for (int j = 0; j < temp[i]->edge; j++)
		{
			printf(" %2d ", temp[i]->next_node[j]->nr);
		}
		printf("|\n");
	}

	printf("\n\n");
}


void print_graph(node_t *temp, int size, int in, int out, int counter)
{
     
	for(int i = 0; i < size; i++)
	{
		if(i != in-1)
			printf(" _");
		else if(i == in-1)
			printf("  ");
	}

	//printf("\n|");

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	for (int i = 0; i < temp[counter]->edge; i++)
	{
		if (temp[counter]->nr+1 == temp[counter]->next_node[i]->nr)
			right = true;

		if (temp[counter]->nr-1 == temp[counter]->next_node[i]->nr)
			left = true;

		if (temp[counter]->nr+size == temp[counter]->next_node[i]->nr)
			up = true;

		if (temp[counter]->nr-size == temp[counter]->next_node[i]->nr)
			down = true;
			
	}
	
	if (!left)
	{
		printf("\033]C");
		printf("|");
		printf("\033]D");
	}

	if(!up)
	{
		printf("\033]B");
		printf("|");
		printf("\033]A");
	}

	if (!left)
		printf("|");
		
	if (!down)
		printf("_");

	counter++;

	print_graph(temp, size, in, out, counter);

	if(counter + 1 % size == 0 )
		printf("\n");
}


node_t *init_graph(int size, int in, int out)
{
	int n = size*size;

	node_t *temp = malloc(sizeof *temp * n);
	
	for(int i = 0; i < n; i++)
	{		
		node_t node = malloc(sizeof *node);

		node->nr = i+1;
		node->checked = false;
		
		if(i == in - 1)
		{
			node->type = -1;
			node->end = true;
		}

		else if(i == out - 1)
		{
			node->type = 1;
			node->end = true;
		}

		else
		{
			node->type = 0;
			node->end = false;
		}	
		
		temp[i] = node;
	}

	for(int i = 0; i < n; i++)
	{
		int counter = 0;
		int edge;
		
		if((temp[i]->nr == 1) || temp[i]->nr == size || temp[i]->nr == ((size-1)*size+1) || temp[i]->nr == size*size)
		{
			temp[i]->next_node = malloc(sizeof temp * 2);
		}
		else if((temp[i]->nr > 1 && temp[i]->nr < size) || (temp[i]->nr > (size-1)*size+1 && temp[i]->nr < size*size) || (temp[i]->nr % size <= 1 && (temp[i]->nr != 1 || temp[i]->nr != (size-1)*size+1 || temp[i]->nr != size || temp[i]->nr != size*size)))
		{
			temp[i]->next_node = malloc(sizeof temp * 3);
		}

		else	
		{
			temp[i]->next_node = malloc(sizeof temp * 4);
		}

		if(temp[i]->nr-1 > 0 && (temp[i]->nr-1) % size != 0)
		{
			temp[i]->next_node[counter] = temp[i-1];
			counter++;
		}

		if(temp[i]->nr+1 <= n && (temp[i]->nr+1) % size != 1)
                {
                        temp[i]->next_node[counter] = temp[i+1];
                        counter++;
                }
		
		if(temp[i]->nr-size > 0 )
                {
                        temp[i]->next_node[counter] = temp[i-size];
                        counter++;
                }
	
		if(temp[i]->nr+size <= n)
                {
                        temp[i]->next_node[counter] = temp[i+size];
                        counter++;
                }

		temp[i]->edge = counter;		
	}

	return temp;
}

void rand_graph(node_t node, int size, int in, int out)
{

	if(node->edge > 0)
	{
		bool *to_connect = malloc(sizeof(bool) * node->edge);
		int counter_true = 0;
		int counter_false = 0;

		for(int i = 0; i < node->edge; i++)
		{
			int connection = rand() % 2;

			if(connection == 0 || counter_true == 2)
			{
				to_connect[i] = false;
				counter_false++;
			}

			else if(connection == 1 || counter_false == 2)
			{
				to_connect[i] = true;
				counter_true++;
			}
		}

		if(counter_false > 0)
		{
			node_t *tab_to_connect = malloc(sizeof node * counter_true);
			int connected = 0;
			int not_connected = 0;

			for(int i = 0; i < node->edge; i++)
			{
 				if(to_connect[i])
				{
					tab_to_connect[connected] = node->next_node[i];
					connected++;
				}

				else if(!to_connect[i])
				{
					not_connected++;
				}
			}
		

			for(int i = 0; i < node->edge; i++)
			{
				if(node->next_node[i]->edge >= 1)
				{
					node_t *tab = malloc(sizeof node * (node->next_node[i]->edge-1));
					int counter2 = 0;

					for(int j = 0; j < node->next_node[i]->edge; j++)
					{
						if(node->next_node[i]->next_node[j] != node)
						{
							tab[counter2] = node->next_node[i]->next_node[j];
							counter2++;
						}
					}

					node->next_node[i]->edge -= 1;
					if(node->next_node[i]->edge > 0)
					{
						//printf("przed realloc: %p ", node->next_node[i]->next_node);
						node->next_node[i]->next_node = realloc(node->next_node[i]->next_node, sizeof node * node->next_node[i]->edge);
						//printf("po realloc: %p\n", node->next_node[i]->next_node);
						for(int j = 0; j < node->next_node[i]->edge; j++)
							node->next_node[i]->next_node[j] = tab[j];
					}

					else
					{
						node->next_node = NULL;
					}

					free(tab);	
				}
			
				else
					node->next_node = NULL;
			}
		
			node->edge -= not_connected;

			if(node->edge > 0)	
			{
				node->next_node = realloc(node->next_node, sizeof node * node->edge);

				for(int i = 0; i < node->edge; i++)
				{
					node->next_node[i] = tab_to_connect[i];
				}
			}
			
			free(tab_to_connect);
		}
		
		else
			node->next_node = NULL;

	

		free(to_connect);

	}

	for(int i = 0; i < node->edge; i++)
	{
		if(node->next_node != NULL)
			rand_graph(node->next_node[i], size, in, out);
	}
}

void connect_graph(node_t *temp, int size, int in, int out)
{
	int n = size*size;

	for(int i = 0; i < n; i++)
	{
		node_t *tab = malloc(sizeof tab * temp[i]->edge);
		int counter = 0;
		
		for(int j = 0; j < temp[i]->edge; j++)
		{	
			for(int k = 0; k < temp[i]->next_node[j]->edge; k++)
			{
				if(temp[i]->next_node[j]->next_node[k] == temp[i])
				{
					tab[counter] = temp[i]->next_node[j];
					counter++;
					break;
				}
			}
		}	
		
		if(counter != temp[i]->edge)
		{
			temp[i]->edge = counter;
			temp[i]->next_node = realloc(temp[i]->next_node, sizeof temp * temp[i]->edge);
			
			for(int l = 0; l < temp[i]->edge; l++)
			{
				temp[i]->next_node[l] = tab[l];
			}
		}

	}
}

node_t *check_graph(node_t node)
{
	node->checked = true;
	
	for(int i = 0; i < node->edge; i++)
	{
		if(node->type != 1 && node->edge > 0)
		check_graph(node->next_node[i]);
	}
}	

void free_graph(node_t *temp, int size)
{
	for(int i = 0; i < size*size; i++)
	{
		free(temp[i]->next_node);
		free(temp[i]);
	}

	free(temp);
}




