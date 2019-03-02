#include <stdio.h>
#include "headers.h"

void daemon_behavior( process_t *process )
{
   char line[80];

   if ( hit( process->stats.attack, getPlayerDefense( ) ) )
   {
      int damage = process->stats.base_damage + getRand( process->stats.ext_damage );
      sprintf( line, "[%04d] Daemon hits for %d.", process->pid, damage );
      add_chat_message( line );
      damagePlayer( damage );
   }

   return;
}





