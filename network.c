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
      { SOU | ENTRY         ,                     0,                    0 },
      { NOR | SOU | EAS     , EAS | WES | SOU      ,       WES | SOU      },
      { NOR | SOU | EAS     , NOR | SOU | EAS | WES, NOR | SOU | WES      },
      { NOR | SOU | EAS     , NOR | SOU | EAS | WES, NOR | SOU | WES      },
      { NOR | SOU | EAS     , NOR | EAS | WES      ,       NOR | WES      },
      { NOR | EXIT | SPECIAL,                     0,                    0 },
   },
   {
      {                    0, SOU | ENTRY           ,                    0 },
      { SOU | EAS           , WES | EAS | NOR       , WES | SOU            },
      { EAS | NOR           , WES | SOU             , WES | NOR            },
      { EAS | SOU           , WES | NOR | EAS       , WES | SOU            },
      { EAS | NOR           , WES | SOU | EAS       , WES | NOR            },
      {                    0, NOR | EXIT | SPECIAL  ,                    0 },
   },
   {
      {                    0,                      0, SOU | ENTRY          },
      { EAS | SOU           , WES | EAS             , WES | NOR            },
      { EAS | NOR           , WES | EAS             , WES | SOU            },
      { EAS | SOU           , WES | EAS             , WES | NOR            },
      { EAS | NOR           , WES | EAS             , WES | SOU            },
      {                    0,                      0, NOR | EXIT | SPECIAL },
   },
   {
      {                    0, SOU | ENTRY           ,                    0 },
      { EAS                 , NOR | SOU | EAS | WES , WES                  },
      { EAS                 , NOR | SOU | EAS | WES , WES                  },
      { EAS                 , NOR | SOU | EAS | WES , WES                  },
      { EAS                 , NOR | SOU | EAS | WES , WES                  },
      {                    0, NOR | EXIT | SPECIAL  ,                    0 },
   },
   {
      { SOU | ENTRY         ,                      0,                    0 },
      { NOR | EAS           , WES | EAS             , WES | SOU            },
      { EAS                 , WES | EAS             , WES | NOR | SOU      },
      { EAS                 , WES | EAS             , WES | NOR | SOU      },
      { SOU | EAS           , WES | EAS             , WES | NOR            },
      { NOR | EXIT | SPECIAL,                      0,                    0 },
   },
   {
      {                    0,                      0, SOU | ENTRY          },
      { EAS | SOU           , WES | SOU             , NOR | SOU            },
      { NOR | SOU           , NOR | SOU             , NOR | SOU            },
      { NOR | SOU           , NOR | SOU             , NOR | SOU            },
      { NOR | SOU           , NOR | EAS             , WES | NOR            },
      { NOR | EXIT | SPECIAL,                      0,                    0 },
   }
};

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
               if ( network[ rows ][ cols ] == current_node( ) ) wattron( win, A_BOLD );
               mvwprintw( win, 
                           locations[ rows ][ cols ].row,
                           locations[ rows ][ cols ].col,
                           network[ rows ][ cols ]->ip_adrs );
               if ( network[ rows ][ cols ] == current_node( ) ) wattroff( win, A_BOLD );

               if ( templates[ cur_template ][ rows ][ cols ] & NOR )
               {
                  mvwprintw( win,
                           locations[ rows ][ cols ].row - 1,
                           locations[ rows ][ cols ].col + 5, "|" );
               }
               if ( templates[ cur_template ][ rows ][ cols ] & SOU )
               {
                  mvwprintw( win,
                           locations[ rows ][ cols ].row + 1,
                           locations[ rows ][ cols ].col + 5, "|" );
               }
               if ( templates[ cur_template ][ rows ][ cols ] & EAS )
               {
                  mvwprintw( win,
                           locations[ rows ][ cols ].row,
                           locations[ rows ][ cols ].col + 12, "-" );
               }
               if ( templates[ cur_template ][ rows ][ cols ] & WES )
               {
                  mvwprintw( win,
                           locations[ rows ][ cols ].row,
                           locations[ rows ][ cols ].col - 2, "-" );
               }
            }
         }
      }
   }

   return;
}

// Called by screen to display the processes of current_node();
void display_network_processes( WINDOW *win )
{
   int line = 2;

   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( is_process_active( current_node()->processes[ index ] ) )
      {
         display_process( win, line, current_node()->processes[ index ] );
         line++;
      }
   }

   display_player( win, line );

   mvwprintw( win, line+1, 4, "                                                " );

   return;
}

void create_network_processes( node_t *node, int level )
{
   int i = 0;
   const int proclimit[ 6 ] = { 1, 2, 3, 5, 7, 8 };
   int limit = 2 + getRand( proclimit[ level-1 ] );
   process_t *process;

   switch( level )
   {
      case 1:  // First level only daemons
         if ( templates[ cur_template ][ node->row ][ node->col ] & SPECIAL )
         {
            process = calloc( 1, sizeof( process_t ) );
            node->processes[ i++ ] = create_process( process, Fork, level );
         }
         break;
      case 2:
      case 3:  // Fork
      case 4:
      case 5:
      case 6:  // Final level, introduce the executive
         break;
      default:
         assert( 0 );
         break;
   }

   if ( getSRand( ) > 0.8 )
   {
      if ( getSRand( ) > 0.5 )
      {
         process = calloc( 1, sizeof( process_t ) );
         node->processes[ i++ ] = create_process( process, Sentry, level );
      }
      else
      {
         process = calloc( 1, sizeof( process_t ) );
         node->processes[ i++ ] = create_process( process, Armor, level );
      }
   }

   for ( ; i < limit ; i++ )
   {
      process = calloc( 1, sizeof( process_t ) );
      if ( getSRand( ) > 0.5 )
      {
         node->processes[ i ] = create_process( process, Daemon, level );
      }
      else
      {
         node->processes[ i ] = create_process( process, Worker, level );
      }
   }

   return;
}


void set_visible_nodes( void )
{
   int row = current_node()->row;
   int col = current_node()->col;

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

void node_func( node_t *node, int state )
{
   if ( state == NODE_ENTRY )
   {
      // Invoke each processes init behavior.
      for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
      {
         if ( is_process_active( node->processes[ index ] ) )
         {
            ( node->processes[ index ]->function )( node->processes[ index ], PROCESS_INIT );
         }
      }

   }
   else
   {


   }

   return;
}

node_t *create_node( int level, unsigned short template, int row, int col )
{
   node_t *node = calloc( 1, sizeof( node_t ) );

   snprintf( node->ip_adrs, MAX_IP_ADRS, "%2d.%2d.%2d.%2d", 
               level+10, getRand(89)+10, getRand(89)+10, getRand(88)+10 );

   // Initialize the node.
   node->row = row; node->col = col;
   node->node_function = &node_func;

   create_network_processes( node, level );

   if ( template & ENTRY )
   {
      init_node_stack( node );
      node->visible = node->visited = 1;
   }

   return node;
}


void remove_process_from_node( process_t *proc )
{
   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( current_node( )->processes[ index ] == proc )
      {
         free( proc );
         current_node( )->processes[ index ] = ( process_t * ) 0;
         return;
      }
   }

   return;
}


process_t *add_process_to_node( void )
{
   process_t *proc = ( process_t * ) 0;

   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( current_node( )->processes[ index ] == ( process_t * ) 0 )
      {
         proc = current_node( )->processes[ index ] = calloc( 1, sizeof( process_t ) );
         break;
      }
   }

   return proc;
}


void create_network( int level )
{
   int rows, cols;

   cur_template = getRand( 6 );

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


void free_network( void )
{
   int rows, cols;

   for ( rows = 0 ; rows < NETROWS ; rows++ )
   {
      for ( cols = 0 ; cols < NETCOLS ; cols++ )
      {
         if ( network[ rows ][ cols ] )
         {
            for ( int i = 0 ; i < MAX_PROCESSES_PER_NODE ; i++ )
            {
               if ( network[ rows ][ cols ]->processes[ i ] )
               {
                  free( ( void * ) network[ rows ][ cols ]->processes[ i ] );
               }
            }

            free( ( void * ) network[ rows ][ cols ] );
         }
      }
   }

   return;
}


void execute_network( void )
{
   int row = current_node()->row;
   int col = current_node()->col;
   int active = 0;

   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( is_process_active( current_node()->processes[ index ] ) )
      {
         schedule_process( current_node()->processes[ index ] );
         active++;
      }
   }

   // If this is the exit node, and it's clear, drop into the next level.
   if ( ( templates[ cur_template ][ row ][ col ] & EXIT ) && ( active == 0 ) )
   {
      add_chat_message( "!Level complete.  Process migrating." );

      free_network( );

      increment_level( );

      create_network( get_level( ) );
   }

   return;
}


process_t *find_process_by_pid( unsigned short pid )
{
   process_t *proc = ( process_t * ) 0;

   for ( int i = 0 ; i < MAX_PROCESSES_PER_NODE ; i++ )
   {
      if ( is_process_active( current_node()->processes[ i ] ) )
      {
         if ( current_node()->processes[ i ]->pid == pid )
         {
            proc = current_node()->processes[ i ];
            break;
         }
      }
   }

   return proc;
}


int connect_to_ip_address_from_node ( char *ip_adrs )
{
   // Brute force, but this should work...
   int row = current_node()->row;
   int col = current_node()->col;
   int ret = 0;

   if ( templates[ cur_template ][ row ][ col ] & NOR )
   {
      if ( !strncmp( network[ row-1 ][ col ]->ip_adrs, ip_adrs, 11 ) )
      {
         push_node( current_node() );
         set_current_node( network[ row-1 ][ col ] );
         ret = 1;
      }
   }

   if ( templates[ cur_template ][ row ][ col ] & SOU )
   {
      if ( !strncmp( network[ row+1 ][ col ]->ip_adrs, ip_adrs, 11 ) )
      {
         push_node( current_node() );
         set_current_node( network[ row+1 ][ col ] );
         ret = 1;
      }
   }

   if ( templates[ cur_template ][ row ][ col ] & WES )
   {
      if ( !strncmp( network[ row ][ col-1 ]->ip_adrs, ip_adrs, 11 ) )
      {
         push_node( current_node() );
         set_current_node( network[ row ][ col-1 ] );
         ret = 1;
      }
   }

   if ( templates[ cur_template ][ row ][ col ] & EAS )
   {
      if ( !strncmp( network[ row ][ col+1 ]->ip_adrs, ip_adrs, 11 ) )
      {
         push_node( current_node() );
         set_current_node( network[ row ][ col+1 ] );
         ret = 1;
      }
   }

   if ( ret )
   {
      current_node()->visited = current_node()->visible = 1;
      set_visible_nodes( );
   }

   return ret;
}


void get_process_buffs( int *attack, int *defense )
{
   *attack = *defense = 0;

   for ( int index = 0 ; index < MAX_PROCESSES_PER_NODE ; index++ )
   {
      if ( current_node( )->processes[ index ] )
      {
         if ( current_node( )->processes[ index ]->attributes.buff )
         {
            *attack += current_node( )->processes[ index ]->stats.attack;
            *defense += current_node( )->processes[ index ]->stats.defense;
         }
      }
   }

   return;
}

