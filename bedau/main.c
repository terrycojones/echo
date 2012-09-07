#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/main.c,v 1.7 1992/10/22 04:14:17 terry Exp $";
#endif


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <memory.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "types.h"
#include "defaults.h"
#include "globals.h"
#include "main.h"
#include "worlds.h"
#include "running.h"
#include "graphics.h"
#include "random.h"
#include "syscalls.h"
#include "options.h"



STRING myname;
long seed = NO_SEED;
long old_seed = NO_SEED;
BOOLEAN sysdie = TRUE;
NRESOURCES nresources = MAX_RESOURCES;
AGENT_POPULATION_SIZE nspecies;
WORLD *the_world = (WORLD *)0;
STRING echo_version = "0.1";
BOOLEAN graphics = DEF_GRAPHICS;
BOOLEAN sex = DEF_SEX;
BOOLEAN self_replication = DEF_SELF_REPLICATION;
STRING verbose;
int runs = 1;
HASH_TABLE *genome_hash = (HASH_TABLE *) 0;
HASH_TABLE *species_name_hash = (HASH_TABLE *) 0;
int genome_hash_size = DEFAULT_GENOME_HASH_SIZE;
#ifndef NO_TCL
char site_file_name[MAXPATHLEN];
FILE *site_file = NULL;
#endif

/* BEDAU: */
BOOLEAN dump_species = DEF_DUMP_SPECIES;
FILE *dump_species_fp = NULL;
BOOLEAN update_on_testing = DEF_UPDATE_ON_TESTING;
BOOLEAN update_on_success = DEF_UPDATE_ON_SUCCESS;
BOOLEAN update_combat = DEF_UPDATE_COMBAT;
BOOLEAN update_trading = DEF_UPDATE_TRADING;
BOOLEAN update_mating = DEF_UPDATE_MATING;
BOOLEAN sum_over_genes = DEF_SUM_OVER_GENES;
FILE *gene_fp[NCHROMOSOMES];
long total_agents = 0L;
STRING genes_to_dump = DEF_GENES_TO_DUMP;
long total_allocated = 0L;

char *chromosome_names[NCHROMOSOMES] = {
    "Offense_Tag",
    "Defense_Tag",
    "Mating_Tag",
    "Combat_Condition",
    "Trade_Condition",
    "Mating_Condition"
};

static GENERATION ngenerations = MAX_POSSIBLE_GENERATION;
static STRING world_file = NULL;

#include "main_options.c"

int
main(argc, argv)
int argc;
char **argv;
{
    STRING echo_object_location;
    extern STRING getenv();
    void dump();
    struct stat sbuf;
    register int i;

    if (signal(SIGSEGV, dump) == BADSIG){
	error("could not arrange to catch SIGSEGV");
    }

    if (signal(SIGBUS, dump) == BADSIG){
	error("could not arrange to catch SIGBUS");
    }


    myname = set_name(argv[0]);
    rcsid = rcsid;


#ifdef NO_GRAPHICS
    if (graphics == TRUE)
    {
	error("You have turned on graphics, but it was not compiled into this version.");
    }
#endif
    
#ifdef NO_SEX
    if (sex == TRUE)
    {
	error("You have turned on sex, but it was not compiled into this version.");
    }
#endif

#ifdef NO_SELF_REPLICATION
    if (self_replication == TRUE)
    {
	error("You have turned on self-replication, but it was not compiled into this version.");
    }
#endif

    
    /* Make verbose something that can always be freed. */
    verbose = strdup("");
    
    (void) process_options(argc, argv, main_options);

    for (i = 0; i < NCHROMOSOMES; i++){
	gene_fp[i] = NULL;
    }

    
    /* Look for ECHO_LOCATION environment variable, else use . */
    if ((echo_object_location = getenv("ECHO_LOCATION"))){
	Chdir(echo_object_location);
    }
    else {

	if (stat(DEF_OBJECTS_DIRECTORY, &sbuf) == -1){
	    sysdie = FALSE;
	    error("You have not set the environment variable ECHO_OBJECTS");
	    error("and I cannot find a directory called '%s'. One of these", DEF_OBJECTS_DIRECTORY);
	    sysdie = TRUE;
	    error("is needed so we can locate agents, sites and worlds.");
	}

	Chdir(DEF_OBJECTS_DIRECTORY);
	echo_object_location = DEF_OBJECTS_DIRECTORY;
    }

    
    if (graphics == FALSE){
    
	register int run;
	
	if (!world_file){
	    error("you must pre-specify a world if graphics are disabled.");
	}
	
	for (run = 0; run < runs; run++){
	    extern int count_variants();
	    int nvariants_ever = 0;
	    int nvariants_alive = 0;
	
	    instantiate_world("worlds", world_file, genome_hash_size);
	    WORLD_HALT_GENERATION(the_world) = ngenerations;
	    
	    run_world(genome_hash_size);
	    /* fprintf(stderr, "This code needs work!!!!! the new seed generation will not work with the tcl site stuff!!!!!\n"); */
	    seed = knuth_random() * 1000000000;
	    

#if 0
	    hash_traverse(genome_hash, count_variants, TRUE, &nvariants_alive);
	    hash_traverse(genome_hash, count_variants, FALSE, &nvariants_ever);
	    printf("%d variants alive, %d variants ever, %d agents alive.\n", nvariants_alive, nvariants_ever, world_population_size(the_world));
#endif
	} 
    }
    else {
#ifndef NO_GRAPHICS
	initialize_graphics(argc, argv, world_file, echo_object_location);
#else
	error("graphics was not compiled into this version of ECHO, use -world to specify a world.");
#endif
    }

    return 0;
}


STRING 
set_name(s)
STRING s;
{
    extern STRING strrchr();
    STRING slash = strrchr(s, '/');
    
    return slash ? slash + 1 : s;
}

void
version()
{
    printf("ECHO version %s\n", echo_version);
    exit(0);
}

void
dump(n)
int n;
{
    fprintf(stderr, "Core was dumped on signal %d. Seed is %ld\n", n, old_seed);
    Fflush(stdout);
    abort();
    
    return;
}


#if defined(ultrix) || defined(NeXT)
STRING strdup(s)
STRING s;
{
    return strcpy(Malloc(strlen(s) + 1), s);
}
#endif

#ifdef COMPRESS_OUTPUT
#define Fopen Popen
#define Fclose Pclose
#define COMPRESS "gzip -9 -c > "
#define SUFFIX ".gz"
#else
#define COMPRESS ""
#define SUFFIX ""
#endif

#ifndef NO_TCL
void
open_site_file()
{
    /*
     * We don't really want to compress this in general as tcl wont know how to read it,
     * but we may as well do it for the Bedau stuff since we're not going to be looking
     * at the site data with the site viewer.
     */
    
    sprintf(site_file_name, "%s%ld.sites%s", COMPRESS, old_seed, SUFFIX);
    /* sprintf(site_file_name, "%ld.sites", old_seed); */
    site_file = Fopen(site_file_name, "w");

    return;
}
#endif

void
open_gene_files(genes, seed)
STRING genes;
long seed;
{
    register int i;

    /* Close anything already open. */
    for (i = 0; i < NCHROMOSOMES; i++){
	if (gene_fp[i]){
	    Fclose(gene_fp[i]);
	    gene_fp[i] = NULL;
	}
    }

    while (*genes){

	int gene = *genes - '0';
	char file[1024];
	
	if (gene >= 0 && gene < NCHROMOSOMES){
	    sprintf(file, "%s%ld.gene%d%s", COMPRESS, seed, gene, SUFFIX);
	    gene_fp[gene] = Fopen(file, "w");
	}
	else {
	    error("An invalid gene number (%d) was given to the -dump-genes option. The valid range is %d to %d",
		  gene, 0, NCHROMOSOMES - 1);
	}

	genes++;
    }

    return;
}

void
open_species_file()
{
    char file[1024];
	    
    if (dump_species_fp) {
	Fclose(dump_species_fp);
    }

    sprintf(file, "%s%ld.species%s", COMPRESS, old_seed, SUFFIX);
    dump_species_fp = Fopen(file, "w");
}

#ifdef COMPRESS_OUTPUT
#undef Fopen
#undef Fclose
#endif

#undef COMPRESS
#undef SUFFIX
