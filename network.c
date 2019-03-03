#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include "headers.h"

node_t *cur_node;

void display_network( WINDOW *win )
{
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


node_t *create_node( int level )
{
   node_t *proc = calloc( 1, sizeof( node_t ) );

   snprintf( proc->ip_adrs, MAX_IP_ADRS, "%2d.%2d.%2d.%2d", 
               level+1, getRand(99), getRand(99), getRand(98)+1 );

   create_network_processes( proc, level );

   return proc;
}


void create_network( int level )
{
   cur_node = create_node( level );

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

