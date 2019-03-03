#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include "headers.h"

#define NETROWS    6
#define NETCOLS    3

typedef struct
{
   int row;
   int col;
} loc_t;

loc_t locations[ NETROWS ][ NETCOLS ] = {
   { {  2,  4 }, {  2, 18 }, {  2, 32 } },
   { {  4,  4 }, {  4, 18 }, {  4, 32 } },
   { {  6,  4 }, {  6, 18 }, {  6, 32 } },
   { {  8,  4 }, {  8, 18 }, {  8, 32 } },
   { { 10,  4 }, { 10, 18 }, { 10, 32 } },
   { { 12,  4 }, { 12, 18 }, { 12, 32 } },
};

// Display location for nodes in the network window.
int 

// Template bitfiels
//
//   bit 0 - north connection
//   bit 1 - south connection
//   bit 2 - east connection
//   bit 3 - west connection
//   bit 4 - entry node
//   bit 5 - exit node
//   bit 6 - special node

#define NOR       1
#define SOU       2
#define EAS       4
#define WES       8
#define ENTRY    16
#define EXIT     32
#define SPECIAL  64

#define MAX_TEMPLATES    6

const unsigned short templates [ MAX_TEMPLATES ][ NETROWS ][ NETCOLS ] = {
   {
      { SOU | ENTRY         ,                     0,               0 },
      { NOR | SOU | EAS     , EAS | WES | SOU      ,       WES | SOU },
      { NOR | SOU | EAS     , NOR | SOU | EAS | WES, NOR | SOU | WES },
      { NOR | SOU | EAS     , NOR | EAS | WES      ,       NOR | WES },
      { NOR | SPECIAL | EXIT,                     0,               0 },
   },
   {
      { 0, 1, 0 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 0, 1, 0 },
   },
   {
      { 0, 0, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 0, 0, 1 },
   },
   {
      { 1, 0, 0 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 0, 0, 1 },
   },
   {
      { 0, 0, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 0, 0 },
   },
   {
      { 0, 1, 0 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 1, 1, 1 },
      { 0, 1, 0 },
   }
};

node_t *cur_node;

node_t *network[ NETROWS ][ NETCOLS ];

int cur_template = 0;

void display_network( WINDOW *win )
{
   for ( int rows = 0 ; rows < NETROWS ; rows++ )
   {
      for ( int cols = 0 ; cols < NETROWS ; cols++ )
      {
         if ( network[ rows ][ cols ] )
         {
            if ( network[ rows ][ cols ]->visible )
            {
               mvwprintw( win, 
                           locations[ rows ][ cols ].row,
                           locations[ rows ][ cols ].col,
                           network[ rows ][ cols ]->ip_adrs );
            }
         }
      }
   }

#if 0
   // Temp
   mvwprintw( win,  2, 4, "[xx.xx.xx.xx]                            " );
   mvwprintw( win,  3, 4, "      |                                  " );
   mvwprintw( win,  4, 4, " xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx " );
   mvwprintw( win,  5, 4, "      |             |             |      " );
   mvwprintw( win,  6, 4, " xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx " );
   mvwprintw( win,  7, 4, "      |             |             |      " );
   mvwprintw( win,  8, 4, " xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx " );
   mvwprintw( win,  9, 4, "      |             |             |      " );
   mvwprintw( win, 10, 4, " xx.xx.xx.xx - xx.xx.xx.xx - xx.xx.xx.xx " );
   mvwprintw( win, 11, 4, "      |                                  " );
   mvwprintw( win, 12, 4, " xx.xx.xx.xx                             " );
   // Temwin
#endif
}

// Called by screen to display the processes of cur_node;
void display_network_processes( WINDOW *win )
{
   int line = 2;

   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( is_process_active( cur_node->processes[ index ] ) )
      {
         display_process( win, line, cur_node->processes[ index ] );
         line++;
      }

   }

   display_player( win, line );

   mvwprintw( win, line+1, 4, "                                                " );

   return;
}

void create_network_processes( node_t *node, int level )
{
   int i;
   const int proclimit[ 6 ] = { 1, 2, 3, 5, 7, 8 };
   int limit = 2 + getRand( proclimit[ level-1 ] );

   switch( level )
   {
      case 1:  // First level only daemons
      case 2:
      case 3:  // Fork
      case 4:
      case 5:
      case 6:  // Final level, introduce the executive
         i = 0;
         break;
      default:
         assert( 0 );
         break;
   }

   for ( ; i < limit ; i++ )
   {
      if ( getSRand( ) > 0.5 )
      {
         node->processes[ i ] = create_process( Daemon, level );
      }
      else
      {
         node->processes[ i ] = create_process( Worker, level );
      }
   }

   return;
}


void set_visible_nodes( void )
{
   int row = cur_node->row;
   int col = cur_node->col;

   if ( templates[ cur_template ][ row ][ col ] & NOR )
   {
      network[ row-1 ][ col ]->visible = 1;
   }
   if ( templates[ cur_template ][ row ][ col ] & SOU )
   {
      network[ row+1 ][ col ]->visible = 1;
   }
   if ( templates[ cur_template ][ row ][ col ] & WES )
   {
      network[ row ][ col-1 ]->visible = 1;
   }
   if ( templates[ cur_template ][ row ][ col ] & EAS )
   {
      network[ row ][ col+1 ]->visible = 1;
   }
   
   return;
}


node_t *create_node( int level, unsigned short template, int row, int col )
{
   node_t *node = calloc( 1, sizeof( node_t ) );

   snprintf( node->ip_adrs, MAX_IP_ADRS, "%2d.%2d.%2d.%2d", 
               level+1, getRand(99), getRand(99), getRand(98)+1 );

   // Initialize the node.

   create_network_processes( node, level );

   if ( template & ENTRY )
   {
      cur_node = node;
      node->visible = node->visited = 1;
      node->row = row; node->col = col;
   }

   return node;
}


void create_network( int level )
{
   // @todo: need to randomize cur_template.

   int rows, cols;

   for ( rows = 0 ; rows < NETROWS ; rows++ )
   {
      for ( cols = 0 ; cols < NETCOLS ; cols++ )
      {
         // Is a node to be placed here?
         if ( templates[ cur_template ][ rows ][ cols ] )
         {
            network[ rows ][ cols ] = 
               create_node( level, templates[ cur_template ][ rows ][ cols ],
                            rows, cols );
         }
      }
   }

   set_visible_nodes( );

   return;
}


void execute_network( void )
{
   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      execute_process( cur_node->processes[ index ] );
   }

   return;
}


process_t *find_process_by_pid( unsigned short pid )
{
   process_t *proc = ( process_t * ) 0;

   for ( int i = 0 ; i < MAX_PROCESSES_PER_NODE ; i++ )
   {
      if ( is_process_active( cur_node->processes[ i ] ) )
      {
         if ( cur_node->processes[ i ]->pid == pid )
         {
            proc = cur_node->processes[ i ];
            break;
         }
      }
   }

   return proc;
}

