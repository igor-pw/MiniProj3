#include "graph.h"

void find_exit(node_t node, bool *found)
{
	if(node->end)
	{
		*found = true;
	}

	if(node->edge > 0 && !(*found))
	{
		for(int i = 0; i < node->edge; i++)
			find_exit(node->next_node[i], found);
	}
}
