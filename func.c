#include "graph.h"
#include "func.h"
#include <unistd.h>

double generate_wage()
{
	double wage = rand() % 5;
        int temp = 0;

        while(temp % 10 == 0)
              temp = 1 + rand() % 100;

        double decimal = temp;

	return wage+decimal/100;
}

void correct_directions(node_t node, int size)
{
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

}

void connect_node(node_t *nodes, node_t node, int value, bool *direction, bool *opposite_direction, int mode)
{	
	int to_connect;

	if(!nodes[node->nr+value]->checked)
	{
		if(nodes[node->nr+value]->possible_connection <= mode && !nodes[node->nr+value]->connected)
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

int select_mode(int size, int max)
{		
	if(size > 2 && size <= 10)
	      	return 1;

	else if(size > 10 && size <= 25)
		return 2;

	else if(size > 25 && size <= max)
		return 3;	

	else 
		return 0;
	
}

int check_value(char **argv, int min, int max, int index)
{
	char *choice = argv[index+1];
	char *text[] = {"rozmiaru", "wejscia", "wyjscia"};			
	int value = 0;

	bool correct = false;

	while(!correct)
	{	
		bool number = true;
		
		for(long unsigned int i = 0; i < strlen(choice); i++)
		{
			if(choice[i] - '0' < 0 || choice[i] - '0' > 9)
			{
				number = false;
				break;
			}
		}

		int temp = atoi(choice);

		if(number && temp >= min && temp <= max)
			correct = true;
				
		if(!correct)
		{
			printf("Podano nieprawidlowa wartosc %s labiryntu (%s)\n", text[index], choice);
			printf("Wprowadz ponownie wartosci (%d-%d): ", min, max);

			scanf("%s", choice);
			printf("\n");
		}
	}

	int temp = 1;

	for(int i = strlen(choice)-1; i >= 0; i--)
	{
		int number = choice[i] - '0';
		value += number*temp;
	       	temp *= 10;	
	}

	return value;
}

void check_mode(int *mode, int size, int max)
{
	system("clear");
	
	while(*mode < 1 || *mode > 3)
        {
                printf("Niepoprawny tryb generowania labiryntu. Czy chce kontynuowac reczne wybieranie trybu? [y/n]\n");

                char *choice = malloc(sizeof choice);

                scanf("%s", choice);

		check_answer(choice);

                if(*choice == 'y')
                {
                        printf("\nWprowadz tryb generowania labiryntu (1-3): ");
                        scanf("%s", choice);
			
			printf("\n");

                        if((*choice == '1'|| *choice == '2' || *choice == '3') && strlen(choice) == 1)
                        {
                                char cmode = *choice;
                                *mode = cmode - '0';

				system("clear");
				free(choice);
                        }

                }

                else if(*choice == 'n')
                {
                        free(choice);
			system("clear");

			printf("\nAutomatyczne wybieranie trybu...\n");
                        *mode = select_mode(size, max);
                }

        }
}

int change_mode(int size, int *mode, int max)
{
	char *choice = malloc(sizeof choice);

	printf("\nCzy chcesz kontynuowac generowanie labiryntu dla wybranego trybu? [y/n]\n");
	scanf("%s", choice);

	check_answer(choice);	

	if(*choice == 'y')
	{
		system("clear");
		free(choice);
		
		return 0;
	}
	
	else if(*choice == 'n')
	{
		free(choice);
		system("clear");	

		printf("\nAutomatyczne wybieranie trybu...\n");
		*mode = select_mode(size, max);
	}

	return 1;
}

void check_answer(char *choice)
{
	bool correct = false;

                while(!correct)
                {
                        if((*choice == 'y' || *choice == 'n') && strlen(choice) == 1)
                                correct = true;
                        else
                        {
                                printf("\nNieprawidlowy wybor (%s)\n", choice);
                                printf("Wprowadz wartosc ponownie [y/n]: ");
                                scanf("%s", choice);
                        }
                }

}

void* timer(void* arg)
{
	for(int *duration = (int*)arg; *duration >=0; *duration -= 1)
		sleep(1);

	return NULL;
}	
