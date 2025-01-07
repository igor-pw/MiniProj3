#include "graph.h"
#include "func.h"

void save_status(node_t *nodes, int size)
{
	FILE *out = fopen("status.txt", "w");
	
	//zapisanie statusu kazdego wierzcholka do pliku
	for(int i = 0; i < size*size; i++)
	{
		fprintf(out, "| nr: %2d | waga: %.2f | polaczenia:", nodes[i]->nr, nodes[i]->weight); 
	
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

	//generowanie gornej sciany labiryntu
	for(int i = 0; i < in-1; i++)
		printf("_ ");

	printf("  ");

	for(int i = in; i < size; i++)
	       printf("_ ");	
	
	printf("\n");

	//generowanie srodka labiryntu
	for(int i = 0; i < size-1; i++)
	{
		printf("|");
		
		for(int j = 0; j < size-1; j++)
		{	
			//sprawdzenie i generowanie przejscia w dol
			if(!nodes[j+i*size]->down)
				{
					if(!nodes[j+i*size+size]->up)
						printf("_");

					else if(nodes[j+i*size+size]->up)
						printf(" ");
				}

			else if(nodes[j+i*size]->down)
				printf(" ");
			
			//sprawdzenie i generowanie przejscia w prawo
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

		//sprawdzenie i generowanie przejscia dla wierzcholkow prawej krawedzi labiryntu
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

	//sprawdzenie i generowanie przejscia dla wierzcholkow przy dolnej krawedzi labiryntu
	for(int i = size*size-size; i < size*size-1; i++)
	{
		//sprawdzenie i generowanie przejscia w prawo
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

		//sprawdzenie i generowanie przejscia w prawo w przypadku wyjscia
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

	//wygenerowanie prawego dolnego rogu labiryntu
	if(!nodes[size*size-1]->end)
		printf("_|\n\n");

	//wygenerowanie prawego dolnego rogu labiryntu w przypadku wyjscia
	else if(nodes[size*size-1]->end)
		printf(" |\n\n");
}

void init_node(node_t *nodes, int size, int out, int index)
{
	int n = size*size;
	

	//sprawdzenie czy alokacja pamieci sie powiodla
	if(nodes == NULL)
	{
		nodes = malloc(n * sizeof nodes);

		if(nodes == NULL)
		{
			printf("Blad alokacji pamieci\n");
			return;
		}
	}

	//utworzenie struktury wierzcholka grafu
	node_t node = malloc(sizeof *node);
	
	//przypisanie wartosci zmiennych w strukturze
	node->nr = index+1;	
	node->edge = 0;
	node->weight = generate_weight();	
	node->checked = false;
	node->end = false;
	node->connected = false;

	if(index == out - 1)
		node->end = true;

	node->up = true;
	node->down = true;
	node->right = true;
	node->left = true;
	
	//poprawienie mozliwych kierukow polaczen dla wierzcholkow znajdujacych sie przy wyjsciu labiryntu
	correct_directions(node, size);

	//zapisanie adresu wierzcholka w tablicy
	nodes[index] = node;	

	index++;

	//inicjacja kolejnego wierzcholka
	if(index < n)
		init_node(nodes, size, out, index);

	else 
		return;

}

void correct_possible_connection(node_t *nodes, int size, int out)
{
	//poprawienie ilosci mozliwych polaczen dla wierzcholkow najdujacych sie obok wyjscia labiryntu
	if(nodes[out-1]->up)
		nodes[out-size-1]->possible_connection--;

	if(nodes[out-1]->left)
		nodes[out-2]->possible_connection--;

	if(nodes[out-1]->right)
		nodes[out]->possible_connection--;

	//poprawienie mozliwych kierunkow dla wyjscia
	nodes[out-1]->up = false;
	nodes[out-1]->down = false;
	nodes[out-1]->right = false;
	nodes[out-1]->left = false;

	return;
}

void connect_graph(node_t *nodes, node_t node, int size, int in, int mode)
{
	//droga do wierzcholka istieje
	node->checked = true;	
	
	//wartosc na podstawie ktorej otrzymujemy dostep do odpowiedniego wierzcholka
	int value;

	//sprawdzenie czy moze przejsc w gore
	if(node->up)
	{
		value = -size-1;
		//wylosowanie przejcia w gore
		connect_node(nodes, node, value, &node->up, &nodes[node->nr+value]->down, mode);
	}
	
	//sprawdzenie czy mozna przejsc w dol
	if(node->down)
	{
		value = size-1;
		//wylosowanie przejsia w dol
		connect_node(nodes, node, value, &node->down, &nodes[node->nr+value]->up, mode);
	}
	
	//sprawdzenie czy mozna przejsc lewo
	if(node->left)
	{
		value = -2;

		//wylosowanie przejscia w lewo
		connect_node(nodes, node, value, &node->left, &nodes[node->nr+value]->right, mode);
	}
	
	//sprawdzenie czy mozna przejsc w prawo
	if(node->right)
	{
		value = 0;

		//wylosowanie przejscia w prawo
		connect_node(nodes, node, value, &node->right, &nodes[node->nr+value]->left, mode);
	}

	//wierzcholek ma chociaz jedno polaczenie do innego wierzcholka
	if(node->edge > 0)
	{
		//alokacja tablicy ze wskaznikami do polaczonych wierzcholkow
		node->next_node = malloc(sizeof node * node->edge);
		int counter = 0;
	
		//zapiwanie w tablicy wskaznikow do polaczanych wierzcholkow grafu
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

	//rekurencyjne wywolanie funkcji losujacej polaczenia z wierzcholkami
	if(node->edge > 0)
	{
		for(int i = 0; i < node->edge; i++)
		{
			//wywolanie funkcji dla polaczonych wierzcholkow
			if(!node->next_node[i]->checked)
				connect_graph(nodes, node->next_node[i], size, in, mode); 
		}
	}

	//sprawdzenie czy wejscie ma chociaz jedno przejsie
	else if(node->edge < 1 && node->nr == in)
	{
		//ponowne ustawienie wszystkich mozliwych polaczen dla wejscia
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
		
		//ponowne wywolanie funkcji
		connect_graph(nodes, node, size, in, mode);
	}
}

void free_graph(node_t *nodes, int n)
{
	for(int i = 0; i < n; i++)
	{
		//zwolnienie pamieci zajmowanej przez strukture wierzcholka
		if(nodes[i]->edge == 0)
			free(nodes[i]);

		//zwolnienie tablicy polaczen do innych wierzcholkow oraz pamieci zajmowanej przez strukture wierzcholka
		else
		{
			free(nodes[i]->next_node);
			free(nodes[i]);
		}
	}
}
