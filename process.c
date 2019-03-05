// Functions for processes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

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
   ulogin.stats.max_energy = ulogin.stats.energy = 20;
   ulogin.stats.level = 1;
   ulogin.stats.xp = 0;
   ulogin.stats.xp_to_next_level = 20;

   ulogin.action_rate = 1;

   return;
}

process_t *GetPlayer( void )
{
   return &ulogin;
}

process_t *create_process( process_t *process, process_type_t type, int level )
{
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
         process->stats.attack = level+1;
         process->stats.defense = level;
         process->stats.max_energy = process->stats.energy = 10+getRand((level*2));
         process->action_rate = getRand(3)+1;
         process->function = &daemon_behavior;
         process->stats.base_damage = level;
         process->stats.ext_damage = level;
         break;
      case Worker:
         strcpy( process->name, "kworker" );
         process->process_type = type;
         process->pid = getRand( 9999 );
         process->attributes.hackable = 1;
         process->attributes.active = 1;
         process->attributes.attack = 1;
         process->stats.level = level;
         process->stats.attack = level;
         process->stats.defense = level+1;
         process->stats.max_energy = process->stats.energy = 10+level;
         process->action_rate = getRand(2)+1;
         process->function = &daemon_behavior;
         process->stats.base_damage = level;
         process->stats.ext_damage = level;
         break;
      case Fork:
         strcpy( process->name, "fork" );
         process->process_type = type;
         process->pid = getRand( 9999 );
         process->attributes.hackable = 0;
         process->attributes.active = 1;
         process->attributes.special = 1;
         process->stats.level = level;
         process->stats.attack = 0;
         process->stats.defense = level+3;
         process->stats.max_energy = process->stats.energy = 20+level;
         process->action_rate = 2;
         process->function = &fork_behavior;
         process->stats.base_damage = 0;
         process->stats.ext_damage = 0;
         break;
      case Minion:
         strcpy( process->name, "minion" );
         process->process_type = type;
         process->pid = getRand( 9999 );
         process->attributes.hackable = 1;
         process->attributes.active = 1;
         process->attributes.attack = 1;
         process->stats.level = level;
         process->stats.attack = level;
         process->stats.defense = level + getRand( 2 );
         process->stats.max_energy = process->stats.energy = 10+2*level;
         process->action_rate = 1;
         process->function = &daemon_behavior;
         process->stats.base_damage = 2;
         process->stats.ext_damage = 2*level;
         break;
      case Sentry:
         strcpy( process->name, "sentry" );
         process->process_type = type;
         process->pid = getRand( 9999 );
         process->attributes.hackable = 0;
         process->attributes.buff = 1;
         process->attributes.active = 1;
         process->attributes.attack = 0;
         process->stats.level = level;
         process->stats.attack = 0;
         process->stats.defense = level + getRand( 2 );
         process->stats.max_energy = process->stats.energy = 15+2*level;
         process->action_rate = 1;
         process->stats.base_damage = 0;
         process->stats.ext_damage = 0;
         break;

      default:
         break;
   }

   return process;
}


void display_process( WINDOW *window, int line, process_t *process )
{
   int attack, defense;

   get_process_buffs( &attack, &defense );

   if ( process->attributes.active )
   {
      mvwprintw( window, line,  4, "%4d  ", process->pid );
      mvwprintw( window, line, 10, "%-10s  ", process->name );
// display if process not suspended.
      mvwprintw( window, line, 22, "%3d  ", (100 / process->action_rate ) );
      mvwprintw( window, line, 26, "%3d  ", process->stats.attack + attack );
      mvwprintw( window, line, 31, "%3d  ", process->stats.defense + defense );
      mvwprintw( window, line, 36, "%3d       ", process->stats.level );
      mvwprintw( window, line, 49, "%3d  ", process->stats.energy );

      mvwprintw( window, line, 43, "     " );
      if ( process->attributes.user )
      {
         mvwprintw( window, line, 43, "@" );
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
      if ( process->attributes.hackable )
      {
         mvwprintw( window, line, 46, "H" );
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


void execute_process( process_t *process )
{
   if ( is_process_active( process ) )
   {
      if ( ++process->state_value >= process->action_rate )
      {
         process->state_value = 0;
         if ( process->function )
         {
            ( process->function )( process );
         }
      }
   }
}


int hit( int attack, int defense )
{
   return ( getSRand( ) < ( (float)attack / ( (float)attack + (float)defense ) ) );
}


int getPlayerDefense( void )
{
   return ulogin.stats.defense;
}


int getPlayerAttack( void )
{
   return ulogin.stats.attack;
}

void damageProcess( process_t *process, int damage )
{
   if ( process->process_type == User )
   {
      ulogin.stats.damage_received += damage;
   }
   else
   {
      ulogin.stats.damage_given += damage;
   }

   process->stats.energy -= damage;
   if ( process->stats.energy <= 0 )
   {
      ulogin.stats.kills++;
      process->stats.energy = 0;
      process->attributes.active = 0;
      char line[ 80 ];
      sprintf( line, "[%04d] Process has exited.", process->pid );
      add_message( line );

      // Add xp earned to the player.
      if ( process->process_type != User )
      {
         ulogin.stats.xp += ( ( process->stats.attack + process->stats.defense ) );
         if ( ulogin.stats.xp >= ulogin.stats.xp_to_next_level )
         {
            char line[ 80 ];
            ulogin.stats.xp_to_next_level = 
               ( int ) ( ( float ) ulogin.stats.xp_to_next_level * 1.7 );

            sprintf( line, "![%d] Increased level.", ulogin.pid );
            add_chat_message( line );

            ulogin.stats.max_energy *= 1.2;
            ulogin.stats.energy = ulogin.stats.max_energy;
            ulogin.stats.level++;

            if ( getRand( 10 ) > 5 ) 
            {
               ulogin.stats.attack++;
               sprintf( line, "![%04d] Attack increased.", ulogin.pid );
               add_chat_message( line );
            }
            else 
            {
               ulogin.stats.defense++;
               sprintf( line, "![%04d] Defense increased.", ulogin.pid );
               add_chat_message( line );
            }
         }

         remove_process_from_node( process );
      }

   }

   return;
}

void healPlayer( int energy )
{
   ulogin.stats.energy += energy;
   ulogin.stats.health_added += energy;
   if ( ulogin.stats.energy > ulogin.stats.max_energy )
   {
      ulogin.stats.energy = ulogin.stats.max_energy;
   }
}

int isPlayerDead( void )
{
   return ( ulogin.stats.energy == 0 );
}

