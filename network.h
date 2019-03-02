
#define MAX_IP_ADRS              12
#define MAX_PROCESSES_PER_NODE   11

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
   char ip_adrs[ MAX_IP_ADRS ];
   node_t *connections[ 4 ];
   process_t *processes[ MAX_PROCESSES_PER_NODE ];
   unsigned int visited:1;
   unsigned int visible:1; // Auto set for entry node
   unsigned int :0;
   node_function node_entry_function;
   node_function node_exit_function;
} node_t;

void create_network( int level );

