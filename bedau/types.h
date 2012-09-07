/* $Header: /disks/disk1/Users/terry/r/echo/s.orig/RCS/types.h,v 1.10 1994/02/07 23:16:09 terry Exp terry $ */

#include <limits.h>

#include "defs.h"

#define MAX_RESOURCES            26

/* Strings used by Agents, Sites and Worlds. */
#define NAME_STR                 "Name"
#define FILENAME_STR             "File_Name"

/* Agent strings. */
#define UPTAKE_MASK_STR          "Uptake_Mask"
#define TRADING_RESOURCE_STR     "Trading_Resource"
#define RESERVOIR_STR            "Initial_Reserves"

/* Site strings. */
#define MUTATION_PROB_STR        "Mutation_Probability"
#define CROSSOVER_PROB_STR       "Crossover_Probability"
#define RANDOM_DEATH_PROB_STR    "Random_Death_Probability"
#define PRODUCTION_FUNCTION_STR  "Production_Function"
#define SITE_RESOURCES_STR       "Initial_Resource_Levels"
#define MAXIMUMS_STR             "Maximums"
#define MAINTENANCE_STR          "Maintenance"
#define AGENTS_STR               "Agents"

/* World strings. */
#define NRESOURCES_STR           "Number_Of_Resources"
#define SITES_STR                "Sites"
#define ROWS_STR                 "Rows"
#define COLS_STR                 "Columns"
#define COMBAT_STR               "Combat_Matrix"
#define TRADE_STR                "Trading_Fraction"
#define SELFREP_FRACTION_STR     "Self_Replication_Fraction"
#define SELFREP_THRESHOLD_STR    "Self_Replication_Threshold"
#define C_STR                    "Interaction_Fraction"
#define M_STR                    "Maintenance_Probability"
#define NEIGHBORHOOD_STR         "Neighborhood"

typedef char *                   STRING;
typedef char                     RESOURCE;
typedef int                      RESOURCE_SIZE;
typedef RESOURCE_SIZE *          RESOURCE_LIST;
typedef int                      NRESOURCES;
typedef int                      CHROMOSOME_LENGTH;
typedef int                      AGENT_POPULATION_SIZE;
typedef int                      COORD;
typedef double                   FRACTION;
typedef double                   PROBABILITY;
typedef unsigned long            COUNT;
typedef unsigned long            GENERATION;

#include "hash.h"

/* This is dependent on the type of GENERATION. Defined in <limits.h> */
#define MAX_POSSIBLE_GENERATION  ULONG_MAX

/* The following nonsense makes us compatible with XIntrinsic.h. */
#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

typedef enum {
    FALSE = 0,    /* The =0 for compatibility with X. */
    TRUE,
    UNINITIALIZED
} BOOLEAN;

#ifndef NO_MIGRATION
typedef enum {
    NEWS, 
    ALLEIGHT, 
    NONE
} NEIGHBORHOOD;
#endif

typedef enum {
    BOOLEAN_OPT,
    CHAR_OPT,
    STRING_OPT,
    INT_OPT,
    FLOAT_OPT,
    DOUBLE_OPT,
    LONG_OPT,
    FUNC0_OPT,
    FUNC1_OPT,
} OPTION_TYPE;

typedef struct {
    char *name;
    char *abbreviation;
    OPTION_TYPE type;
    BOOLEAN required;
    BOOLEAN has_lower_limit;
    double lower_limit;
    BOOLEAN has_upper_limit;
    double upper_limit;
    void *address;
    char *explanation;
} OPTION;

typedef struct {
    COORD row;
    COORD col;
} SITE_COORD;

typedef struct {
    CHROMOSOME_LENGTH current_length;
    CHROMOSOME_LENGTH max_length;
    RESOURCE *chromosome;
    /* BEDAU: */
    long usage;
} CHROMOSOME;

typedef struct {
    BOOLEAN alive;
    STRING genome;
    STRING name;
    AGENT_POPULATION_SIZE instance;
    GENERATION this_generation;
    GENERATION first_found;
    AGENT_POPULATION_SIZE count;
} GENOME_HASH_DATA;

typedef struct {
    STRING name;
    AGENT_POPULATION_SIZE instance_count;
    AGENT_POPULATION_SIZE number_alive;
    AGENT_POPULATION_SIZE variants_alive;
} SPECIES_NAME_HASH_DATA;

#define OFFENSE_TAG_INDEX  0
#define DEFENSE_TAG_INDEX  1
#define MATING_TAG_INDEX   2
#define COMBAT_COND_INDEX  3
#define TRADE_COND_INDEX   4
#define MATING_COND_INDEX  5

#define NCHROMOSOMES       6

typedef struct {
    char *name;
    long id;
    CHROMOSOME *chromosomes[NCHROMOSOMES];
    STRING genome;
    RESOURCE_SIZE genome_length;
    RESOURCE_SIZE max_genome_length;
    
    RESOURCE_SIZE *reservoir;
    RESOURCE_SIZE *makeup;
    RESOURCE trading_resource;
    
    BOOLEAN *resource_uptake_mask;
    BOOLEAN newborn;
    BOOLEAN gained_resources_this_round;
#ifndef NO_SELF_REPLICATION
    BOOLEAN can_self_replicate;
#endif
} AGENT;

typedef struct {
    char *name;
    char *chromosomes[NCHROMOSOMES];
    
    char *reservoir;
    char *trading_resource;
    
    char *resource_uptake_mask;
} AGENT_EDIT;

typedef struct {
    char *name;
    SITE_COORD location;
    
    AGENT **agents;
    AGENT_POPULATION_SIZE nagents;
    AGENT_POPULATION_SIZE max_agents;
    AGENT_POPULATION_SIZE interactions;
    
    PROBABILITY p_mutation;
    PROBABILITY p_crossover;
    PROBABILITY p_death_without_cause;
    
    RESOURCE_SIZE *produces;
    RESOURCE_SIZE *maximums;
    RESOURCE_SIZE *resource_levels;
    RESOURCE_SIZE *maintenance;
} SITE;

typedef struct {
    char *name;
    
    char *agents;
    
    char *p_mutation;
    char *p_crossover;
    char *p_death_without_cause;
    
    char *produces;
    char *maximums;
    char *resource_levels;
    char *maintenance;
} SITE_EDIT;

typedef struct {
    char *name;
    BOOLEAN running;
    NRESOURCES nresources;
    SITE ***sites;
 
    COORD rows;
    COORD cols;
    RESOURCE_SIZE **combat_matrix;
    
    GENERATION generation;
    GENERATION halt_generation;
#ifndef NO_MUTATION
    COUNT mutations;
#endif
    
    FRACTION c;
    FRACTION selfrep_fraction;
    FRACTION selfrep_threshold;
    PROBABILITY trade_fraction;
    PROBABILITY p_maintenance;
    
    NEIGHBORHOOD neighborhood;
} WORLD;

typedef struct {
    char *name;
    char *nresources;
    char *sites;
    
    char *rows;
    char *cols;
    char **combat_matrix;
    
    char *c;
    char *selfrep_fraction;
    char *selfrep_threshold;
    char *p_maintenance;
    char *trade_fraction;
    char *neighborhood;
} WORLD_EDIT;

#define CHROMOSOME_LEN(chrom)                  ((chrom)->current_length)
#define CHROMOSOME_MAX_LEN(chrom)              ((chrom)->max_length)
#define CHROMOSOME_STR(chrom)                  ((chrom)->chromosome)
/* BEDAU: */
#define CHROMOSOME_USAGE(chrom)                ((chrom)->usage)

/* BEDAU: */
#define AGENT_NAME(agent)                      ((agent)->name)
#define AGENT_ID(agent)                        ((agent)->id)
#define AGENT_GENOME(agent)                    ((agent)->genome)
#define AGENT_GENOME_LEN(agent)                ((agent)->genome_length)
#define AGENT_MAX_GENOME_LEN(agent)            ((agent)->max_genome_length)
#define AGENT_CHROMOSOMES(agent)               ((agent)->chromosomes)
#define AGENT_CHROMOSOME(agent, c)             (AGENT_CHROMOSOMES(agent)[(int)(c)])
#define AGENT_OFFENSE_TAG(agent)               (AGENT_CHROMOSOME(agent, OFFENSE_TAG_INDEX)->chromosome)
#define AGENT_OFFENSE_TAG_LEN(agent)           (AGENT_CHROMOSOME(agent, OFFENSE_TAG_INDEX)->current_length)
#define AGENT_OFFENSE_TAG_MAX_LEN(agent)       (AGENT_CHROMOSOME(agent, OFFENSE_TAG_INDEX)->max_length)
#define AGENT_OFFENSE_TAG_USAGE(agent)         (AGENT_CHROMOSOME(agent, OFFENSE_TAG_INDEX)->usage)
#define AGENT_DEFENSE_TAG(agent)               (AGENT_CHROMOSOME(agent, DEFENSE_TAG_INDEX)->chromosome)
#define AGENT_DEFENSE_TAG_LEN(agent)           (AGENT_CHROMOSOME(agent, DEFENSE_TAG_INDEX)->current_length)
#define AGENT_DEFENSE_TAG_MAX_LEN(agent)       (AGENT_CHROMOSOME(agent, DEFENSE_TAG_INDEX)->max_length)
#define AGENT_DEFENSE_TAG_USAGE(agent)         (AGENT_CHROMOSOME(agent, DEFENSE_TAG_INDEX)->usage)
#define AGENT_MATING_TAG(agent)                (AGENT_CHROMOSOME(agent, MATING_TAG_INDEX)->chromosome)
#define AGENT_MATING_TAG_LEN(agent)            (AGENT_CHROMOSOME(agent, MATING_TAG_INDEX)->current_length)
#define AGENT_MATING_TAG_MAX_LEN(agent)        (AGENT_CHROMOSOME(agent, MATING_TAG_INDEX)->max_length)
#define AGENT_MATING_TAG_USAGE(agent)          (AGENT_CHROMOSOME(agent, MATING_TAG_INDEX)->usage)
#define AGENT_COMBAT_COND(agent)               (AGENT_CHROMOSOME(agent, COMBAT_COND_INDEX)->chromosome)
#define AGENT_COMBAT_COND_LEN(agent)           (AGENT_CHROMOSOME(agent, COMBAT_COND_INDEX)->current_length)
#define AGENT_COMBAT_COND_MAX_LEN(agent)       (AGENT_CHROMOSOME(agent, COMBAT_COND_INDEX)->max_length)
#define AGENT_COMBAT_COND_USAGE(agent)         (AGENT_CHROMOSOME(agent, COMBAT_COND_INDEX)->usage)
#define AGENT_TRADE_COND(agent)                (AGENT_CHROMOSOME(agent, TRADE_COND_INDEX)->chromosome)
#define AGENT_TRADE_COND_LEN(agent)            (AGENT_CHROMOSOME(agent, TRADE_COND_INDEX)->current_length)
#define AGENT_TRADE_COND_MAX_LEN(agent)        (AGENT_CHROMOSOME(agent, TRADE_COND_INDEX)->max_length)
#define AGENT_TRADE_COND_USAGE(agent)          (AGENT_CHROMOSOME(agent, TRADE_COND_INDEX)->usage)
#define AGENT_MATING_COND(agent)               (AGENT_CHROMOSOME(agent, MATING_COND_INDEX)->chromosome)
#define AGENT_MATING_COND_LEN(agent)           (AGENT_CHROMOSOME(agent, MATING_COND_INDEX)->current_length)
#define AGENT_MATING_COND_MAX_LEN(agent)       (AGENT_CHROMOSOME(agent, MATING_COND_INDEX)->max_length)
#define AGENT_MATING_COND_USAGE(agent)         (AGENT_CHROMOSOME(agent, MATING_COND_INDEX)->usage)
#define AGENT_MAKEUP(agent, type)              ((agent)->makeup[(int)(type)])
#define AGENT_MAKEUP_LIST(agent)               ((agent)->makeup)
#define AGENT_RESERVE(agent, type)             ((agent)->reservoir[(int)(type)])
#define AGENT_RESERVE_LIST(agent)              ((agent)->reservoir)
#define AGENT_TRADING_RESOURCE(agent)          ((agent)->trading_resource)
#define AGENT_UPTAKE_MASK(agent)               ((agent)->resource_uptake_mask)
#define AGENT_UPTAKE(agent, type)              ((agent)->resource_uptake_mask[(int)(type)])
#define AGENT_NEWBORN(agent)                   ((agent)->newborn)
#define AGENT_GAINED(agent)                    ((agent)->gained_resources_this_round)
#ifndef NO_SELF_REPLICATION
#define AGENT_CAN_SELF_REPLICATE(agent)        ((agent)->can_self_replicate)
#endif

#define AGENT_EXCESS(agent, type) \
((AGENT_RESERVE(agent, type) > AGENT_MAKEUP(agent, type)) ? AGENT_RESERVE(agent, type) - AGENT_MAKEUP(agent, type) : 0)

#define AGENT_EDIT_NAME(agent)                 ((agent)->name)
#define AGENT_EDIT_CHROMOSOMES(agent)          ((agent)->chromosomes)
#define AGENT_EDIT_CHROMOSOME(agent, c)        (AGENT_EDIT_CHROMOSOMES(agent)[(int)(c)])
#define AGENT_EDIT_RESERVE(agent, type)        ((agent)->reservoir[(int)(type)])
#define AGENT_EDIT_RESERVE_LIST(agent)         ((agent)->reservoir)
#define AGENT_EDIT_TRADING_RESOURCE(agent)     ((agent)->trading_resource)
#define AGENT_EDIT_UPTAKE_MASK(agent)          ((agent)->resource_uptake_mask)
#define AGENT_EDIT_UPTAKE(agent, type)         ((agent)->resource_uptake_mask[(int)(type)])

#define SITE_NAME(site)                        ((site)->name)
#define SITE_ROW(site)                         ((site)->location.row)
#define SITE_COL(site)                         ((site)->location.col)
#define SITE_AGENTS(site)                      ((site)->agents)
#define SITE_AGENT(site, agent)                ((site)->agents[(int)(agent)])
#define SITE_NAGENTS(site)                     ((site)->nagents)
#define SITE_MAX_AGENTS(site)                  ((site)->max_agents)
#define SITE_INTERACTIONS(site)                ((site)->interactions)
#define SITE_P_MUTATION(site)                  ((site)->p_mutation)
#define SITE_P_CROSSOVER(site)                 ((site)->p_crossover)
#define SITE_P_RANDOM_DEATH(site)              ((site)->p_death_without_cause)
#define SITE_PRODUCES(site, type)              ((site)->produces[(int)(type)])
#define SITE_PRODUCE_LIST(site)                ((site)->produces)
#define SITE_MAXIMUM(site, type)               ((site)->maximums[(int)(type)])
#define SITE_MAXIMUM_LIST(site)                ((site)->maximums)
#define SITE_RESOURCE(site, type)              ((site)->resource_levels[(int)(type)])
#define SITE_RESOURCE_LIST(site)               ((site)->resource_levels)
#define SITE_MAINTENANCE(site, type)           ((site)->maintenance[(int)(type)])
#define SITE_MAINTENANCE_LIST(site)            ((site)->maintenance)

#define SITE_EDIT_NAME(site)                   ((site)->name)
#define SITE_EDIT_AGENTS(site)                 ((site)->agents)
#define SITE_EDIT_P_MUTATION(site)             ((site)->p_mutation)
#define SITE_EDIT_P_CROSSOVER(site)            ((site)->p_crossover)
#define SITE_EDIT_P_RANDOM_DEATH(site)         ((site)->p_death_without_cause)
#define SITE_EDIT_PRODUCES(site, type)         ((site)->produces[(int)(type)])
#define SITE_EDIT_PRODUCE_LIST(site)           ((site)->produces)
#define SITE_EDIT_MAXIMUM(site, type)          ((site)->maximums[(int)(type)])
#define SITE_EDIT_MAXIMUM_LIST(site)           ((site)->maximums)
#define SITE_EDIT_RESOURCE(site, type)         ((site)->resource_levels[(int)(type)])
#define SITE_EDIT_RESOURCE_LIST(site)          ((site)->resource_levels)
#define SITE_EDIT_MAINTENANCE(site, type)      ((site)->maintenance[(int)(type)])
#define SITE_EDIT_MAINTENANCE_LIST(site)       ((site)->maintenance)

#define WORLD_NAME(world)                      ((world)->name)
#define WORLD_RUNNING(world)                   ((world)->running)
#define WORLD_NRESOURCES(world)                ((world)->nresources)
#define WORLD_SITES(world)                     ((world)->sites)
#define WORLD_SITE(world, row, col)            ((world)->sites[(int)(row)][(int)(col)])
#define WORLD_ROW(world, row)                  ((world)->sites[(int)(row)])
#define WORLD_ROWS(world)                      ((world)->rows)
#define WORLD_COLS(world)                      ((world)->cols)
#define WORLD_COMBAT(world)                    ((world)->combat_matrix)
#define WORLD_COMBAT_XY(world, x, y)           ((world)->combat_matrix[(int)(x)][(int)(y)])
#define WORLD_GENERATION(world)                ((world)->generation)
#define WORLD_HALT_GENERATION(world)           ((world)->halt_generation)
#define WORLD_MUTATIONS(world)                 ((world)->mutations)
#define WORLD_TRADE_FRACTION(world)            ((world)->trade_fraction)
#define WORLD_C(world)                         ((world)->c)
#define WORLD_SELFREP_FRACTION(world)          ((world)->selfrep_fraction)
#define WORLD_SELFREP_THRESHOLD(world)         ((world)->selfrep_threshold)
#define WORLD_P_MAINTENANCE(world)             ((world)->p_maintenance)
#define WORLD_NEIGHBORHOOD(world)              ((world)->neighborhood)

#define WORLD_EDIT_NAME(world)                 ((world)->name)
#define WORLD_EDIT_NRESOURCES(world)           ((world)->nresources)
#define WORLD_EDIT_SITES(world)                ((world)->sites)
#define WORLD_EDIT_ROWS(world)                 ((world)->rows)
#define WORLD_EDIT_COLS(world)                 ((world)->cols)
#define WORLD_EDIT_COMBAT(world)               ((world)->combat_matrix)
#define WORLD_EDIT_TRADE_FRACTION(world)       ((world)->trade_fraction)
#define WORLD_EDIT_C(world)                    ((world)->c)
#define WORLD_EDIT_SELFREP_FRACTION(world)     ((world)->selfrep_fraction)
#define WORLD_EDIT_SELFREP_THRESHOLD(world)    ((world)->selfrep_threshold)
#define WORLD_EDIT_P_MAINTENANCE(world)        ((world)->p_maintenance)
#define WORLD_EDIT_NEIGHBORHOOD(world)         ((world)->neighborhood)

#define skip_white(s)                          while ((*s) == ' ' || (*s) == '\t') (s)++
#define skip_to_white(s)                       while ((*s) != '\0' && (*s) != ' ' && (*s) != '\t') (s)++

#define DEBUG(s)                               (strpbrk(verbose, (s)))
