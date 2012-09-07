#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/chromosomes.c,v 1.7 1992/11/16 07:05:45 terry Exp terry $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "types.h"
#include "globals.h"
#include "chromosomes.h"
#include "syscalls.h"
#include "random.h"

#define DEF_CHROMOSOME_LEN        10
#define DEF_CHROMOSOME_GROWTH_INC 10

CHROMOSOME *
create_chromosome(chromosome)
CHROMOSOME *chromosome;
{
    /*
     * This creates a new chromosome. If chromosome is given, it is
     * copied (including its text), otherwise a default sized 
     * chromosome is created.
     *
     * This function should be called using one of the two macros 
     * new_chromosome() and copy_chromosome() defined in chromosomes.h
     */

    CHROMOSOME *new;

    rcsid = rcsid;
    
    new = (CHROMOSOME *)Malloc(sizeof(CHROMOSOME));
    
    if (chromosome){
	CHROMOSOME_STR(new) = (RESOURCE *)Malloc((CHROMOSOME_MAX_LEN(chromosome) + 1) * sizeof(RESOURCE));
	CHROMOSOME_STR(new)[CHROMOSOME_MAX_LEN(chromosome)] = '\0';
	CHROMOSOME_LEN(new) = CHROMOSOME_LEN(chromosome);
	CHROMOSOME_MAX_LEN(new) = CHROMOSOME_MAX_LEN(chromosome);
	memcpy(CHROMOSOME_STR(new), CHROMOSOME_STR(chromosome), CHROMOSOME_MAX_LEN(chromosome));
    }
    else {
	CHROMOSOME_STR(new) = (RESOURCE *)Malloc((DEF_CHROMOSOME_LEN + 1) * sizeof(RESOURCE));
	CHROMOSOME_STR(new)[0] = '\0';
	CHROMOSOME_STR(new)[DEF_CHROMOSOME_LEN] = '\0';
	CHROMOSOME_LEN(new) = (CHROMOSOME_LENGTH)0;
	CHROMOSOME_MAX_LEN(new) = DEF_CHROMOSOME_LEN;
    }
    
    return new;
}


void
grow_chromosome(chromosome, increment)
CHROMOSOME *chromosome;
CHROMOSOME_LENGTH increment;
{
    /*
     * Add to the length of a chromosome. This is a bit ugly, I
     * hate reallocing and I hope we won't have to do it too often.
     * If we do, we can increase the default sizes.
     */

    register int new_len;
    RESOURCE *new;
    
    if (increment == (CHROMOSOME_LENGTH)0){
	increment = DEF_CHROMOSOME_GROWTH_INC;
    }
#ifdef CHECK_ARGS
    else {
	if (increment < (CHROMOSOME_LENGTH)0){
	    error("grow_chromosome() called with negative increment.");
	}
    }

    if (!chromosome){
	error("grow_chromosome() called with NULL chromosome!");
    }
#endif /* CHECK_ARGS */
    
    new_len = CHROMOSOME_MAX_LEN(chromosome) + increment;
    new = (RESOURCE *)Malloc((new_len + 1) * sizeof(RESOURCE));
    
    /* printf("chromosome grows by %d to size %d\n", increment, new_len); */
    
    new[new_len] = '\0';

    (void) memcpy(new, CHROMOSOME_STR(chromosome), CHROMOSOME_LEN(chromosome));
    new[CHROMOSOME_LEN(chromosome)] = '\0';
    
    Free(CHROMOSOME_STR(chromosome));
    CHROMOSOME_STR(chromosome) = new;
    CHROMOSOME_MAX_LEN(chromosome) = new_len;

    return;
}

#ifndef NO_MUTATION
void
mutate_chromosome(chromosome)
CHROMOSOME *chromosome;
{
    register int mutation_location;
    register int current_length;

#ifdef CHECK_ARGS
    if (!chromosome){
	error("mutate_chromosome() called with NULL chromosome!");
    }
#endif /* CHECK_ARGS */
    
    current_length = CHROMOSOME_LEN(chromosome);
    mutation_location = uniform(current_length + 1);
    
    /* Do we add an allele to the end of the chromosome? */
    if (mutation_location == current_length){
	RESOURCE new_allele = uniform(nresources) + 'a';

	if (current_length == chromosome->max_length){
	    grow_chromosome(chromosome, 0);
	}
	
	CHROMOSOME_STR(chromosome)[current_length] = new_allele;
	CHROMOSOME_STR(chromosome)[current_length + 1] = '\0';
	CHROMOSOME_LEN(chromosome)++;
	
	if DEBUG("u") printf("MUTATION: chromosome mutates (grows) adding a '%c'\n", new_allele);
	return;
    }
    
    /* Do we change the last allele or delete it? */
    if (mutation_location == current_length - 1){
	RESOURCE new_allele = uniform(nresources + 1);
	
	if (new_allele == nresources){
	    /* Delete the last allele. */
	    CHROMOSOME_LEN(chromosome)--;
	    if DEBUG("u") printf("MUTATION: chromosome mutates (shrinks) losing trailing '%c'\n", CHROMOSOME_STR(chromosome)[current_length - 1]);
	    CHROMOSOME_STR(chromosome)[current_length - 1] = '\0';
	}
	else {
	    /* Change the last allele value to the new one. */
	    if DEBUG("u") printf("MUTATION: chromosome mutates last allele from '%c' to '%c'\n", 
			      CHROMOSOME_STR(chromosome)[current_length - 1], new_allele + 'a');
	    CHROMOSOME_STR(chromosome)[current_length - 1] = new_allele + 'a';
	}
    }
    else {
	/* Change an allele value elsewhere on the chromosome. */
	RESOURCE new_allele = uniform(nresources) + 'a';
	
	if DEBUG("u") printf("MUTATION: chromosome mutates allele %d from '%c' to '%c'\n", mutation_location, 
			  CHROMOSOME_STR(chromosome)[mutation_location], new_allele);
	
	CHROMOSOME_STR(chromosome)[mutation_location] = new_allele;
    }

    return;
}
#endif

void
set_chromosome(chromosome, value)
CHROMOSOME *chromosome;
char *value;
{
    register int len;

#ifdef CHECK_ARGS
    if (!chromosome){
	error("set_chromosome() called with NULL chromosome!");
    }
    if (!value){
	error("set_chromosome() called with NULL chromosome value!");
    }
#endif /* CHECK_ARGS */

    len = strlen(value);

    if (len - CHROMOSOME_MAX_LEN(chromosome) > 0){
	grow_chromosome(chromosome, len - CHROMOSOME_MAX_LEN(chromosome));
    }
    
    memcpy(CHROMOSOME_STR(chromosome), value, len);
    CHROMOSOME_STR(chromosome)[len] = '\0';
    CHROMOSOME_LEN(chromosome) = len;
    
    return;
}

void
swap_chromosomes(chromosome1, chromosome2)
CHROMOSOME *chromosome1;
CHROMOSOME *chromosome2;
{
    RESOURCE *tmp = (RESOURCE *) strdup(CHROMOSOME_STR(chromosome1));
    
    set_chromosome(chromosome1, CHROMOSOME_STR(chromosome2));
    set_chromosome(chromosome2, tmp);
    
    Free(tmp);

    return;
}

int
chromosome_distance(chromosome1, chromosome2)
CHROMOSOME *chromosome1;
CHROMOSOME *chromosome2;
{
    /*
     * Returns the number of mutations needed to change chromosome1
     * into chromosome2. This is the difference in their lengths plus
     * the number of loci that they differ at.
     */

    register int distance;
    register int min_length;
    register int i;

    if (CHROMOSOME_LEN(chromosome1) >= CHROMOSOME_LEN(chromosome2)){
	distance = CHROMOSOME_LEN(chromosome1) - CHROMOSOME_LEN(chromosome2);
	min_length = CHROMOSOME_LEN(chromosome2);
    }
    else {
	distance = CHROMOSOME_LEN(chromosome2) - CHROMOSOME_LEN(chromosome1);
	min_length = CHROMOSOME_LEN(chromosome1);
    }
    
    for (i = 0; i < min_length; i++){
	if (CHROMOSOME_STR(chromosome1)[i] != CHROMOSOME_STR(chromosome2)[i]){
	    distance++;
	}
    } 

    return distance;
}

int
chromosome_string_distance(chromosome1, chromosome2)
STRING chromosome1;
STRING chromosome2;
{
    /*
     * Returns the number of mutations needed to change chromosome1
     * into chromosome2. This is the difference in their lengths plus
     * the number of loci that they differ at.
     */

    register int distance;
    register int min_length;
    register int len1 = strlen(chromosome1);
    register int len2 = strlen(chromosome2);
    register int i;

    if (len1 >= len2){
	distance = len1 - len2;
	min_length = len2;
    }
    else {
	distance = len2 - len1;
	min_length = len1;
    }
    
    for (i = 0; i < min_length; i++){
	if (chromosome1[i] != chromosome2[i]){
	    distance++;
	}
    } 

    return distance;
}

void
one_point_crossover(agent_1, agent_2, where)
AGENT *agent_1;
AGENT *agent_2;
int where;
{
    /*
     * We do a one-point crossover inside gene 'where' of both agents.
     * We need to pick a random location in each gene to cut the gene at
     * (locus_1 and locus_2) and the result is that agent_1 will have a
     * new gene that is the same up to its locus_1 and is a copy of
     * agent_2's gene from there on. Agent_2 gets the "same" (mutatis mutandi).
     */
    
    int locus_1;
    int locus_2;
    char *new_1;
    char *new_2;
    
    locus_1 = uniform(CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_1, where) ) );
    locus_2 = uniform(CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_2, where) ) );

    if DEBUG("X"){
	printf("The cut loci in chromosome %d are %d in parent_1 and %d in parent_2\n", where, locus_1, locus_2);
    }

    new_1 = Malloc(CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_1, where)) +
		   CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_2, where)) + 1);
    new_2 = Malloc(CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_1, where)) +
		   CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_2, where)) + 1);

    new_1[0] = '\0';
    new_2[0] = '\0';

    strncat(new_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, where)), locus_1);
    strncat(new_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, where)) + locus_2,
	    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, where)) - locus_2);

    strncat(new_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, where)), locus_2);
    strncat(new_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, where)) + locus_1,
	    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, where)) - locus_1);
    
    set_chromosome(AGENT_CHROMOSOME(agent_1, where), new_1);
    set_chromosome(AGENT_CHROMOSOME(agent_2, where), new_2);

    Free(new_1);
    Free(new_2);
}
