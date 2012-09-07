#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/s/me/bedau-stats/RCS/main.c,v 1.2 1994/06/16 10:22:33 terry Exp terry $";
#endif


#include <stdio.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "syscalls.h"
#include "defaults.h"
#include "options.h"

BOOLEAN sysdie = TRUE;
STRING myname;

/*
 * Options in main_options.c.
 */

static LONG hash_size            = DEF_HASH_SIZE;
static DOUBLE bucket_size        = DEF_BUCKET_SIZE;
static INT nbuckets              = DEF_NBUCKETS;
static BOOLEAN verbose           = FALSE;
static INT first_generation      = DEF_FIRST_GENERATION;
static BOOLEAN check_generations = TRUE;
static INT generation_gap        = DEF_GENERATION_GAP;
static BOOLEAN truncate          = TRUE;
static INT period                = DEF_PERIOD;
static INT ngenerations          = DEF_NGENERATIONS;
static LONG start_generation     = DEF_START_GENERATION;
static LONG stop_generation      = DEF_STOP_GENERATION;
static BOOLEAN ssplot            = DEF_SSPLOT;
static BOOLEAN data_cumulative   = DEF_DATA_CUMULATIVE;

#include "main_options.c"

/* Globals, so that we don't have to bother passing them to the hash traversing functions. */
LONG *buckets;
LONG generation;
LONG max_buckets = -1L;

/*
 * Things for ssplot style output.
 */
#define SSPLOT_HEADER "== DATA ==\nM1"
#define SSPLOT_DELIM  '\t'

INT
main(argc, argv)
INT argc;
CHAR **argv;
{
    STRING set_name();
    INT set_bucket();
    VOID clear_buckets();
    VOID resize_buckets();
    CHAR line[BUF_SZ];
    LONG line_number = 0L;
    HASH_TABLE *ht = NULL;
    register INT i;
    INT last_generation = first_generation - 1;
    char delimiter = ' ';

    myname = set_name(argv[0]);
    process_options(argc, argv, main_options);

#ifndef DEBUG
    if (verbose == TRUE){
	error("You have requested verbosity, but the code was not compiled with debugging enabled.");
    }
#endif

    if (ssplot == TRUE && ngenerations == DEF_NGENERATIONS && stop_generation == DEF_STOP_GENERATION){
	error("If you use the -ssplot option, you must use either the -generations or -to options so the number of generations can be determined in advance.");
    }

    buckets = (LONG *) Malloc(nbuckets * sizeof(LONG));

    clear_buckets();

    if (data_cumulative == FALSE){
	ht = hash_create(hash_size);

	if (!ht){
	    error("Could not create hash table of size %ld", hash_size);
	}
    }

    if (stop_generation != DEF_STOP_GENERATION){
	INT count = stop_generation - start_generation;

	if (ngenerations != DEF_NGENERATIONS && ngenerations != count){
	    /* They have given a stop generation and a ngenerations. Check these agree. */

	    error("You have specifed ngenerations = %d, but your start (%d) and stop (%d) generations request processing of %d generations!", ngenerations, start_generation, stop_generation, count);
	}
    }

    if (ssplot == TRUE){
	delimiter = SSPLOT_DELIM;
	printf("%s\n%d %d\n", SSPLOT_HEADER, nbuckets, ngenerations / period);
    }
    
    while (fgets(line, BUF_SZ, stdin)){

	extern long atol();
	register CHAR *tmp;
	LONG population_size;
	register CHAR *name;
	LONG level;

	line_number++;

	tmp = strtok(line, " \t\n");

	if (!tmp){
	    error("Could not read generation number on line %ld of input!", line_number);
	}

	generation = atol(tmp);

	if (check_generations == TRUE && generation != last_generation + generation_gap){
	    error("Incorrect generation numbers! Gen %d follows gen %d on line %d of input!",
		  generation, last_generation, line_number);
	}

	last_generation = generation;

	if (generation < start_generation){
	    continue;
	}

	if (stop_generation != DEF_STOP_GENERATION && generation > stop_generation){
	    break;
	}

	if (ssplot){
	    if (!ngenerations){
		break;
	    }

	    ngenerations--;
	}

	tmp = strtok(0, " \t\n");

	if (!tmp){
	    error("Could not read population level on line %ld of input!", line_number);
	}

	population_size = atol(tmp);

	while ((tmp = strtok(0, " \t\n"))){

	    VOID *hash_return;

	    name = tmp;

	    tmp = strtok(0, " \t\n");

	    if (!tmp){
		error("Could not read population level for agent type '%s' on line %ld of input!", name, line_number);
	    }

	    level = atol(tmp);

	    /* Ignore names that have a zero usage. */
	    if (level == 0L){
		continue;
	    }

	    if (data_cumulative == TRUE){
		INT bucket = (INT) floor (level / bucket_size);

		if (bucket > max_buckets){
		    max_buckets = bucket;
		}

		if (bucket >= nbuckets){
		    if (truncate == FALSE){
			resize_buckets();
			buckets[bucket]++;
		    }
		}
		else {
		    buckets[bucket]++;
		}
	    }
	    else {
		/* Search for this name in the hash table. */

		if ((hash_return = hash_search(ht, name, 0, 0)) == 0){
		    /* Not in hash table, insert it. */
		    HASH_DATA *new = (HASH_DATA *) Malloc(sizeof(HASH_DATA));

#if DEBUG
		    new->name = strdup(name);
#endif
		    new->usage = (DOUBLE) level / (DOUBLE) population_size;
		    new->last_alive = generation;

		    /* Insert. */
		    hash_search(ht, name, (VOID *)new, 0);
#if DEBUG
		    if (verbose == TRUE){
			printf("generation %ld adding %s to hash table, usage set to %f\n", generation, name, new->usage);
		    }
#endif
		}
		else {
		    /* We've seen this agent before. */
		    HASH_DATA *data = (HASH_DATA *)hash_return;
		
		    if (data->last_alive >= generation - 1){
			/* It was alive last generation, or we've seen it this generation. Increment the usage. */
			data->usage += (DOUBLE) level / (DOUBLE) population_size;
#if DEBUG
			if (verbose == TRUE){
			    printf("generation %ld found %s alive. usage inc to %f\n", generation, data->name, data->usage);
			}
#endif
		    }
		    else {
			/* It is new (or newly rediscovered), set the usage. */
			data->usage = (DOUBLE) level / (DOUBLE) population_size;
#if DEBUG
			if (verbose == TRUE){
			    printf("generation %ld, %s is new. usage set to %f\n", generation, data->name, data->usage);
			}
#endif
		    }

		    data->last_alive = generation;
		}
		
		hash_traverse(ht, set_bucket, 0, 0);
	    }
	}


	if (period == 1 || (generation && generation % period == 0)){

	    for (i = 0; i < nbuckets; i++){
		long average = buckets[i] / period;

		/* Make sure that we don't lose information if the above fraction was >0 but <1 */
		if (average == 0L && buckets[i]){
		    average = 1L;
		}
	    
		printf("%ld%c", average, delimiter);
	    }

	    putc('\n', stdout);

	    clear_buckets();
	}
    }

    fprintf(stderr, "The maximum number of buckets was %ld\n", max_buckets);

    return 0;
}

STRING
set_name(s)
STRING s;
{
    STRING slash = strrchr(s, '/');
    return slash ? slash + 1 : s;
}

#if defined(ultrix) || defined(NeXT)
STRING strdup(s)
STRING s;
{
	return strcpy(Malloc(strlen(s) + 1), s);
}
#endif

VOID
resize_buckets()
{
    register INT i;
    INT new_nbuckets = nbuckets << 1;
    LONG *new_buckets = (LONG *) Malloc (new_nbuckets * sizeof(LONG));

    for (i = 0; i < nbuckets; i++){
	new_buckets[i] = buckets[i];
    }

    for (; i < new_nbuckets; i++){
	new_buckets[i] = 0L;
    }

    Free(buckets);

    buckets = new_buckets;
    nbuckets = new_nbuckets;

#if DEBUG
    if (verbose == TRUE){
	printf("Number of buckets incremented to %d\n", new_nbuckets);
    }
#endif

    return;
}


INT
set_bucket(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    VOID resize_buckets();
    HASH_DATA *data = (HASH_DATA *)entry;
    
    /* Increment the appropriate bucket if the agent was alive this generation. */
    if (data->last_alive == generation) {

	INT bucket = (INT) floor (data->usage / bucket_size);

	if (bucket > max_buckets){
	    max_buckets = bucket;
	}

	if (bucket >= nbuckets){
	    if (truncate == FALSE){
		resize_buckets();
		buckets[bucket]++;
	    }
	}
	else {
	    buckets[bucket]++;
	}
    }
    
    return 1;
}

VOID
clear_buckets()
{
    register int i;
    
    for (i = 0; i < nbuckets; i++){
	buckets[i] = 0;
    }

    return;
}

