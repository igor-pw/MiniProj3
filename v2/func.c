#include "graph.h"
#include "func.h"

#define MAX_SIZE 361

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

int select_mode(int size)
{		
	if(size > 2 && size <= 10)
	      	return 1;

	else if(size > 10 && size <= 25)
		return 2;

	else if(size > 25 && size <= MAX_SIZE)
		return 3;	

	else 
		return 0;
	
}

void rand_values(int argc, char **argv, int *size, int *in, int *out)
{
	*size = argc > 1 ? atoi(argv[1]) : 3 + rand() % MAX_SIZE;
	
	while(*size < 3 || *size > MAX_SIZE)
        {
                printf("Podano nieprawidlowy rozmiar labiryntu (%d)\n", *size);
                printf("Wprowadz ponownie wartosc (3-%d): ", MAX_SIZE);
                scanf("%d", size);
        }

        int v_size = *size;
        int n = v_size*v_size;

        *in = argc > 2 ? atoi(argv[2]) : 1 + rand() % v_size;
		
	while(*in < 1 || *in > v_size)
	{
		printf("Podano niewlasciwa wartosc dla wejscia labiryntu (%d)\n", *in);
		printf("Wprowadz ponownie wartosc (1-%d): ", v_size);
		scanf("%d", in); 
	}


        *out = argc > 3 ? atoi(argv[3]) : 1 + rand() % v_size;
	
	while(*out < 1 || *out > v_size)
	{
                printf("Podano niewlasciwa wartosc dla wyjscia labiryntu (%d)\n", *out);
                printf("Wprowadz ponownie wartosc (1-%d): ", v_size);
                scanf("%d", out);
        }

        *out += n-v_size;

        if(argc < 3)
        {
                while(*out % v_size == *in % v_size)
                        *out = n - rand() % v_size;
        }

}

void check_mode(int *mode, int size)
{
	while(*mode < 1 || *mode > 3)
        {
                printf("\nNiepoprawny tryb generowania labiryntu. Czy chce kontynuowac reczne wybieranie trybu? [y/n]\n");

                char *choice = malloc(sizeof choice);

                scanf("%s", choice);

                if(*choice == 'y' && strlen(choice) == 1)
                {
                        printf("\nWprowadz tryb generowania labiryntu: ");
                        scanf("%s", choice);

                        if((*choice == '1'|| *choice == '2' || *choice == '3') && strlen(choice) == 1)
                        {
                                char cmode = *choice;
                                *mode = cmode - '0';
                        }

                }

                else
                {
                        printf("\nAutomatyczne wybieranie trybu...\n");
                        *mode = select_mode(size);
                }

        }
}
