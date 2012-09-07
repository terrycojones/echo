/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/agents.h,v 1.6 1992/11/16 07:05:45 terry Exp terry $ */
extern AGENT *create_agent();
extern AGENT_EDIT *new_agent_edit();
#ifndef NO_SELF_REPLICATION
extern BOOLEAN can_self_replicate();
#endif
extern void calculate_makeup();
#ifndef NO_MAINTENANCE
extern BOOLEAN can_pay_maintenance();
#endif
#ifndef NO_MUTATION
extern COUNT mutate_agent();
#endif
extern void print_agent();
#ifndef NO_SELF_REPLICATION
extern AGENT *agent_self_replicate();
#endif
extern void write_agent_to_file();
extern void write_agent_edit_to_file();
extern AGENT *read_agent_from_file();
extern AGENT_EDIT *read_agent_edit_from_file();
#ifndef NO_MIGRATION
extern void migrate_agent();
#endif
extern void make_agent_genome();
extern AGENT_POPULATION_SIZE agent_instance();
extern int agent_distance();
#ifndef NO_SEX
extern void agent_crossover();
#endif

/* I leave these as lower case since it's safe to think of them as functions. */
#define copy_agent(agent, flag)               create_agent((agent), (flag))
#define new_agent()                           create_agent((AGENT *)0, FALSE)
