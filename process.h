#include <stdio.h>

typedef enum {
   User = 0,
   Worker,
   Daemon,
   Sentry,
   Encrypt,
   Upgrade,
   Fork,
   Minion,
   Random,
   Executive,
   Total
} process_type_t;

typedef struct stats_t
{
   int attack;
   int defense;
   int energy;
   int max_energy;
   int level;
   int base_damage;
   int ext_damage;
   int xp;
   int xp_to_next_level;
   int hack_successes;
   int hack_failures;
   int kills;
   int health_added;
   int damage_given;
   int damage_received;
} stats_t;

typedef struct attributes_t
{
   unsigned int hackable:1;
   unsigned int active:1;
   unsigned int user:1;
   unsigned int attack:1;
   unsigned int buff:1;
   unsigned int special:1;
   unsigned int :0;
} attributes_t;

typedef struct process_s process_t;

typedef void ( process_func_t )( process_t *, int );

#define MAX_NAME   20

typedef struct process_s
{
   process_type_t process_type;
   process_func_t *function;
   unsigned short pid;
   char name[ MAX_NAME ];
   stats_t stats;
   attributes_t attributes;
 //  process_state_t state;
   int state_value;
   int action_rate; // 1 = every turn.
} process_t;

void create_player( void );
process_t *create_process( process_t *proc, process_type_t type, int level );
void display_network( WINDOW *window );
void display_network_processes( WINDOW *window );
void display_player( WINDOW *window, int line );
void display_process( WINDOW *window, int line, process_t *process );
int  is_process_active( process_t *process );
void schedule_process( process_t *process );
int  hit( int attack, int defense );
int  getPlayerDefense( void );
int  getPlayerAttack( void );
void damageProcess( process_t *process, int damage );
int  isPlayerDead( void );
process_t *GetPlayer( void );
void healPlayer( int energy );
void remove_process_from_node( process_t *process );
process_t *add_process_to_node( void );
