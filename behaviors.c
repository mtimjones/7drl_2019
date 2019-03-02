#include <stdio.h>
#include "headers.h"

void daemon_behavior( process_t *process )
{
   static int i = 0;

   if ( !i )
   {
      add_chat_message( "Daemon says hi." );
      i = 1;
   }

   return;
}
