#include "graph.h"
#include "func.h"
#include <unistd.h>

double generate_weight()
{
	//losowanie liczby jednosci wagi
	double weight = rand() % 5;
        
	int temp = 0;

	//wylosowanie liczby po przecinku
      	temp = 1 + rand() % 100;

        double decimal = temp;

	return weight+decimal/100;
}

void correct_directions(node_t node, int size)
{
	//wierzcholki znajdujace sie w rogu 
	if((node->nr == 1) || node->nr == size || node->nr == ((size-1)*size+1) || node->nr == size*size)
        {
		//ilosc mozliwych krawedzi, ktore moga przylaczyc sie do wierzcholka
                node->possible_connection = 2;

		//poprawienie mozliwych kierunkow, krawedzi ktore moga wychodzic z wierzcholka
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

	//wierzcholki znajdujace sie przy krawedzi labiryntu
        else if((node->nr > 1 && node->nr < size) || (node->nr > (size-1)*size+1 && node->nr < size*size) || (node->nr % size <= 1 && (node->nr != 1 || node->nr != (size-1)*size+1 || node->nr != size || node->nr != size*size)))
        {
		//ustawienie mozliwych krawedzi, ktore moga przylaczyc sie do wierzcholka
                node->possible_connection = 3;

		//poprawienie mozliwych kierunkow krawedzi, ktore moga wychodzic z wierzcholka
                if(node->nr > 1 && node->nr < size)
                        node->up = false;

                else if(node->nr > (size-1)*size+1 && node->nr < size*size)
                        node->down = false;

                else if(node->nr % size == 1)
                        node->left = false;

                else if(node->nr % size == 0)
                        node->right = false;
        }

	//wierzcholki znajdujace sie w srodku labiryntu
        else
		//ustawienie mozliwych krawedzi, ktore moga przylaczyc sie do wierzcholka
                node->possible_connection = 4;

}

void connect_node(node_t *nodes, node_t node, int value, bool *direction, bool *opposite_direction, int mode)
{	
	//value odpowiada za dostep do sasiadujacego wierzcholka 
	//mode odpowiada za to dla jakiej wartosci possible_connection (ilosc mozliwych przylaczen krawedzi wychodzacych z sasiadujacych wierzcholkow) polaczenie zostanie wymuszone
	//direction to kierunek mozliwej krawedzi wierzcholka
	//opposite_direction to przeciwny kierunek mozliwej krawedzi sasiedniego wierzcholka

	int to_connect;

	//sasiedni wierzcholek nie mial losowanych polaczen
	if(!nodes[node->nr+value]->checked)
	{
		//wymuszenie polaczenia sasiedniego wierzcholka
		if(nodes[node->nr+value]->possible_connection <= mode && !nodes[node->nr+value]->connected)
		{
			//zmiany w wartosciach zmiennych struktur wierzcholka oraz sasiada
			*opposite_direction = false;
			nodes[node->nr+value]->connected = true;
			node->edge++;
		}

		//czy sasiedni wierzcholek ma przylaczona krawedz
		else if(nodes[node->nr+value]->connected)
		{
			//zmiany w wartosciach zmiennych struktur wierzcholka oraz sasiada
			nodes[node->nr+value]->possible_connection--;
			*direction = false;
		}
			
		//losowanie czy wierzcholek ma miec polaczenie do sasiada
		else
		{
			to_connect = rand() % 2;

			//braku polaczenia
			if(to_connect == 0)
			{
				nodes[node->nr+value]->possible_connection--;
				*direction = false;
			}

			//polaczenie
			else if(to_connect == 1)
			{
				*opposite_direction = false;
				nodes[node->nr+value]->connected = true;
				node->edge++;
			}
		}
	}

	//sasiedni wierzcholek mial losowane polaczenia
	else if(nodes[node->nr+value]->checked)
	{
		nodes[node->nr+value]->possible_connection--;
		*direction = false;
	}
}

int select_mode(int size, int min, int max)
{		
	//wybor trybu 1, 2 lub 3
	if(size >= min && size <= 10)
	      	return 1;

	else if(size > 10 && size <= 25)
		return 2;

	else if(size > 25 && size <= max)
		return 3;	

	//niepowodzenie wybrania trybu
	else 
		return 0;
	
}

int check_value(char *choice, int min, int max, int index)
{
	char *text[] = {"rozmiaru", "wejscia", "wyjscia"};			
	int value = 0;

	bool correct = false;

	while(!correct)
	{	
		bool number = true;
		
		//sprawdzenie czy kazdy znak jest cyfra
		for(long unsigned int i = 0; i < strlen(choice); i++)
		{
			if(choice[i] - '0' < 0 || choice[i] - '0' > 9)
			{
				number = false;
				break;
			}
		}

		int temp = atoi(choice);

		//sprawdzenie czy podany napis jest liczba oraz czy zawiera sie w przedziale od min do max 
		if(number && temp >= min && temp <= max)
			correct = true;
				
		//ponowne odczytanie wartosci (rozmiaru, wejscia, lub wyjscia)
		if(!correct)
		{
			printf("Podano nieprawidlowa wartosc %s labiryntu (%s)\n", text[index], choice);
			printf("Wprowadz ponownie wartosci (%d-%d): ", min, max);

			scanf("%s", choice);
			printf("\n");
		}
	}

	int temp = 1;

	//przekonwertowanie zmiennej string na int
	for(int i = strlen(choice)-1; i >= 0; i--)
	{
		int number = choice[i] - '0';
		value += number*temp;
	       	temp *= 10;	
	}

	return value;
}

void check_mode(int *mode, int size, int min, int max)
{
	system("clear");
	

	//sprawdzenie czy wartosc trybu jest poprawna
	while(*mode < 1 || *mode > 3)
        {
		//zapytanie o reczne wybieranie trybu
                printf("Niepoprawny tryb generowania labiryntu. Czy chce kontynuowac reczne wybieranie trybu? [y/n]\n");

                char *choice = malloc(sizeof choice);

                scanf("%s", choice);

		//sprawdzenie poprawnosci wyboru
		check_answer(choice);

		//reczny wybor trybu
                if(*choice == 'y')
                {
                        printf("\nWprowadz tryb generowania labiryntu (1-3): ");
                        scanf("%s", choice);
			
			printf("\n");

			//wybranie odpowiedniego trybu
                        if((*choice == '1'|| *choice == '2' || *choice == '3') && strlen(choice) == 1)
                        {
                                char cmode = *choice;
                                *mode = cmode - '0';

				system("clear");
				free(choice);
                        }

                }

		//automatyczny wybor trybu
                else if(*choice == 'n')
                {
                        free(choice);
			system("clear");

			printf("\nAutomatyczne wybieranie trybu...\n");
                        *mode = select_mode(size, min, max);
                }

        }
}

int change_mode(int size, int *mode, int min, int max)
{
	char *choice = malloc(sizeof choice);

	//zapytanie o kontynuacje proby wygenerowania labiryntu
	printf("\nCzy chcesz kontynuowac generowanie labiryntu dla wybranego trybu? [y/n]\n");
	scanf("%s", choice);

	//sprawdzenie poprawnosci wyboru
	check_answer(choice);	

	//kontynuowanie proby wygenerowania labiryntu
	if(*choice == 'y')
	{
		system("clear");
		free(choice);
		
		return 0;
	}
	
	//zmiana trybu na standardowy
	else if(*choice == 'n')
	{
		free(choice);
		system("clear");	

		printf("\nAutomatyczne wybieranie trybu...\n");
		*mode = select_mode(size, min, max);
	}

	return 1;
}

void check_answer(char *choice)
{
	bool correct = false;

    	while(!correct)
    	{
		//poprawna odpowiedz
     		if((*choice == 'y' || *choice == 'n') && strlen(choice) == 1)
  	            correct = true;
		
		//niepoprawna odpowiedz
		else
             	{
			//ponowny odczyt
           		printf("\nNieprawidlowy wybor (%s)\n", choice);
                 	printf("Wprowadz wartosc ponownie [y/n]: ");
            		scanf("%s", choice);
           	}
    	}
}

void* timer(void* arg)
{
	//symulowanie czasomierza
	for(int *duration = (int*)arg; *duration >=0; *duration -= 1)
		sleep(1);

	return NULL;
}

void scan_value(char *string, char *number, int index)
{
	char *text[] = {"rozmiar", "wejscie", "wyjscie", "tryb generowania"};

	//odczytanie odpowiedzi
	printf("\nCzy chcesz podac %s labiryntu? [y/n]\n", text[index]);
        scanf("%s", string);

	//sprawdzenie poprawnosci odpowiedzi
	check_answer(string);

	//pozytywna odpowiedz
        if(*string == 'y')
        {
                //odczytanie wartosci
		printf("\nWprowadz %s labiryntu: ", text[index]);
                scanf("%s", number);
        }

}
