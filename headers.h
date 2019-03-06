#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <strings.h>
#include <string.h>
#include <ncurses.h>

#include "process.h"
#include "network.h"
#include "behaviors.h"

// Size of the UI
#define ULINES      ( 15 )
#define LLINES      ( 22 )
#define NCOLS       ( 60 )
#define CCOLS       ( 50 )

// 100Hz Game Loop
#define MS_PER_FRAME  10

// Max number of window messages
#define MAX_MESSAGES       ( LLINES - 3 )
#define MAX_MSG_SIZE       ( NCOLS - 2  )
#define MAX_CHAT_MESSAGES  ( LLINES - 2 )
#define MAX_CHAT_MSG_SIZE  ( CCOLS - 2  )

// Messages API
void init_messages( void );
void add_message( char *msg );
char* get_message( int pos );
void add_chat_message( char *msg );
char *get_chat_message( int pos );

// Screen API
void win_startup( void );
void win_update( void );
void win_shutdown( void );
int  get_user_char( void );
void clear_network_window( void );

// Time API
unsigned long long getTimestamp( void );
extern unsigned int GameTime;

// User Input API
void init_user_input( void );
void handle_user_input( void );
char *get_user_input_line( void );
int system_exec( char *line );
void init_hack( int len, int time, process_t *proc );

#if 0
// Helpers API
void parse_args( char *line, args *arguments );
int  parse_attribute( char *line, char *attribute );
int  find_file( int system, char *filename );
int  find_empty_file( int system );
int  find_empty_process( void );
int  find_process( unsigned short pid );
int  find_system( char *ip_address );
void move_file( int to, int from, char *file );
int  can_execute( void );
#endif

// Node stack API
void    init_node_stack( node_t *node );
node_t *current_node( void );
int     node_stack_empty( void );
void    set_current_node( node_t *node );
void    push_node( node_t *node );
node_t *pop_node( void );

// RNG Functions 
#define getSRand()      ( ( float ) rand( ) / ( float ) RAND_MAX )
#define getRand( x )    ( int ) ( ( x ) * getSRand( ) )

int get_level( void );
int increment_level( void );
