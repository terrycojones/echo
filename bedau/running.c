#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/r/echo/s.orig/RCS/running.c,v 1.7 1994/02/07 23:16:06 terry Exp terry $";
#endif


#include "types.h"
#include "sites.h"
#include "worlds.h"
#include "graphics.h"
#include "syscalls.h"
#include "globals.h"
#include "running.h"

#include <stdio.h>
#include <string.h>

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
    
    /* printf("Run through generation %d.\n", generation); */
    
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
print_bedau_genome_hash(entry, alive, where)
void *entry;
void *alive;
void *where;
{
    GENOME_HASH_DATA *node = ((GENOME_HASH_DATA *)entry);
    FILE *fp = (FILE *)where;
    
    if ((BOOLEAN) alive != node->alive){
	/* Not the kind we are looking for. */
	return 1;
    }

    if ((BOOLEAN) alive == TRUE){
	fprintf(fp, "%s%03d %d ", node->name, node->instance, node->count);
    }

    
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
    /* BEDAU: */
    void do_dump_species();
    void do_dump_genes();
    BOOLEAN some_genes_are_dumping = strlen(genes_to_dump) ? TRUE : FALSE;
    
    register COORD row;
    register COORD nrows;
    register COORD col;
    register COORD ncols;
    static RESOURCE_SIZE *world_resource_levels_list = (RESOURCE_SIZE *) 0;
    static AGENT_POPULATION_SIZE *world_species_levels_list = (AGENT_POPULATION_SIZE *) 0;
    static AGENT_POPULATION_SIZE max_gen_pop;
    static AGENT_POPULATION_SIZE total_gen_pop;
    
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
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

    /* BEDAU: */
    if (dump_species == TRUE){
	do_dump_species();
    }

    /* BEDAU: */
    if (some_genes_are_dumping == TRUE){
	do_dump_genes();
    }

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

	    total_gen_pop += nagents;
	    
	    if (nagents > max_gen_pop){
		max_gen_pop = nagents;
	    }
	}
    }

    fprintf(site_file, "%d %d\n", total_gen_pop, max_gen_pop);
    
    Fflush(site_file);
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
		    
#ifndef NO_INTERACTIONS
		    site_perform_interactions(site);
#endif
		    
		    site_distribute_resources(site);

#ifndef NO_MAINTENANCE
		    site_charge_maintenance(site);
#endif

#ifndef NO_ZAP
		    site_kill_agents_without_cause(site, SITE_P_RANDOM_DEATH(site));
#endif
		    
		    site_produce(site);

#ifndef NO_MIGRATION
		    site_migrate_agents(site);
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
	    
		    total_gen_pop += nagents;
	    
		    if (nagents > max_gen_pop){
			max_gen_pop = nagents;
		    }
		}
	    }

	    fprintf(site_file, "%d %d\n", total_gen_pop, max_gen_pop);
    
	    Fflush(site_file);
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

		/*
		printf("gen %d %d %d %d\n", WORLD_GENERATION(the_world), world_population_size(the_world),
		       nvariants_ever, nvariants_alive);
		*/

		update_all_plotters();
	    }
#endif

	    WORLD_GENERATION(the_world)++;

	    /* BEDAU: */
	    if (some_genes_are_dumping == TRUE){
		do_dump_genes();
	    }

	    /* BEDAU: */
	    if (dump_species == TRUE){
		do_dump_species();
	    }
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

/* BEDAU: */
void
do_dump_species()
{
    fprintf(dump_species_fp, "%d %d ", WORLD_GENERATION(the_world), world_population_size(the_world));
    hash_traverse(genome_hash, print_bedau_genome_hash, TRUE, dump_species_fp);
    putc('\n', dump_species_fp);
    return;
}

/* BEDAU: */
int
print_gene_hash(entry, where, dummy2)
void *entry;
void *where;
void *dummy2;
{
    GENE_HASH_DATA *node = (GENE_HASH_DATA *)entry;
    FILE *fp = (FILE *)where;

    if (node->count){
	/* fprintf(fp, "%s %ld ", node->name, node->count); */
	fprintf(fp, "%s %ld ", strlen(node->name) ? node->name : "E", node->count);
    }
    
    return 1;
}

/* BEDAU: */
int
free_gene_hash(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    GENE_HASH_DATA *node = (GENE_HASH_DATA *)entry;
    
    Free(node->name);
    Free(node);
    
    return 1;
}

/* BEDAU: */
void
do_dump_genes()
{
    /*
     * This does not currently dump out the population total, since we'd
     * have to do the entire loop twice to calculate that first. We can either
     * add that later or else do it via post-processing.
     */
    
    register int gene;
    register COORD row;
    register COORD col;
    register COORD nrows = WORLD_ROWS(the_world);
    register COORD ncols = WORLD_COLS(the_world);

    for (gene = 0; gene < NCHROMOSOMES; gene++){

	/* printf("Before allocating hash table, %ld bytes have been allocated.\n", total_allocated); */
	
	if (gene_fp[gene]){

	    long total_count = 0L;
	    HASH_TABLE *ht = NULL;

	    fprintf(gene_fp[gene], "%d ", WORLD_GENERATION(the_world));

	    if (sum_over_genes == TRUE){
		ht = hash_create(0);
		if (!ht){
		    error("Could not create hash table in do_dump_genes()");
		}
	    }
    

	    for (row = 0; row < nrows; row++){
		for (col = 0; col < ncols; col++){
		    AGENT **agents = SITE_AGENTS(WORLD_SITE(the_world, row, col));
		    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(WORLD_SITE(the_world, row, col));
		    AGENT_POPULATION_SIZE agent;

		    for (agent = 0; agent < nagents; agent++){
			CHROMOSOME *c = AGENT_CHROMOSOME(agents[agent], gene);

			if (sum_over_genes == TRUE){
			    void *hash_return;
			    char name[1024];
			    strcpy(name, CHROMOSOME_STR(c));

			    if ((hash_return = hash_search(ht, name, 0, 0)) == 0){
				/* Not in hash table, insert it. */
				GENE_HASH_DATA *new = (GENE_HASH_DATA *) Malloc(sizeof(GENE_HASH_DATA));

				new->name = strdup(name);
				new->count = CHROMOSOME_USAGE(c);

				/* Insert. */
				hash_search(ht, name, (void *)new, 0);
				if DEBUG("#") {
				    printf("gene %d: generation %ld adding %s to hash table, usage set to %d\n",
					   gene, WORLD_GENERATION(the_world), CHROMOSOME_STR(c), CHROMOSOME_USAGE(c));
				}
			    }
			    else {
				/* It was there, add to its count. */
				((GENE_HASH_DATA *)hash_return)->count += CHROMOSOME_USAGE(c);

				if DEBUG("%") {
				    printf("gene %d: generation %ld found %s in hash table, usage inc'd by %d\n",
					   gene, WORLD_GENERATION(the_world), name, CHROMOSOME_USAGE(c));
				}
			    }

			    total_count += CHROMOSOME_USAGE(c);
			
			    if DEBUG("%") {
				printf("gene %d: generation %ld total_count becomes %ld\n",
				       gene, WORLD_GENERATION(the_world), total_count);
			    }
			}
			else {
			    int count = CHROMOSOME_USAGE(c);
			    char *name = CHROMOSOME_STR(c);

			    if (count){
				total_count += count;

				fprintf(gene_fp[gene], "%ld.%s %ld ", AGENT_ID(agents[agent]), strlen(name) ? name : "E", count);
			    }
			}
			    
		    }
		}
	    }

	    if (sum_over_genes == TRUE){
		fprintf(gene_fp[gene], "%ld ", total_count);
		hash_traverse(ht, print_gene_hash, gene_fp[gene], 0);
	    }
	    else {
		fprintf(gene_fp[gene], "%ld", total_count);
	    }
	    
	    putc('\n', gene_fp[gene]);
	    Fflush(gene_fp[gene]);

	    if (sum_over_genes == TRUE){
		hash_traverse(ht, free_gene_hash, 0, 0);
		hash_free(ht);
	    }
	}
    }

    return;
}

