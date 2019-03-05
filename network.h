
#define MAX_IP_ADRS              12
#define MAX_PROCESSES_PER_NODE   10

typedef struct node_s node_t;

typedef enum
{
   net_up = 0,
   net_right,
   net_down,
   net_left,
} net_dir;

typedef void ( *node_function )( node_t *node );

typedef struct node_s
{
   unsigned int node_type; // Index to string that is emitted upon entering node.
   int row;
   int col;
   char ip_adrs[ MAX_IP_ADRS ];
   process_t *processes[ MAX_PROCESSES_PER_NODE ];
   unsigned int visited:1;
   unsigned int visible:1; // Auto set for entry node
   unsigned int :0;
   node_function node_entry_function;
   node_function node_exit_function;
} node_t;

void create_network( int level );
void execute_network( void );
process_t *find_process_by_pid( unsigned short pid );

int connect_to_ip_address_from_node( char *ip_adrs );

void get_process_buffs( int *attack, int *defense );

