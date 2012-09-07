/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/worlds.h,v 1.6 1992/11/16 07:05:45 terry Exp terry $ */
extern WORLD *new_world();
extern WORLD_EDIT *new_world_edit();
extern void delete_world();
extern WORLD *read_world_from_file();
extern WORLD_EDIT *read_world_edit_from_file();
extern void write_world_to_file();
extern void write_world_edit_to_file();
extern void instantiate_world();
extern AGENT_POPULATION_SIZE world_population_size();
extern RESOURCE_LIST world_resource_levels();
extern void variable_has_no_value();
extern AGENT_POPULATION_SIZE world_count_schema();

/* NORTH, SOUTH, EAST and WEST must be defined as 0 to 3 (in some order). */

#define NORTH      0
#define SOUTH      1
#define EAST       2
#define WEST       3
#define NORTH_EAST 4
#define SOUTH_EAST 5
#define NORTH_WEST 6
#define SOUTH_WEST 7
