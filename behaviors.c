#include <stdio.h>
#include "headers.h"

void daemon_behavior( process_t *process, int action )
{
   char line[80];
   int attack, defense;

   if ( action == PROCESS_INIT )
   {
   }
   else if ( action == PROCESS_EXECUTE )
   {
      if ( ++process->state_value >= process->action_rate )
      {
         process->state_value = 0;

         get_process_buffs( &attack, &defense );

         if ( hit( process->stats.attack + attack, getPlayerDefense( ) ) )
         {
            int damage = process->stats.base_damage + 
                         getRand( process->stats.ext_damage );
            sprintf( line, "[%04d] %s hits for %d.", 
                     process->pid, process->name, damage );
            add_chat_message( line );
            damageProcess( GetPlayer( ), damage );
         }

      }

   }

   return;
}


void fork_behavior( process_t *process, int action )
{
   if ( action == PROCESS_INIT )
   {
   }
   else if ( action == PROCESS_EXECUTE )
   {
      if ( ++process->state_value >= process->action_rate )
      {
         process_t *minion = add_process_to_node( );

         process->state_value = 0;

         // If a slot was available to fill, initialize the minion.
         if ( minion )
         {
            char line[ 80 ];

            ( void ) create_process( minion, Minion, get_level( ) );
      
            sprintf( line, "%s [%04d]: spawned a new %s [%04d]",
                     process->name, process->pid, minion->name, minion->pid );

            add_message( line );
         }
      }
   }

   return;
}


