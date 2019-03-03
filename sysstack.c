#include "headers.h"

#define MAX_NODE_STACK_DEPTH   128

typedef struct
{
   node_t *entry[ MAX_NODE_STACK_DEPTH ];
   int top_index;
} node_stack_t;

static node_t *cur_node;

static node_stack_t node_stack;

void init_node_stack( node_t *node )
{
   node_stack.top_index = 0;
   cur_node = node;
}

node_t *current_node( void )
{
   return cur_node;
}

int node_stack_empty( void )
{
   return ( node_stack.top_index == 0 );
}

void set_current_node( node_t *node )
{
   cur_node = node;
}

void push_node( node_t *node )
{
   node_stack.entry[ node_stack.top_index++ ] = node;
}

node_t *pop_node( void )
{
   return node_stack.entry[ --node_stack.top_index ];
}

