#include <stdbool.h>
#include <string.h>

void connect_node(node_t *nodes, node_t node, int value, bool *direct, bool *opposite_direction, int mode);

void rand_values(int argc, char **argv, int *size, int *in, int *out);

int select_mode(int size);

void check_mode(int *mode, int size);
