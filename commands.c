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

#define MAX_COMMANDS   12

#define MAX( a, b ) ( ( (a) > (b) ) ? (a) : (b) )

void help_command( args *arguments );
void wait_command( args *arguments );
void bash_command( args *arguments );
void redo_command( args *arguments );
void hack_command( args *arguments );
void connect_command( args *arguments );
void exit_command( args *arguments );
void host_command( args *arguments );
void stats_command( args *arguments );
void cut_command( args *arguments );
void slice_command( args *arguments );
void kill_command( args *arguments );

commands command_list[ MAX_COMMANDS ] = {
   { "help",    "Get help about available system commands.",  help_command,    1, 0 },
   { ".",       "Redo last command.",                         redo_command,    1, 1 },
   { "host",    "Print information about the current host.",  host_command,    1, 0 },
   { "connect", "Connect to a node by its IP address.",       connect_command, 1, 0 },
   { "exit",    "Return to the previously connected system.", exit_command,    1, 0 },
   { "wait",    "Wait, skip a turn.",                         wait_command,    1, 1 },
   { "bash",    "Bash a process (by pid) for 1-3 damage.",    bash_command,    1, 1 },
   { "hack",    "Hack a process with arrow keys for energy.", hack_command,    1, 1 },
   { "stats",   "Print stats about the ulogin process.",      stats_command,   1, 0 },
   { "cut",     "Cut a process (by pid) for 3-6 damage.",     cut_command,     0, 1 },
   { "slice",   "Slice a process (by pid) and its neihbors.", slice_command,   0, 1 },
   { "kill",    "Kill a process (by pid) for 6-9 damage.",    kill_command,    0, 1 },
};


void learn_new_command( int user_level )
{
   const int learn_a_new_command[ 10 ] = { 0, 0, 1, 0, 1, 0, 1, 0, 0, 0 };

   // Learn a new command every other level reached.
   if ( learn_a_new_command[ user_level-1 ] )
   {
      for ( int i = 0 ; i < MAX_COMMANDS ; i++ )
      {
         if ( !command_list[ i ].available )
         {
            char line[ 80 ];
            command_list[ i ].available = 1;
            sprintf( line, "![%04d] ulogin learned %s.",
                  GetPlayer( )->pid, command_list[ i ].name );
            add_chat_message( line );
            return;
         }
      }
   }

   return;
}


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
      if ( command_list[ i ].available )
      {
         sprintf( line, "  %-10s | %s",
                  command_list[ i ].name, command_list[ i ].help );
         add_message( line );
      }
   }

   return;
}


void wait_command( args *arguments )
{
   return;
}


void attack ( int attack, process_t *proc, int base, int ext )
{
   int attack_buf, defense_buf;

   get_process_buffs( &attack_buf, &defense_buf );

   if ( hit( attack, proc->stats.defense + defense_buf ) )
   {
      char line[80];
      int  damage = base + getRand( ext );

      damageProcess( proc, damage );
      sprintf( line, "[%04d] hit for %d energy", proc->pid, damage );
      add_chat_message( line );
   }

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
         attack( getPlayerAttack( ), proc, 2, 2 );
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
            proc->attributes.hackable = 0;

            init_hack( limit, 400, proc );
         }
      }
   }

   return;
}


void connect_command( args *arguments )
{
   int ret;
   char line[ 80 ];

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is connect <ip_adress>" );
   }
   else
   {
      ret = connect_to_ip_address_from_node( arguments->args[ 1 ] );

      if ( ret )
      {
         sprintf( line, "Connected to %s", arguments->args[ 1] );
         ( current_node( )->node_function )( current_node( ), NODE_ENTRY );
      }
      else
      {
         sprintf( line, "Unable to access %s", arguments->args[ 1] );
      }
      add_message( line );
   }

   return;
}


void exit_command( args *arguments )
{
   if ( node_stack_empty( ) )
   {
      add_message( "Unable to exit current node." );
      return;
   }

   // Cannot exit system if special is there.
   set_current_node( pop_node( ) );

}


void host_command( args *argument )
{
   node_t *node = current_node( );

   // Need to emit a host line including some other info.

   add_message( node->ip_adrs );

   return;
}


void stats_command( args *arguments )
{
   char line[ 80 ];
   process_t *proc = GetPlayer( );

   sprintf( line, "[%04d] Current XP       : %d", proc->pid, proc->stats.xp );
   add_message( line );
   sprintf( line, "[%04d] XP to next level : %d", proc->pid, proc->stats.xp_to_next_level );
   add_message( line );
   sprintf( line, "[%04d] Hack sucesses    : %d", proc->pid, proc->stats.hack_successes );
   add_message( line );
   sprintf( line, "[%04d] Hack failures    : %d", proc->pid, proc->stats.hack_failures );
   add_message( line );
   sprintf( line, "[%04d] Processes killed : %d", proc->pid, proc->stats.kills );
   add_message( line );
   sprintf( line, "[%04d] Health added     : %d", proc->pid, proc->stats.health_added );
   add_message( line );
   sprintf( line, "[%04d] Damage given     : %d", proc->pid, proc->stats.damage_given );
   add_message( line );
   sprintf( line, "[%04d] Damage received  : %d", proc->pid, proc->stats.damage_received );
   add_message( line );

   return;
}

void cut_command( args *arguments )
{
   process_t *proc;

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is cut <pid>" );
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
         attack( getPlayerAttack( ), proc, 3, 4 );
      }
   }

   return;
}


void slice_command( args *arguments )
{
   process_t *proc;
   unsigned short pid;

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is slice <pid>" );
   }
   else
   {
      pid = atoi ( arguments->args[ 1 ] );

      proc = find_process_by_pid( pid );

      if ( proc == (process_t *)0 )
      {
         add_message( "Pid not found." );
      }
      else
      {
         attack( getPlayerAttack( ), proc, 3, 3 );

         proc = find_process_before_pid( pid );
         if ( proc )
         {
            attack( getPlayerAttack( ), proc, 2, 2 );
         }

         proc = find_process_after_pid( pid );
         if ( proc )
         {
            attack( getPlayerAttack( ), proc, 2, 2 );
         }
      }
   }

   return;
}


void kill_command( args *arguments )
{
   process_t *proc;

   if ( arguments->num_args < 2 ) 
   {
      add_message( "Usage is cut <pid>" );
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
         attack( getPlayerAttack( ), proc, 6, 4 );
      }
   }

   return;
}


