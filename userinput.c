#include "headers.h"
#include <ncurses.h>
#include <ctype.h>

#define MAX_LINE (NCOLS-1)

static char user_line[ MAX_LINE ];
static int cur_char;
static char last_line[ MAX_LINE ] = { 0 };

void init_user_input( void )
{
   cur_char = 0;
   user_line[ cur_char++ ] = '$';
   user_line[ cur_char++ ] = ' ';
   user_line[ cur_char ] = 0;
}

void invoke_user_command( void )
{
   int take_turn;

   add_message( user_line );

   take_turn = system_exec( &user_line[ 2 ] );

   init_user_input( );

   if ( take_turn )
   {
      execute_network( );
   }

   return;
}

void handle_user_input( void )
{
   int c = get_user_char( );

   if ( c == 0 )
   {
   }
   else if ( c == 0x0d )
   {
      if ( user_line[2] == '.' )
      {
         strcpy( user_line, last_line );
      }
      else
      {
         strcpy( last_line, user_line );
      }

      invoke_user_command( );
   }
   else if ( isprint( c ) )
   {
      if ( cur_char < MAX_LINE )
      {
         user_line[ cur_char++ ] = ( char )c;
         user_line[ cur_char ] = 0;
      }
   }
   else if ( c == KEY_BACKSPACE )
   {
      if ( cur_char > 2 )
      {
         user_line[ --cur_char ] = 0;
      }
   }

   return;
}

char *get_user_input_line( void )
{
   return user_line;
}

