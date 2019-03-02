#include <stdio.h>
#include "headers.h"

unsigned int GameRunning = 1;
unsigned int GameTime = 0;
int          Level = 1;

int main( int argc, char *argv[] )
{
   srand( time( NULL ) );

   init_user_input( );

   init_messages( );

   win_startup( );

   create_player( );
   create_network( Level );

   add_message( "CyberOS uKernel started." );
   add_message( "CortextLink detected... attached to ulogin." );
   add_message( "" );
   add_message( "Type help for more information." );

   add_chat_message( "** Chat connection opened." );
   add_chat_message( "It was a decade since you last jacked in, but" );
   add_chat_message( "this was old hardware and the perfect job for" );
   add_chat_message( "your aging expertise.  This is your last" );
   add_chat_message( "opportunity to clear your name and escape the" );
   add_chat_message( "shadows.  Your goal is to dive deep into" );
   add_chat_message( "Sense/Net's corporate network to defeat the" );
   add_chat_message( "executive process and bring the corporation" );
   add_chat_message( "down." );

   win_update( );

   while ( GameRunning )
   {
      unsigned long long start = getTimestamp( );

      handle_user_input( );

      // blinky lights.

      // Update the window
      win_update( );

      while ( getTimestamp( ) < start + MS_PER_FRAME );

      GameTime++;
   }

   win_shutdown( );

   return 0;
}

