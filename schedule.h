#ifndef __SCHEDULE_H
#define __SCHEDULE_H

typedef struct liveness{
	int *vars;
	int size;
}life_t;

	
	
void find_use_def_stmt(node_list_t *start);	
void print_use_def_stmt(node_list_t *start);	
void schedule(node_list_t *start);
#endif//__SCHEDULE_H