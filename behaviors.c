#include <stdio.h>
#include "headers.h"

void daemon_behavior( process_t *process )
{
   static int i = 0;

   if ( !i )
   {
      char line[80];
      sprintf( line, "[%04d] Daemon says hi.", process->pid );
      add_chat_message( line );
//      i = 1;
   }

   return;
}
