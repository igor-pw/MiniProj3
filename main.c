#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "graph.h"
#include "func.h"
#include "search.h"

#define TIME 15 	//czas oczekiwania w sekundach 
#define MAX_SIZE 361 	//maksynalny rozmiar labiryntu
#define MIN_SIZE 3	//minimalny rozmiar labiryntu

int main()
{
	srand(time(NULL));
	system("clear");

	//rozmiar, wejscie, wyjscie, tryb
	int size, in, out, mode;

	char *string = malloc(sizeof string);
	char *number = malloc(sizeof number);

	//odczyt danych oraz sprawdzenie ich poprawnosc lub losowe wygenerowanie wartosci
	scan_value(string, number, 0);
	size = *string == 'y' ? check_value(number, MIN_SIZE, MAX_SIZE, 0) : MIN_SIZE + rand() % (MAX_SIZE-MIN_SIZE+1);
	if(*string == 'n')
		printf("\nrozmiar: %d\n", size);
	
	scan_value(string, number, 1);
	in = *string == 'y' ? check_value(number, 1, size, 1) : 1 + rand() % size;

	if(*string == 'n')
		printf("\nwejscie: %d\n", in);

	scan_value(string, number, 2);
	out = *string == 'y' ? check_value(number, 1, size, 2) : 1 + rand() % size; 

	if(*string == 'n')
		printf("\nwyjscie: %d\n", out);

	//odczyt trybu lub jego automatyczne wybranie
	scan_value(string, number, 3);
	mode = *string == 'y' ? atoi(number) : select_mode(size, MIN_SIZE, MAX_SIZE);

	//kalibracja wyjscia labiryntu
	out += size*size - size;

	//sprawdzene poprawnosci trybu
	check_mode(&mode, size, MIN_SIZE, MAX_SIZE);

	//ilosc wierzcholkow grafu
	int n = size*size;

	free(string);
	free(number);

	printf("\nGenerowanie labiryntu...\n");

	//utworzenie tablicy ze wskaznikami do struktur wierzcholkow grafu
	node_t *nodes = malloc(n * sizeof nodes);
	
	//utworzenie wierzchlokow grafow
	init_node(nodes, size, out, 0);

	//kalibracja mozliwych polaczen
	correct_possible_connection(nodes, size, out);

	//utworzenie polaczen miedzy wierzcholkami grafu (zaczynajac od wejscia)
	connect_graph(nodes, nodes[in-1], size, in, mode);

	//utworzenie watku do dzialania czasomierza w tle
	pthread_t timer_thread;				
	
	//zmienna odpowiadajaca za dlugosc czasu oczekiwania na wygenerowanie labiryntu
	int duration = 0; 

	bool first = true;

	//glowna petla sprawdzajaca czy do kazdego wierzcholka mozna dojsc
	for(int i = 0; i < n; i++)
	{
		//znalezienie wierzchloka bez dostepu
		if(!nodes[i]->checked)
		{	
			//zwolnienie pamieci
			free_graph(nodes, n);

			//uplyniecie czasu mierzonego przez czasomierz
			if(duration == 0)
			{
				//zmienna kontrolujaca kontynuacje generowania labiryntu przy recznie wybranym trybie	
				int choice = 1;
 				
				//pierwsze przejscie przez warunek
				if(first)
                                {
					first = false;

					//ustawienie czasu oczekiwania
                                        duration = TIME;

					//utworzenie czasomierza
                                        pthread_create(&timer_thread, NULL, timer, &duration);
                                }

				//kazde nastepne przejscie przez warunek
				else if(!first)
				{
					//zwolnienie watku czasomierza
					pthread_join(timer_thread, NULL);

					//zapytanie o zmiane recznie wybranego trybu
					choice = change_mode(size, &mode, MIN_SIZE, MAX_SIZE);
					
					printf("\nGenerowanie labiryntu...\n");
				}

				//kontynuowanie generowania labiryntu przy recznie wybranym trybie
				if(choice == 0)
				{				
					//zresetowanie czasu oczekiwania
					duration = TIME;

					//ponownie utworzenie czasomierza
					pthread_create(&timer_thread, NULL, timer, &duration);
				}
			}

			//ponowna inicjacja wierzcholkow
			init_node(nodes, size, out, 0);

			//ponowna kalibracja mozliwych polaczen
			correct_possible_connection(nodes, size, out);

			//ponowne wygenerowanie polaczen miedzy wierzcholkami grafu (zaczynajac od wejscia)
			connect_graph(nodes, nodes[in-1], size, in, mode);

			//zresetowanie licznika petli
			i = -1;
		}	
	}
	
	//wygenerowanie labiryntu
	print_maze(nodes, size, in);
	printf("Rozmiar labiryntu: %dx%d\n", size, size);
	
	//zapisanie statusu wierzcholkow labiryntu do pliku
	save_status(nodes, size);
	
	//zwolnienie watku czasomierza
	if(!first)
	{
		pthread_cancel(timer_thread);	
		pthread_join(timer_thread, NULL);
	}

	//przeszukiwanie labiryntu w celu znalezienia sciezki od wejscia do wyjscia

	//zmienna okreslajaca czy wyjscie zostalo znalezione
	bool found = false;
	
	//wierzcholek startowy, z ktorego szukane jest wyjscie
	node_t start_node = nodes[in-1];
	
	//wierzcholek startowy uzywany w funkcji find_exit
	node_t temp = nodes[in-1];

	//waga startowa sciezki
	double path_weight = start_node->weight;

	//otworzenie pliku, do ktorego zapisana zostanie sciezka
	FILE *path = fopen("path.txt", "w");

	//zapisanie startu sciezki do pliku
	fprintf(path, "Sciezka labiryntu: %d", start_node->nr);

	printf("\nPrzeszukiwanie labiryntu...\n");

	//przeszukiwanie labiryntu do momentu, w ktorym sciezka startowa jest wyjsciem
	while(start_node != nodes[out-1])
	{
		//przeszukiwanie sciezki dla kazdego wierzcholka polaczonego z wierzcholkiem startowym
		for(int i = 0; i < start_node->edge; i++)
		{
			//wierzcholki, ktore maja przejscie
			if(start_node->edge > 0)
			{
				//szukanie wyjscie
				find_exit(temp->next_node[i], &found);

				//sciezka zostala znaleziona
				if(found)
				{
					//zapisanie nr wierzcholka do pliku oraz dodanie wagi
					fprintf(path, " -> %d", start_node->next_node[i]->nr);
					path_weight += start_node->next_node[i]->weight;

					found = false;

					//zmiana wierzcholka startowego na ten, z ktorego znalezione zostalo wyjscie
					start_node = start_node->next_node[i];
					temp = start_node;
					break;
				}
			}
	
		}
	}

	//zapisanie wagi sciezki do pliku
	fprintf(path, "\n\nWaga sciezki: %.2f", path_weight);

	//wypisanie wagi sciezki w konsoli
	printf("\nWaga sciezki: %.2f\n\n", path_weight);

	//zamkniecie pliku
	fclose(path);

	//zwolnienie pamieci
	free_graph(nodes, n);
	free(nodes);

}
