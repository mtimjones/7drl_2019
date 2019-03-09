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
      
            sprintf( line, "[%04d]: %s spawned a new %s [%04d]",
                     process->pid, process->name, minion->name, minion->pid );

            add_message( line );
         }
      }
   }

   return;
}


void sentry_behavior( process_t *process, int action )
{
   if ( action == PROCESS_INIT )
   {
      char line[ 80 ];
      sprintf( line, "[%04d]: %s buffing attack.", process->pid, process->name );
      add_message( line );
   }

   return;
}


void armor_behavior( process_t *process, int action )
{
   if ( action == PROCESS_INIT )
   {
      char line[ 80 ];
      sprintf( line, "[%04d]: %s buffing defense.", process->pid, process->name );
      add_message( line );
   }

   return;
}


void cron_behavior( process_t *process, int action )
{
   char line[ 80 ];
   int  attack, defense;

   if ( action == PROCESS_INIT )
   {
      sprintf( line, "[%04d]: %s is charging.", process->pid, process->name );
      add_message( line );
   }
   else if ( action == PROCESS_EXECUTE )
   {
      if ( ++process->state_value == ( process->action_rate / 2 ) )
      {
         sprintf( line, "[%04d]: %s is half charged.", process->pid, process->name );
         add_message( line );
      }
      else if ( ++process->state_value >= process->action_rate )
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


void encrypt_behavior( process_t *process, int action )
{
   if ( action == PROCESS_INIT )
   {
      char line[ 80 ];
      sprintf( line, "[%04d]: %s concealing pids.", getRand( 9998 )+1 , process->name );
      add_message( line );
   }

   return;
}


void executive_behavior( process_t *process, int action )
{
   char line[ 80 ];
   int  attack, defense;
   static int lives = 2;

   if ( action == PROCESS_INIT )
   {
      sprintf( line, "[%04d] %s: prepare to die.", process->pid, process->name );
      add_message( line );
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
   else if ( action == PROCESS_DEATH )
   {
      if ( --lives == 0 )
      {
      }
      else
      {
         process->stats.energy = 40;
         process->attributes.active = 1;
         sprintf( line, "[%04d] %s respawned.", 
                  process->pid, process->name );
         add_message( line );
      }

   }

   return;
}


