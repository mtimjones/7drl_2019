#include "headers.h"
#include <ncurses.h>
#include <ctype.h>

#define MAX_LINE (NCOLS-1)

static char user_line[ MAX_LINE ];
static int cur_char;
static char last_line[ MAX_LINE ] = { 0 };

#define NORMAL    0
#define HACK      1

#define DOWN      0
#define UP        1
#define LEFT      2
#define RIGHT     3

static int mode = NORMAL;

struct hackstruct
{
   char sequence[ 8 ];
   int cur_seq;
   int seq_length;
   int timeleft;
   process_t *target;
} hack;

void init_hack( int len, int time, process_t *process )
{
   char line[ 80 ] = { 0 };
   char *names[4] = { "DOWN ", "UP ", "LEFT ", "RIGHT " };

   strcat( line, "Hack: " );

   for ( int i = 0 ; i < len ; i++ )
   {
      hack.sequence[ i ] = getRand(4);
      strcat( line, names[ hack.sequence[ i ] ] );
   }

   add_message( line );

   hack.seq_length = len;
   hack.cur_seq    = 0;
   hack.timeleft   = time;
   hack.target     = process;

   mode = HACK;

   return;
}

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

   if ( mode == NORMAL )
   {
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
   }
   else if ( mode == HACK )
   {
      if ( c != 0 )
      {
         c -= 0x102;
         if ( c == hack.sequence[ hack.cur_seq ] )
         {
            if ( ++hack.cur_seq == hack.seq_length )
            {
               add_chat_message( "Hack success." );
               healPlayer( getRand( hack.target->stats.max_energy ) );
               hack.target->attributes.active = 0;
               mode = NORMAL;
               return;
            }
         }
         else
         {
            add_chat_message( "Hack failed." );
            mode = NORMAL;
            hack.target->attributes.hackable = 0;
         }
      }
      if ( --hack.timeleft == 0 )
      {
         // time has run out.
         add_chat_message( "Hack timeout." );
         mode = NORMAL;
      }
   }

   return;
}

char *get_user_input_line( void )
{
   return user_line;
}

