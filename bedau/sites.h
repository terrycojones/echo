/* $Header: /disks/disk1/Users/terry/r/echo/s.orig/RCS/sites.h,v 1.8 1994/02/07 23:16:08 terry Exp terry $ */
extern SITE *new_site();
extern SITE_EDIT *new_site_edit();
extern void grow_site();
extern void site_produce();
extern void site_distribute_resources();
extern void site_add_agent();
extern void site_delete_agent();
extern void site_make_newborns_old();
extern void site_move_agents_together();
extern void print_site();
extern SITE *read_site_from_file();
extern SITE_EDIT *read_site_edit_from_file();
extern void write_site_edit_to_file();
extern BOOLEAN count_agents();
extern BOOLEAN str_to_resource_list();
extern RESOURCE_SIZE *site_resource_levels();
extern BOOLEAN site_read_stack_from_file();

extern void site_hash_genomes();
extern void site_mark_all_genomes_dead();
extern void site_assign_species_hash_names();
extern void site_list_agents();
extern AGENT_POPULATION_SIZE site_count_schema();

#ifndef NO_MAINTENANCE
extern void site_charge_maintenance();
#endif

#ifndef NO_ZAP
extern void site_kill_agents_without_cause();
#endif

#ifndef NO_INTERACTIONS
extern void site_perform_interactions();
#endif

#ifndef NO_MUTATION
extern void site_mutate_agents();
#endif

#ifndef NO_SELF_REPLICATION
extern void site_self_replicate_agents();
#endif

#ifndef NO_MIGRATION
extern void site_migrate_agents();
#endif
