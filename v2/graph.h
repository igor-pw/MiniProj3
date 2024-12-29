#ifndef _NODE_T
#define _NODE_T

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

typedef struct node
{
	bool end;
	bool checked;
	bool connected;
	bool up;
	bool down;
	bool right;
	bool left;
	int nr;
	int edge;
	int possible_connection;
	struct node **next_node;
} *node_t;

void print_status(node_t *nodes, int size, int in);

void print_maze(node_t *nodes, int size, int in);

void init_node(node_t *nodes, int size, int out, int index);

void correct_possible_connection(node_t *nodes, int size ,int out);

void connect_graph(node_t *nodes, node_t node, int size, int in, int *mode);

void check_graph(node_t nodes);

void free_graph(node_t *nodes, int n);

#endif
