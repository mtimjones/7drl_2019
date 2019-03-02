#include <stdio.h>
#include <ctype.h>
#include "headers.h"

#define MAX_ARGS    2
#define MAX_COMMAND_SIZE   12
#define MAX_HELP_SIZE      50

typedef struct
{
   int num_args;
   char *args[ MAX_ARGS ];

} args;

typedef void ( *command )( args *arguments );

typedef struct
{
   char         name [ MAX_COMMAND_SIZE ];
   char         help [ MAX_HELP_SIZE ];
   command      function;
   unsigned int available:1;
   unsigned int take_turn:1;
   unsigned int :0;

} commands;

#define MAX_COMMANDS   16

#define MAX( a, b ) ( ( (a) > (b) ) ? (a) : (b) )

void help_command( args *arguments );

commands command_list[ MAX_COMMANDS ] = {
   { "help",    "Get help about available system commands.", help_command, 1, 0 },
};

void system_command( args *arguments )
{
   for ( int i = 0 ; i < MAX_COMMANDS ; i++ )
   {
      int size = MAX( strlen( arguments->args[ 0 ] ),
                      strlen( command_list[ i ].name ) );

      if ( strncmp( arguments->args[ 0 ], command_list[ i ].name, size ) == 0 )
      {
         ( command_list[ i ].function )( arguments );
         return;
      }
   }

   add_message( "Unknown command." );

   return;
}


void parse_args( char *line, args *arguments )
{
   arguments->num_args = 0;

   while ( *line != 0 )
   {
      // Skip leading white space.
      while ( isspace( *line ) && ( *line != 0 ) ) line++;

      // If at the end of line, return
      if ( *line == 0 ) return;

      // We're at non-white-space, so record this argument.
      arguments->args[ arguments->num_args++ ] = line;

      // Skip this argument.
      while ( *line != ' ' && *line != 0 ) line++;

      // We've found a space, drop a NULL and step to the next char.
      if ( *line == ' ' ) *line++ = 0;

      // If we have MAX_ARGS arguments, just return.
      if ( arguments->num_args == MAX_ARGS ) return;
   }

   return;
}


void system_exec( char *line )
{
   args arguments = { 0, (char*)0, (char*)0 };

   parse_args( line, &arguments );

   if ( arguments.num_args > 0 )
   {
      system_command( &arguments );
   }

   return;
}


void help_command( args *arguments )
{
   char line [ MAX_MSG_SIZE ];

   for ( int i = 0 ; i < MAX_COMMANDS ; i++ )
   {
      sprintf( line, "  %-10s | %s",
               command_list[ i ].name, command_list[ i ].help );
      add_message( line );
   }

   return;
}
