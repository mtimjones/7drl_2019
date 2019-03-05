#include <stdio.h>
#include "headers.h"

void daemon_behavior( process_t *process )
{
   char line[80];

   if ( hit( process->stats.attack, getPlayerDefense( ) ) )
   {
      int damage = process->stats.base_damage + getRand( process->stats.ext_damage );
      sprintf( line, "[%04d] %s hits for %d.", process->pid, process->name, damage );
      add_chat_message( line );
      damageProcess( GetPlayer( ), damage );
   }

   return;
}


void fork_behavior( process_t *process )
{
   process_t *minion = add_process_to_node( );

   // If a slot was available to fill, initialize the minion.
   if ( minion )
   {
      ( void ) create_process( minion, Minion, get_level( ) );
   }

   return;
}


