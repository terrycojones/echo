#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/running.c,v 1.5 1992/11/16 07:05:45 terry Exp terry $";
#endif


#include "types.h"
#include "sites.h"
#include "worlds.h"
#include "graphics.h"
#include "syscalls.h"

#include <stdio.h>
#include <string.h>

#include "globals.h"
extern void fflush();

GENERATION halt_generation_before_pause = MAX_POSSIBLE_GENERATION;

void
set_halt_generation(generation)
GENERATION generation;
{
    rcsid = rcsid;

    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    if (generation < WORLD_GENERATION(the_world)){
	popup_message("You cannot set time backwards!\nOnly God can do that, and it's not here.");
	return;
    }
	
    if (generation != -1){
	printf("Run through generation %d.\n", generation);
    }
    
    WORLD_RUNNING(the_world) = TRUE;
    WORLD_HALT_GENERATION(the_world) = generation;
    
    return;
}

int
free_genome_hash(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    GENOME_HASH_DATA *node = ((GENOME_HASH_DATA *)entry);
    
    Free(node->genome);
    Free(node->name);
    Free(node);
    
    return 1;
}

int
free_species_name_hash(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    Free(((SPECIES_NAME_HASH_DATA *)entry)->name);
    Free((SPECIES_NAME_HASH_DATA *)entry);
    
    return 1;
}

int
zero_genome_name_counts(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    ((SPECIES_NAME_HASH_DATA *)entry)->variants_alive = 0;
    ((SPECIES_NAME_HASH_DATA *)entry)->number_alive = 0;
    return 1;
}

int
mark_genome_as_dead(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    ((GENOME_HASH_DATA *)entry)->alive = FALSE;
    return 1;
}

int
print_genome_hash(entry, alive, dummy)
void *entry;
void *alive;
void *dummy;
{
    GENOME_HASH_DATA *node = ((GENOME_HASH_DATA *)entry);
    
    if ((BOOLEAN) alive != node->alive){
	/* Not the kind we are looking for. */
	return 1;
    }
    
    if ((BOOLEAN) alive == TRUE){
	printf("%s%03d (%s) (%d copies) discovered in %d\n",
	       node->name, node->instance, node->genome, node->count, node->first_found);
    }
    else {
	printf("%s%03d (%s) discovered in %d last alive in %d\n",
	       node->name, node->instance, node->genome, node->first_found, node->this_generation);
    }
    
    return 1;
}

int
count_variants(entry, alive, count)
void *entry;
void *alive;
void *count;
{
    if ((BOOLEAN) alive == TRUE && ((GENOME_HASH_DATA *)entry)->alive == FALSE){
	/* Not the kind we are looking for. */
	return 1;
    }
    
    (*((int *)count))++;
    
    return 1;
}

int
print_species_name_hash_count(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    SPECIES_NAME_HASH_DATA *node = (SPECIES_NAME_HASH_DATA *)entry;
    
    printf("%3d %s variants have existed, %3d are now alive, (%3d population members).\n", 
	   node->instance_count, node->name, node->variants_alive, node->number_alive);
    return 1;
}


int
increment_species_count(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    nspecies++;
    return 1;
}

int
assign_species_levels(entry, array, offset)
void *entry;
void *array;
void *offset;
{
    ((int *)array)[*((int *) offset)] = ((SPECIES_NAME_HASH_DATA *)entry)->number_alive;
    (*((int *) offset))++;
    
    return 1;
}



void
run_world(hash_size)
int hash_size;
{
    register COORD row;
    register COORD nrows;
    register COORD col;
    register COORD ncols;
    static RESOURCE_SIZE *world_resource_levels_list = (RESOURCE_SIZE *) 0;
    static AGENT_POPULATION_SIZE *world_species_levels_list = (AGENT_POPULATION_SIZE *) 0;
    static AGENT_POPULATION_SIZE max_gen_pop;
    static AGENT_POPULATION_SIZE total_gen_pop;
    double migration_probabilities[NMIGRATION_BITS + 1];
    int i;
    
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    /* Set up migration probabilities, to be passed to site_migrate_agents(). */
    for (i = 0; i <= NMIGRATION_BITS; i++){
	migration_probabilities[i] = (double) i / (double) NMIGRATION_BITS;
    }
    
    if (world_resource_levels_list == (RESOURCE_SIZE *) 0){
	world_resource_levels_list = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    }
    
    WORLD_RUNNING(the_world) = TRUE;
    nrows = WORLD_ROWS(the_world);
    ncols = WORLD_COLS(the_world);

    /* This assumes that on subsequent calls we will use the same sized genome hash table. */
    if (genome_hash == (HASH_TABLE *) 0){
	genome_hash = hash_create(hash_size);
    }
    else {
	hash_purge(genome_hash, free_genome_hash);
    }
    
    if (species_name_hash == (HASH_TABLE *) 0){
	species_name_hash = hash_create(5);
    }
    else {
	hash_purge(species_name_hash, free_species_name_hash);
    }

    /* Set up the hash table full of the genome names. */
    for (row = 0; row < nrows; row++){
	for (col = 0; col < ncols; col++){
	    site_assign_species_hash_names(WORLD_SITE(the_world, row, col));
	}
    }

    nspecies = 0;
    hash_traverse(species_name_hash, increment_species_count, 0, 0);
    
    /* nspecies is now set up. Make room to hold the species counts. */
    if (world_species_levels_list == (AGENT_POPULATION_SIZE *) 0){
	world_species_levels_list = (AGENT_POPULATION_SIZE *) Malloc(nspecies * sizeof(AGENT_POPULATION_SIZE));
    }

    /* Hash the initial population. */
    if DEBUG("l") printf("The initial site stacks contain:\n");
    
    for (row = 0; row < nrows; row++){
	for (col = 0; col < ncols; col++){
	    site_hash_genomes(WORLD_SITE(the_world, row, col), 0);
	    if DEBUG("l") site_list_agents(WORLD_SITE(the_world, row, col), TRUE, stdout);
	}
    }
    
#ifndef NO_GRAPHICS
    if (graphics == TRUE){
	initialize_species_graph(world_species_levels_list);
    }
#endif
    
    if DEBUG("p") {
	printf("The initial population contains:\n");
	hash_traverse(genome_hash, print_genome_hash, TRUE, 0);
    }

    if DEBUG("s"){
	printf("Information on species and variants:\n");
	hash_traverse(species_name_hash, print_species_name_hash_count, 0, 0);
    }

    Fflush(stdout);

    
#ifndef NO_TCL
    max_gen_pop = -1;
    total_gen_pop = 0;
    
    for (row = 0; row < nrows; row++){
	for (col = 0; col < ncols; col++){
	    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(WORLD_SITE(the_world, row, col));
	    
	    fprintf(site_file, "%d ", nagents);
	    fprintf(mutation_file, "%d ", site_migration_average(WORLD_SITE(the_world, row, col)));
	    
	    total_gen_pop += nagents;
	    
	    if (nagents > max_gen_pop){
		max_gen_pop = nagents;
	    }
	}
    }

    fprintf(site_file, "%d %d\n", total_gen_pop, max_gen_pop);
    fprintf(mutation_file, "\n");
    
    Fflush(site_file);
    Fflush(mutation_file);
#else
    if DEBUG("S") {
	printf("Site populations:\n");
		
	for (row = 0; row < nrows; row++){
	    for (col = 0; col < ncols; col++){
		SITE *site = WORLD_SITE(the_world, row, col);
			
		printf("%3d%c", SITE_NAGENTS(site), SITE_PRODUCES(site, 1) ? '*' : ' ');
	    }

	    putchar('\n');
	}
    }
#endif

#ifndef NO_GRAPHICS
    if (graphics == TRUE){
	process_graphics_events();
    }
#endif
    
    while (1){

#ifndef NO_GRAPHICS
	if (graphics == TRUE){
	    if (WORLD_RUNNING(the_world) == FALSE){
		
		/* 
		 * This blocks but ensures we don't sit in a polling loop, as X uses select. 
		 * We can block as a graphics event is needed to get the simulation
		 * moving. i.e. something to set the halt generation higher.
		 */
		await_graphic_event();
		
		/* Now deal with any further events, this does not block. */
		process_graphics_events();
	    }
	}
#endif
	
	while (WORLD_RUNNING(the_world) == TRUE && WORLD_GENERATION(the_world) <= WORLD_HALT_GENERATION(the_world)){
	    
	    if DEBUG("g") {
		printf("GENERATION %d BEGINS:\n", WORLD_GENERATION(the_world)); 
	    }
	    
	    for (row = 0; row < nrows; row++){
		for (col = 0; col < ncols; col++){
		    SITE *site = WORLD_SITE(the_world, row, col);
		    
		    site_make_newborns_old(site);
		    site_reset_migration_flags(site);
		    
#ifndef NO_INTERACTIONS
		    site_perform_interactions(site);
#endif
		    
		    site_produce(site);
		    site_distribute_resources(site);

#ifndef NO_MAINTENANCE
		    site_charge_maintenance(site);
#endif

#ifndef NO_ZAP
		    site_kill_agents_without_cause(site, SITE_P_RANDOM_DEATH(site));
#endif
		    
#ifndef NO_MIGRATION
		    site_migrate_agents(site, migration_probabilities);
#endif
		    
		    /* The number of agents may have changed, so change the number of interactions. */
		    SITE_INTERACTIONS(site) = (AGENT_POPULATION_SIZE) (WORLD_C(the_world) * (double) SITE_NAGENTS(site));
		} 
	    } 
	    
	    /* 
	     * We need to do this loop again, as if we include it in the main loop,
	     * some agents may be subject to mutation or self replication twice in one generation.
	     * This is probably only a small consideration. The reason this could
	     * happen is that we could mutate or replicate an agent and then move it to another
	     * site with site_migrate_agents() where it could be mutated again.
	     */
	     
	    hash_traverse(species_name_hash, zero_genome_name_counts, 0, 0);
	    hash_traverse(genome_hash, mark_genome_as_dead, 0, 0);
	    
	    for (row = 0; row < nrows; row++){
		for (col = 0; col < ncols; col++){
#ifndef NO_SELF_REPLICATION
		    if (self_replication == TRUE){
			site_self_replicate_agents(WORLD_SITE(the_world, row, col));
		    }
#endif

#ifndef NO_MUTATION
		    site_mutate_agents(WORLD_SITE(the_world, row, col));
#endif
		    site_hash_genomes(WORLD_SITE(the_world, row, col), WORLD_GENERATION(the_world));
		    if DEBUG("l") site_list_agents(WORLD_SITE(the_world, row, col), TRUE, stdout);
		}
	    }
	    
#ifndef NO_TCL
	    max_gen_pop = -1;
	    total_gen_pop = 0;
	    
	    for (row = 0; row < nrows; row++){
		for (col = 0; col < ncols; col++){
		    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(WORLD_SITE(the_world, row, col));
	    
		    fprintf(site_file, "%d ", nagents);
		    fprintf(mutation_file, "%d ", site_migration_average(WORLD_SITE(the_world, row, col)));
	    
		    total_gen_pop += nagents;
	    
		    if (nagents > max_gen_pop){
			max_gen_pop = nagents;
		    }
		}
	    }

	    fprintf(site_file, "%d %d\n", total_gen_pop, max_gen_pop);
	    fprintf(mutation_file, "\n");
    
	    Fflush(site_file);
	    Fflush(mutation_file);
#else
	    if DEBUG("S") {
		printf("Site populations:\n");
		
		for (row = 0; row < nrows; row++){
		    for (col = 0; col < ncols; col++){
			SITE *site = WORLD_SITE(the_world, row, col);
			
			printf("%3d%c", SITE_NAGENTS(site), SITE_PRODUCES(site, 1) ? '*' : ' ');
		    }

		    putchar('\n');
		}
	    }
#endif

	    if DEBUG("p") {
		printf("The current population follows:\n");
		hash_traverse(genome_hash, print_genome_hash, TRUE, 0);
	    }
	    
	    if DEBUG("d") {
		printf("The graveyard contains:\n");
		hash_traverse(genome_hash, print_genome_hash, FALSE, 0);
	    }
	    
	    if DEBUG("s") {
		printf("Information on species and variants:\n");
		hash_traverse(species_name_hash, print_species_name_hash_count, 0, 0);
	    }
	    
	    Fflush(stdout);

	    if DEBUG("g") {
		printf("GENERATION %d COMPLETE.\n\n", WORLD_GENERATION(the_world));
		/* fprintf(stderr, "GENERATION %d COMPLETE.\n\n", WORLD_GENERATION(the_world)); */
	    }

#ifndef NO_GRAPHICS
	    if (graphics == TRUE){
		/* Process events if there are any. This will not block. */
		process_graphics_events();
	    }
#endif

#ifndef NO_GRAPHICS
	    if (graphics == TRUE){
		int offset = 0;
		int nvariants_ever = 0;
		int nvariants_alive = 0;
		
		plot_population_level(world_population_size(the_world));
		plot_schema_level();
		plot_resource_levels(world_resource_levels(world_resource_levels_list, the_world));
		hash_traverse(species_name_hash, assign_species_levels, world_species_levels_list, &offset);
		plot_species_levels(world_species_levels_list);
		
		hash_traverse(genome_hash, count_variants, TRUE, &nvariants_alive);
		hash_traverse(genome_hash, count_variants, FALSE, &nvariants_ever);
		plot_variant_levels(nvariants_alive, nvariants_ever);
		update_all_plotters();
	    }
#endif

	    WORLD_GENERATION(the_world)++;
	}
	
	/* We have reached the number of generations we were supposed to attain. */
	WORLD_RUNNING(the_world) = FALSE;
	
	if (graphics == FALSE){
	    break;
	}
    }
    
    return;
}

void 
pause_world()
{
    if (!the_world){
	popup_message("There is no currently running world that can be paused.");
	return;
    }
    
    WORLD_RUNNING(the_world) = FALSE;
    
    printf("Run paused after %d generations.\n", WORLD_GENERATION(the_world));
    return;
}

void 
continue_world()
{
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    WORLD_RUNNING(the_world) = TRUE;
    
    printf("Continue.\n");
    return;
}

void 
run_for_x_generations(n)
int n;
{
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    set_halt_generation(WORLD_GENERATION(the_world) + n - 1);
    WORLD_RUNNING(the_world) = TRUE;
    return;
}

void
run_world_indefinitely()
{
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    WORLD_RUNNING(the_world) = TRUE;
    set_halt_generation(MAX_POSSIBLE_GENERATION);
    printf("Run indefinitely.\n");
    return;
}
