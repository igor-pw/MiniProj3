#include <stdbool.h>
#include <string.h>

double generate_weight();

void correct_directions(node_t node, int size);

void connect_node(node_t *nodes, node_t node, int value, bool *direct, bool *opposite_direction, int mode);

int check_value(char *choice, int min, int max, int index);

int select_mode(int size, int min, int max);

void check_mode(int *mode, int size, int min, int max);

int change_mode(int size, int *mode, int min, int max);

void check_answer(char* choice);

void* timer(void* arg);

void scan_value(char *string, char *number, int index);
