#define DEFAULT_TRADE_FRACTION        1.0
#define DEFAULT_NEIGHBORHOOD          NEWS
#define DEF_SITE_GROWTH_INC                 50
#define DEF_SITE_NAGENTS                    50
#define DEFAULT_MUTATION_P                  0.001
#define DEFAULT_CROSSOVER_P                 0.6
#define DEFAULT_RANDOM_DEATH_P              0.001
#define DEFAULT_C                           0.5
#define DEFAULT_SELFREP_FRACTION            0.5
#define DEFAULT_SELFREP_THRESHOLD           2.0
#define DEFAULT_MAINTENANCE                 1.0
#define DEFAULT_GENOME_HASH_SIZE            199
#define DEF_OUTPUT_FILENAME                 "/dev/null"
#define DEF_SITE_NAGENTS_MULTIPLIER         5
#define DEF_OBJECTS_DIRECTORY               "OBJECTS"

#ifndef NO_GRAPHICS
#define DEF_GRAPHICS                        TRUE
#else
#define DEF_GRAPHICS                        FALSE
#endif

#ifndef NO_SEX
#define DEF_SEX                             TRUE
#else
#define DEF_SEX                             FALSE
#endif

#define ECHO_GENS_PER_HAAS_STEP             20
#define HAAS_STEPS_FOR_SELFREP              25

#ifndef NO_SELF_REPLICATION
#define DEF_SELF_REPLICATION                TRUE
#define SELF_REPLICATION_INTERVAL           HAAS_STEPS_FOR_SELFREP * ECHO_GENS_PER_HAAS_STEP
#else
#define DEF_SELF_REPLICATION                FALSE
#endif

#define DEFAULT_WATER_UPTAKE                5
