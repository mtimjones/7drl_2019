// Functions for processes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <headers.h>
#include "process.h"

process_t ulogin;

void create_player( void )
{
   memset( ( void * )&ulogin, 0, sizeof( process_t ) );

   strcpy( ulogin.name, "ulogin" );
   ulogin.process_type = User;
   ulogin.pid = getRand( 9999 );
   ulogin.attributes.user = 1;
   ulogin.attributes.active = 1;

   ulogin.stats.attack = 2;
   ulogin.stats.defense = 2;
   ulogin.stats.energy = 15;
   ulogin.stats.level = 1;

   ulogin.action_rate = 1;

   return;
}

process_t *create_process( process_type_t type, int level )
{
   process_t *process = calloc( 1, sizeof( process_t ) );

   switch( type )
   {
      case Daemon:
         strcpy( process->name, "daemon" );
         process->process_type = type;
         process->pid = getRand( 9999 );
         process->attributes.hackable = 1;
         process->attributes.active = 1;
         process->attributes.attack = 1;
         process->stats.level = level;
         process->stats.attack = level;
         process->stats.defense = level;
         process->stats.energy = 10+getRand((level*4));
         process->action_rate = getRand(2)+1;
         break;

      default:
         break;
   }

   return process;
}


void display_process( WINDOW *window, int line, process_t *process )
{
   if ( process->attributes.active )
   {
      mvwprintw( window, line,  4, "%4d  ", process->pid );
      mvwprintw( window, line, 10, "%-10s  ", process->name );
// display if process not suspended.
      mvwprintw( window, line, 22, "%3d  ", (100 / process->action_rate ) );
      mvwprintw( window, line, 26, "%3d  ", process->stats.attack );
      mvwprintw( window, line, 31, "%3d  ", process->stats.defense );
      mvwprintw( window, line, 36, "%3d       ", process->stats.level );
      mvwprintw( window, line, 49, "%3d  ", process->stats.energy );

      if ( process->attributes.user )
      {
         mvwprintw( window, line, 43, "U" );
      }
      if ( process->attributes.attack )
      {
         mvwprintw( window, line, 44, "A" );
      }
      if ( process->attributes.buff )
      {
         mvwprintw( window, line, 45, "B" );
      }
      if ( process->attributes.special )
      {
         mvwprintw( window, line, 46, "S" );
      }
   }

   return;
}

void display_player( WINDOW *window, int line )
{
   display_process( window, line, &ulogin );
}

int is_process_active( process_t *process )
{
   if ( process )
   {
      return ( process->attributes.active );
   }

   return 0;
}

