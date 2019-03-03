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

#define MAX_COMMANDS   5

#define MAX( a, b ) ( ( (a) > (b) ) ? (a) : (b) )

void help_command( args *arguments );
void wait_command( args *arguments );
void bash_command( args *arguments );
void redo_command( args *arguments );
void hack_command( args *arguments );

commands command_list[ MAX_COMMANDS ] = {
   { "help", "Get help about available system commands.", help_command, 1, 0 },
   { ".",    "Redo last command.",                        redo_command, 1, 1 },
   { "wait", "Wait, skip a turn.",                        wait_command, 1, 1 },
   { "bash", "Bash a process (by pid) for 1-3 damage.",   bash_command, 1, 1 },
   { "hack", "Hack a process to gain energy.",            hack_command, 1, 1 },
};

int system_command( args *arguments )
{
   for ( int i = 0 ; i < MAX_COMMANDS ; i++ )
   {
      int size = MAX( strlen( arguments->args[ 0 ] ),
                      strlen( command_list[ i ].name ) );

      if ( strncmp( arguments->args[ 0 ], command_list[ i ].name, size ) == 0 )
      {
         ( command_list[ i ].function )( arguments );
         return command_list[ i ].take_turn;
      }
   }

   add_message( "Unknown command." );

   return 0;
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


int system_exec( char *line )
{
   args arguments = { 0, (char*)0, (char*)0 };

   parse_args( line, &arguments );

   if ( arguments.num_args > 0 )
   {
      return system_command( &arguments );
   }

   return 0;
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


void wait_command( args *arguments )
{
   return;
}


void bash_command( args *arguments )
{
   process_t *proc;

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is bash <pid>" );
   }
   else
   {
      proc = find_process_by_pid( atoi( arguments->args[ 1 ] ) );
      if ( proc == (process_t *)0 )
      {
         add_message( "Pid not found." );
      }
      else
      {
         char line[80];
         if ( hit( getPlayerAttack( ), proc->stats.defense ) )
         {
            int  damage = 1 + getRand( 3 );
            damageProcess( proc, damage );
            sprintf( line, "Process %04d hit for %d", proc->pid, damage );
         }
         else
         {
            sprintf( line, "Process %04d defends.", proc->pid );
         }
         add_chat_message( line );
      }
   }

   return;
}


void redo_command( args *arguments )
{
   // This command is implemented at a higher level and therefore
   // won't be called here.
   return;
}


void hack_command( args *arguments )
{
   // disable hackable for the process.   
   process_t *proc;

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is bash <pid>" );
   }
   else
   {
      proc = find_process_by_pid( atoi( arguments->args[ 1 ] ) );
      if ( proc == (process_t *)0 )
      {
         add_message( "Pid not found." );
      }
      else
      {
         if ( !proc->attributes.hackable )
         {
            add_message( "Process cannot be hacked." );
         }
         else
         {
            int limit = 2 + proc->stats.level;

            // @TODO: Need to escalate time with limit.

            init_hack( limit, 100, proc );
         }
      }
   }

   return;
}


