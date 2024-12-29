#include <stdbool.h>

void connect_node(node_t *nodes, node_t node, int value, bool *direct, bool *opposite_direction, int *mode);

int rand_values(int mode, int argc, char **argv, int *size, int *in, int *out);
