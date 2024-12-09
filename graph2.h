#ifndef _NODE_T
#define _NODE_T

#include <stdbool.h>

typedef struct node
{
	bool end;
	bool checked;
	int type;
	int nr;
	int edge;
	struct node **next_node;
 
} node_o, *node_t;

void print_graph(node_t *temp, int size, int in, int out);

node_t *init_graph(int size, int in, int out);

void rand_graph(node_t node, int size, int in, int out);

void connect_graph(node_t *temp, int size, int in, int out);

node_t *check_graph(node_t temp);

void free_graph(node_t *temp, int size);

#endif
