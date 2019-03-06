
#define PROCESS_INIT      0
#define PROCESS_EXECUTE   1
#define PROCESS_DAMAGE    2

void daemon_behavior( process_t *process, int action );
void fork_behavior( process_t *process, int action );
void sentry_behavior( process_t *process, int action );
void armor_behavior( process_t *process, int action );

