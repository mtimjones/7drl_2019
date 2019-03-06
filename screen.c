#include <ncurses.h>
#include <assert.h>
#include "headers.h"

WINDOW *processwin;
WINDOW *networkwin;
WINDOW *consolewin;
WINDOW *chatwin;

#define CHAT_MSGS         1  // MAGENTA / BLACK
#define CONSOLE_MSGS      2  // CYAN / BLACK


int get_user_char( void )
{
   int c;

   c = wgetch( consolewin );

   if ( c != ERR )
   {
      return c;
   }

   return 0;
}

void win_startup( void )
{
   initscr( );
   clear( );
   if ( has_colors( ) == FALSE ) assert( FALSE );
   start_color( );
   cbreak( );
   noecho( );
   curs_set( 0 );
   nonl( );

   // Need a test for the window size, error if too small...

   processwin = newwin( ULINES, NCOLS, 0, 0 );

   networkwin = newwin( ULINES, CCOLS, 0, NCOLS );

   consolewin = newwin( LLINES, NCOLS, ULINES, 0 );

   nodelay( consolewin, TRUE );
   keypad( consolewin, TRUE );

   chatwin = newwin( LLINES, CCOLS, ULINES, NCOLS );

   init_pair( CHAT_MSGS, COLOR_MAGENTA, COLOR_BLACK );
   init_pair( CONSOLE_MSGS, COLOR_CYAN, COLOR_BLACK );
   

   return;
}


void win_update( void )
{
   int Y, X;
   int bold = 0;

   extern int exit_y, exit_x;

   wborder( processwin, 0, 0, 0, 0, 0, 0, 0, 0 );
   wborder( networkwin, 0, 0, 0, 0, 0, 0, 0, 0 );
   wborder( consolewin, 0, 0, 0, 0, 0, 0, 0, 0 );
   wborder( chatwin, 0, 0, 0, 0, 0, 0, 0, 0 );
   wattron( processwin, A_STANDOUT );
   mvwprintw( processwin, 0, 2, " Process Window " );
   wattroff( processwin, A_STANDOUT );
   wattron( networkwin, A_STANDOUT );
   mvwprintw( networkwin, 0, 2, " Network Window " );
   wattroff( networkwin, A_STANDOUT );
   wattron( consolewin, A_STANDOUT );
   mvwprintw( consolewin, 0, 2, " Console Window " );
   wattroff( consolewin, A_STANDOUT );
   wattron( chatwin, A_STANDOUT );
   mvwprintw( chatwin, 0, 2, " Message Window " );
   wattroff( chatwin, A_STANDOUT );

   mvwprintw( networkwin, 0, 40, "%6d ", (GameTime/100) );

   wattron( consolewin, COLOR_PAIR( CONSOLE_MSGS ) );
   for ( int i = 0 ; i < MAX_MESSAGES ; i++ )
   {
      mvwprintw( consolewin, ( i+1 ), 1, "%s", get_message( i ) );
   }
   wattroff( consolewin, COLOR_PAIR( CONSOLE_MSGS ) );

   wattron( chatwin, COLOR_PAIR( CHAT_MSGS ) );
   for ( int i = 0 ; i < MAX_CHAT_MESSAGES ; i++ )
   {
      char *msg = get_chat_message( i );
      if ( msg[0] == '!')
      {
         wattron( chatwin, A_BOLD );
         msg++;
         bold = 1;
      }
      mvwprintw( chatwin, ( i+1 ), 1, "%s", msg );
      if ( bold )
      {
         wattroff( chatwin, A_BOLD );
         bold = 0;
      }
   }
   wattroff( chatwin, COLOR_PAIR( CHAT_MSGS ) );

   wattron( consolewin, COLOR_PAIR( CONSOLE_MSGS ) );
   mvwprintw( consolewin, LLINES-2, 1, 
              "                                                          " );
   mvwprintw( consolewin, LLINES-2, 1, "%s", get_user_input_line() );
   wattroff( consolewin, COLOR_PAIR( CONSOLE_MSGS ) );

   mvwprintw( processwin, 1, 4, "PID.  Name......  Rate  Att  Def  Lvl  Attr.  Energy" );

   display_network_processes( processwin );

   display_network( networkwin );

   wrefresh( processwin );
   wrefresh( networkwin );
   wrefresh( consolewin );
   wrefresh( chatwin );
   
   return;
}


void clear_network_window( void )
{
   for ( int i = 1 ; i < 13 ; i++ )
   {
      mvwprintw( networkwin, i, 1, "                                             " );
   }

   return;
}


void win_shutdown( void )
{
   delwin( consolewin );

   endwin( );

   return;
}

