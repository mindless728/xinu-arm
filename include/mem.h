#ifndef MEM_H
#define MEM_H
#include <stdio.h>
#include <stdlib.h>
#define buffer 0xDEADBEEF



typedef struct _node{
	struct _node  * prev;		
	struct _node  * next;		
	unsigned int size;		
	void  * data_addr;

}node;
//first node	 
//Create node
node * create_node(node *, int );
//The almighty malloc:
void * m_malloc(unsigned int);
//Check for the buffer 16 bytes out from the address given
unsigned int check_deadbeef(node *);
//The almighty free
void m_free(void * addr);
//Figure out if two nodes are contigious
unsigned int is_contiguous(node , node);
//Merge two nodes
node *  merge(node * , node * );
#endif
