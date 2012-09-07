#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/r/echo/s/RCS/agents.c,v 1.9 1993/12/07 05:59:27 terry Exp terry $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <sys/param.h>

#include "types.h"
#include "defaults.h"
#include "globals.h"
#include "syscalls.h"
#include "agents.h"
#include "sites.h"
#include "worlds.h"
#include "graphics.h"
#include "chromosomes.h"
#include "resources.h"
#include "random.h"
#include "free.h"

AGENT *
create_agent(agent, copy_migration_bits)
AGENT *agent;
BOOLEAN copy_migration_bits;
{
    /*
     * This creates a new agent. If agent is given, it is
     * copied, otherwise a default empty agent is created.
     * If we are copying some existing agent, then we take note
     * of the migration bits flag and copy them too if need be.
     *
     * This function should be called using one of the two macros 
     * new_agent() and copy_agent() defined in agents.h
     */

    register int i;
    AGENT *new = (AGENT *)Malloc(sizeof(AGENT));

    rcsid = rcsid;

    AGENT_RESERVE_LIST(new) = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    AGENT_MAKEUP_LIST(new)  = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    AGENT_UPTAKE_MASK(new)  = (BOOLEAN *)       Malloc(nresources * sizeof(BOOLEAN));
    AGENT_MIGRATION_BIT(new, NMIGRATION_BITS) = '\0';

    if (agent){
	
	AGENT_NAME(new) = strdup(AGENT_NAME(agent));

	for (i = 0; i < NCHROMOSOMES; i++){
	    AGENT_CHROMOSOME(new, i) = copy_chromosome(AGENT_CHROMOSOME(agent, i));
	} 
	
	set_resources(AGENT_RESERVE_LIST(new), AGENT_RESERVE_LIST(agent));
	set_resources(AGENT_MAKEUP_LIST(new), AGENT_MAKEUP_LIST(agent));
	
	for (i = 0; i < nresources; i++){
	    AGENT_UPTAKE(new, i) = AGENT_UPTAKE(agent, i);
	} 

	if (copy_migration_bits == TRUE){
	    for (i = 0; i < NMIGRATION_BITS; i++){
		AGENT_MIGRATION_BIT(new, i) = AGENT_MIGRATION_BIT(agent, i);
	    }
	}
	
	AGENT_TRADING_RESOURCE(new) = AGENT_TRADING_RESOURCE(agent);
	AGENT_GENOME(new) = strdup(AGENT_GENOME(agent));
	AGENT_GENOME_LEN(new) = AGENT_MAX_GENOME_LEN(new) = AGENT_GENOME_LEN(agent);
    }
    else {
	for (i = 0; i < NCHROMOSOMES; i++){
	    AGENT_CHROMOSOME(new, i) = new_chromosome();
	} 
    
	zero_resources(AGENT_RESERVE_LIST(new));
	zero_resources(AGENT_MAKEUP_LIST(new));
    
	for (i = 0; i < nresources; i++){
	    AGENT_UPTAKE(new, i) = UNINITIALIZED;
	} 
	
	AGENT_TRADING_RESOURCE(new) = '\0';
	AGENT_GENOME(new) = NULL;
	AGENT_GENOME_LEN(new) = AGENT_MAX_GENOME_LEN(new) = (RESOURCE_SIZE)0;
    }

    if (!agent || copy_migration_bits == FALSE){
	for (i = 0; i < NMIGRATION_BITS; i++){
	    AGENT_MIGRATION_BIT(new, i) = uniform(2) ? '1' : '0';
	}
    }
	
    AGENT_HAS_MIGRATED(new) = FALSE;
    AGENT_GAINED(new) = FALSE;
    AGENT_NEWBORN(new) = TRUE;
#ifndef NO_SELF_REPLICATION
    AGENT_TIME_SINCE_REPLICATION(new) = 0;
#endif

    make_agent_genome(new);
    
    return new;
}

AGENT_EDIT *
new_agent_edit()
{
    /*
     * This creates a new editing agent.
     */

    register int i;
    AGENT_EDIT *new = (AGENT_EDIT *)Malloc(sizeof(AGENT_EDIT));

    AGENT_EDIT_RESERVE_LIST(new) = "";
    AGENT_EDIT_UPTAKE_MASK(new) = "";
    AGENT_EDIT_NAME(new) = "";
	
    for (i = 0; i < NCHROMOSOMES; i++){
	AGENT_CHROMOSOME(new, i) = "";
    } 
    
    AGENT_EDIT_RESERVE_LIST(new) = "";
    AGENT_EDIT_UPTAKE_MASK(new) = "";
    AGENT_EDIT_TRADING_RESOURCE(new) = "";
    
    return new;
}

/*
 * This is not needed in the Haas version since agent replication
 * is based on time, not on whether they have accumulated enough
 * resources to make a copy of themselves.
 */
#if 0
#ifndef NO_SELF_REPLICATION
BOOLEAN
can_self_replicate(agent)
AGENT *agent;
{
    /*
     * Return TRUE if an agent has enough resources in its
     * reservoir to self-reproduce. This takes into account
     * the threshold, which is the number of copies of itself
     * an agent must be able to make before it actrually makes one.
     *
     * Watch for agents that have
     * absolutely no resources in their genome - don't
     * let them self-replicate or they'll fill the world
     * exponentially quickly.
     */
    register NRESOURCES i;
    register int total_makeup = 0;
    double threshold = WORLD_SELFREP_THRESHOLD(the_world);

#ifdef CHECK_ARGS
    if (!agent){
	error("can_self_replicate() called with NULL agent!");
    }
#endif /* CHECK_ARGS */
    
    for (i = 0; i < nresources; i++) {         
	if (AGENT_RESERVE(agent, i) < (int) ceil(threshold * (double) AGENT_MAKEUP(agent, i))){
	    return FALSE;
	}

	total_makeup += AGENT_MAKEUP(agent, i);
    }                                                
	
    return total_makeup ? TRUE : FALSE;
}
#endif
#endif

void
calculate_makeup(agent)
AGENT *agent;
{
    /*
     * Fill in the agent's makeup array by totalling the various
     * resources in its chromosomes. Don't look at its reserve.
     */

    register CHROMOSOME_LENGTH i;
    register CHROMOSOME_LENGTH len;
    
#ifdef CHECK_ARGS
    if (!agent){
	error("calculate_makeup() called with NULL agent!");
    }
#endif /* CHECK_ARGS */

    zero_resources(AGENT_MAKEUP_LIST(agent));
    
    len = AGENT_OFFENSE_TAG_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_OFFENSE_TAG(agent)[i] - 'a')++;
    } 
    
    len = AGENT_DEFENSE_TAG_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_DEFENSE_TAG(agent)[i] - 'a')++;
    } 
    
    len = AGENT_MATING_TAG_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_MATING_TAG(agent)[i] - 'a')++;
    } 
    
    len = AGENT_COMBAT_COND_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_COMBAT_COND(agent)[i] - 'a')++;
    } 
    
    len = AGENT_TRADE_COND_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_TRADE_COND(agent)[i] - 'a')++;
    } 
    
    len = AGENT_MATING_COND_LEN(agent);
    for (i = 0; i < len; i++){
	AGENT_MAKEUP(agent, AGENT_MATING_COND(agent)[i] - 'a')++;
    } 
    
    return;
}

#ifndef NO_MAINTENANCE
BOOLEAN
can_pay_maintenance(costs, agent)
RESOURCE_LIST costs;
AGENT *agent;
{
    /*
     * Return TRUE if an agent has enough resources in its
     * reservoir to cover the costs in the costs array.
     */
    register NRESOURCES i;                                   
    
#ifdef CHECK_ARGS
    if (!agent){
	error("can_pay_maintenance() called with NULL agent!");
    }
    
    if (!costs){
	error("can_pay_maintenance() called with NULL costs!");
    }
    
#endif /* CHECK_ARGS */
    
    for (i = 0; i < nresources; i++) {         
	if (AGENT_RESERVE(agent, i) < costs[i]){
	    return FALSE;
	}
    }                                                
    
    return TRUE;
}
#endif

#ifndef NO_MUTATION
COUNT
mutate_agent(agent, mutation_probability)
AGENT *agent;
PROBABILITY mutation_probability;
{
    register int i;
    COUNT mutation_count;

#ifdef CHECK_ARGS
    if (!agent){
	error("mutate_agent() called with NULL agent!");
    }
#endif /* CHECK_ARGS */

    mutation_count = (COUNT)0;

    /* Chromosomes do not mutate in Haas Echo since all interactions are turned off. */
#if 0
    /* Mutate each of the chromosomes. */
    for (i = 0; i < NCHROMOSOMES; i++){
	if (biased_coin_flip(mutation_probability)){
	    if DEBUG("u") printf("MUTATION: %s mutates in agent %s%03d\n", chromosome_names[i], AGENT_NAME(agent), agent_instance(agent));
	    mutate_chromosome(AGENT_CHROMOSOME(agent, i));
	    mutation_count++;
	}
    } 
    
    /* Mutate the trading resource. */
    if (biased_coin_flip(mutation_probability)){
	RESOURCE old_trading_resource = AGENT_TRADING_RESOURCE(agent);
	AGENT_TRADING_RESOURCE(agent) = uniform(nresources) + 'a';
	if (old_trading_resource != AGENT_TRADING_RESOURCE(agent)){
	    if DEBUG("u") printf("MUTATION: trading resource mutates from %c to %c in agent %s%03d\n", old_trading_resource, 
			      AGENT_TRADING_RESOURCE(agent), AGENT_NAME(agent), agent_instance(agent));
	    mutation_count++;
	}
    }
    
    /* Mutate the uptake mask. */
    if (biased_coin_flip(mutation_probability)){
	NRESOURCES bit = uniform(nresources);
	BOOLEAN old_bit_value = AGENT_UPTAKE(agent, bit);
	AGENT_UPTAKE(agent, bit) = random_boolean();
	if (old_bit_value != AGENT_UPTAKE(agent, bit)){
	    mutation_count++;
	    if DEBUG("u") printf("MUTATION: uptake mask bit %d changes from %s to %s in agent %s%03d\n" , bit, 
			      old_bit_value == TRUE ? "TRUE" : "FALSE",
			      AGENT_UPTAKE(agent, bit) == TRUE ? "TRUE" : "FALSE", 
			      AGENT_NAME(agent), agent_instance(agent));
	}
    }
#endif

#ifndef NO_MIGRATION
    /* Mutate the migration bits. */
    for (i = 0; i < NMIGRATION_BITS; i++){
	if (biased_coin_flip(mutation_probability)){
	    if DEBUG("u") printf("MUTATION: migration bit %d mutates in agent %s%03d\n", i,
				 AGENT_NAME(agent), agent_instance(agent));
	    
	    if (AGENT_MIGRATION_BIT(agent, i) == '1'){
		AGENT_MIGRATION_BIT(agent, i) = '0';
	    }
	    else {
		AGENT_MIGRATION_BIT(agent, i) = '1';
	    }
	    
	    mutation_count++;
	}
    }
#endif


    /* Rebuild the genome from the chromosomes if necessary. */
    if (mutation_count != (COUNT) 0){
#if 0
	/* No need to re-calculate since all mutations (except migration bits) are off. */
	calculate_makeup(agent);
#endif
	make_agent_genome(agent);
	AGENT_NEWBORN(agent) = TRUE;
    }

    return mutation_count;
}
#endif

#ifndef NO_MIGRATION
void
migrate_agent(site, agent_index)
SITE *site;
AGENT_POPULATION_SIZE agent_index;
{
    /*
     * Migrate the agent to a neighbouring site.
     * The world wraps around at edges and sides.
     */

    COORD new_row;
    COORD new_col;
    
#ifdef CHECK_ARGS
    if (!site){
	error("migrate_agent() called with NULL site!");
    }
    
    if (agent_index < (AGENT_POPULATION_SIZE)0){
	error("migrate_agent() called with agent_index = %d", agent_index);
    }
    
    if (agent_index >= SITE_NAGENTS(site)){
	error("migrate_agent() called with agent_index >= SITE_NAGENTS (%d >= %d)", agent_index, SITE_NAGENTS(site));
    }
#endif /* CHECK_ARGS */

    if (WORLD_ROWS(the_world) == 1 && WORLD_COLS(the_world) == 1){
	/* There's no place like home. */
	return;
    }
    
    new_row = SITE_ROW(site);
    new_col = SITE_COL(site);
    
    switch (WORLD_NEIGHBORHOOD(the_world)){
    
	case NONE:{
	    return;
	}

	case NEWS:{
	    switch (uniform(4)){
		case NORTH: new_row--; break;
		case SOUTH: new_row++; break;
		case EAST: new_col++; break;
		case WEST: new_col--; break;
	    }
	    break;
	}
	
	case ALLEIGHT:{
	    switch (uniform(8)){
		case NORTH: new_row--; break;
		case SOUTH: new_row++; break;
		case EAST: new_col++; break;
		case WEST: new_col--; break;
		case NORTH_EAST: new_row--; new_col++; break;
		case SOUTH_EAST: new_row++; new_col++; break;
		case NORTH_WEST: new_row--; new_col--; break;
		case SOUTH_WEST: new_row++; new_col--; break;
	    }
	    break;
	}
	
	default:{
	    error("found unknown neighborhood type in migrate_agent().");
	}
    }
    
    /* Adjust for the edges of the world. */
    if (new_row >= WORLD_ROWS(the_world)){
	new_row = (COORD)0;
    }
    else {
	if (new_row < (COORD)0){
	    new_row = WORLD_ROWS(the_world) - (COORD)1;
	}
    }

    if (new_col >= WORLD_COLS(the_world)){
	new_col = (COORD)0;
    }
    else {
	if (new_col < (COORD)0){
	    new_col = WORLD_COLS(the_world) - (COORD)1;
	}
    }
    
    /* Now we move the agent. */
    site_add_agent(SITE_AGENT(site, agent_index), WORLD_SITE(the_world, new_row, new_col));
    site_delete_agent(site, agent_index, FALSE, FALSE);

    if DEBUG("m") printf("MIGRATION: agent %d at site (%d, %d) migrates to (%d, %d).\n", 
		      agent_index, SITE_ROW(site), SITE_COL(site), new_row, new_col);
    return;
}
#endif


#define BUF_SZ 8092

AGENT *
read_agent_from_file(dir, file)
STRING dir;
STRING file;
{
    /*
     * Read an agent from a file, create a new agent data structure
     * and fill in the fields. 'dir' may be empty, in which case we
     * just open 'file'. If not, we concatenate the two and open the
     * result.
     */

    AGENT *new;
    AGENT_EDIT *edit;
    NRESOURCES resource;
    int chromosome;
    
#ifdef CHECK_ARGS
    if (!file){
	error("read_agent_from_file() called with NULL file argument.");
    }
#endif

    edit = read_agent_edit_from_file(dir, file);

    if (!edit){
	return (AGENT *) 0;
    }
    
    new = new_agent();
    
    AGENT_NAME(new) = strdup( AGENT_EDIT_NAME(edit) );
    
    /* Get and check the initial reservoir levels. */
    if (str_to_resource_list(AGENT_EDIT_RESERVE_LIST(edit), AGENT_RESERVE_LIST(new), nresources) == FALSE){
	popup_message("Could not convert agent reservoir list\ninto %d integers in file '%s'.", nresources, file);
	free_agent_edit(new);
	return (AGENT *) 0;
    }
    
    /* Get resource uptake mask. */
    for (resource = 0; resource < nresources; resource++){
	AGENT_UPTAKE(new, resource) = (AGENT_EDIT_UPTAKE(edit, resource) == '1') ? TRUE : FALSE;
    } 
    
    /* Get the trading resource. */
    AGENT_TRADING_RESOURCE(new) = *AGENT_EDIT_TRADING_RESOURCE(edit);
    
    /* Set up all the chromosomes. */
    for (chromosome = 0; chromosome < NCHROMOSOMES; chromosome++){
	set_chromosome(AGENT_CHROMOSOME(new, chromosome), AGENT_EDIT_CHROMOSOME(edit, chromosome));
    } 
    
    /* Total up the resources contained in all the chromosomes. */
    calculate_makeup(new);
    
    /* Create the genome from the chromosomes. */
    make_agent_genome(new);

    return new;
}

AGENT_EDIT *
read_agent_edit_from_file(dir, file)
STRING dir;
STRING file;
{
    /*
     * Read an agent from a file, create a new agent_edit data structure
     * and fill in the fields. 'dir' may be empty, in which case we
     * just open 'file'. If not, we concatenate the two and open the
     * result.
     */

    FILE *fp;
    STRING name = file;
    AGENT_EDIT *new;
    register int i; 
    char line[BUF_SZ];
    int line_num = 0;
    
#ifdef CHECK_ARGS
    if (!file){
	error("read_agent_edit_from_file() called with NULL file argument.");
    }
#endif

    if (dir){
	if (dir[strlen(dir) - 1] == '/'){
	    name = Malloc(strlen(dir) + strlen(file) + 1);
	    sprintf(name, "%s%s", dir, file);
	}
	else {
	    name = Malloc(strlen(dir) + 1 + strlen(file) + 1);
	    sprintf(name, "%s/%s", dir, file);
	}
    }
    
    fp = Fopen(name, "r");
    new = new_agent_edit();

    while (fgets(line, BUF_SZ, fp)){
	STRING variable;
	STRING value;
	BOOLEAN recognized = FALSE;

	line_num++;
	
	variable = strtok(line, " :\t\n");
	
	/* Watch for empty or comment lines. */
	if (!variable || *variable == '#'){
	    continue;
	}
	
	/* Is this the name? */
	if (!strcmp(variable, NAME_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_agent_edit(new);
		return (AGENT_EDIT *) 0;
	    }
	
	    AGENT_EDIT_NAME(new) = strdup(value);
	    continue;
	}
	
	/* Is this the reservoir? */
	if (!strcmp(variable, RESERVOIR_STR)){
	    register char *nl;

	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		popup_message("Could not find newline character on line %d\nof '%s'!", line_num, name);
		free_site_edit(new);
		return (AGENT_EDIT *) 0;
	    }
	    
	    *nl = '\0';

	    AGENT_EDIT_RESERVE_LIST(new) = strdup(value);
	    continue;
	}
	
	/* Is this the uptake mask? */
	if (!strcmp(variable, UPTAKE_MASK_STR)){
    
	    register STRING tmp;
	    
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_agent_edit(new);
		return (AGENT_EDIT *) 0;
	    }
	
	    tmp = value;
	    
	    while (*tmp){
		if (*tmp != '0' && *tmp != '1'){
		    popup_message("Invalid character '%c' found in uptake mask on line %d in\n'%s'. Please use '0' or '1'.", 
				  *tmp, line_num, name);
		    free_agent_edit(new);
		    return (AGENT_EDIT *) 0;
		}
		tmp++;
	    } 
	    
	    AGENT_EDIT_UPTAKE_MASK(new) = strdup(value);
	    continue;
	}
    
	/* Is this the trading resource? */
	if (!strcmp(variable, TRADING_RESOURCE_STR)){
	
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_agent_edit(new);
		return (AGENT_EDIT *) 0;
	    }
	
	    if (strlen(value) != 1){
		popup_message("%s line on line %d of '%s'\ndoes not contain exactly 1 trading resource character.", 
		      TRADING_RESOURCE_STR, line_num, name);
		free_agent_edit(new);
		return (AGENT_EDIT *) 0;
	    }
	    
	    if (!IS_VALID_RESOURCE(*value)){
		popup_message("'%c' is not a valid resource, on line %d in\n'%s'.", *value, line_num, name);
		free_agent_edit(new);
		return (AGENT_EDIT *) 0;
	    }

	    AGENT_EDIT_TRADING_RESOURCE(new) = strdup(value);
	    
	    continue;
	}

	/* Is it one of the chromosomes we know about? */
	for (i = 0; i < NCHROMOSOMES; i++){
	    if (!strcmp(variable, chromosome_names[i])){
		value = strtok(NULL, " :\t\n");
		
		if (!value){
		    variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		    free_agent_edit(new);
		    return (AGENT_EDIT *) 0;
		}
	
		AGENT_EDIT_CHROMOSOMES(new)[i] = strdup(value);
		recognized = TRUE;
		break;
	    }
	} 
	
	if (recognized == FALSE){
	    /* We don't know what it is... */
	    popup_message("Unrecognized line (%d) in file\n'%s'.", line_num, name);
	    free_agent_edit(new);
	    return (AGENT_EDIT *) 0;
	}
    }

    if (dir){
	Free(name);
    }

    Fclose(fp);
    return new;
}

void
write_agent_to_file(dir, file, agent)
STRING dir;
STRING file;
AGENT *agent;
{
    FILE *in_fp;
    FILE *out_fp;
    STRING name = file;
    register int i; 
    char tmp_file[MAXPATHLEN];
    char line[BUF_SZ];
    extern STRING mktemp();

#ifdef CHECK_ARGS
    if (!file){
	error("write_agent_to_file() called with NULL file argument.");
    }
#endif
    
    sprintf(tmp_file, "/tmp/__echo__XXXXXX");
    mktemp(tmp_file);

    if (!tmp_file){
	error("could not make temporary file in write_agent_to_file().");
    }
    
    out_fp = Fopen(tmp_file, "w");
    
    if (dir){
	if (dir[strlen(dir) - 1] == '/'){
	    name = Malloc(strlen(dir) + strlen(file) + 1);
	    sprintf(name, "%s%s", dir, file);
	}
	else {
	    name = Malloc(strlen(dir) + 1 + strlen(file) + 1);
	    sprintf(name, "%s/%s", dir, file);
	}
    }
    
    in_fp = fopen(name, "r");
    
    if (in_fp){
	/* Read the comment and blank lines at the start of the old file, if any. */
	while (fgets(line, BUF_SZ, in_fp)){
	    STRING tmp = line;
	    
	    while (*tmp == ' ' || *tmp == '\t'){
		tmp++;
	    }
	    
	    if (*tmp == '\n' || *tmp == '#'){
		fprintf(out_fp, "%s", line);
	    }
	    else {
		break;
	    }
	}
	
	Fclose(in_fp);
    }
    
    fprintf(out_fp, "%s : %c\n", TRADING_RESOURCE_STR, AGENT_TRADING_RESOURCE(agent));
    fprintf(out_fp, "%s : ", UPTAKE_MASK_STR);

    for (i = 0; i < nresources; i++){
	if ((AGENT_UPTAKE(agent, i) == UNINITIALIZED)){
	    break;
	}
	putc((AGENT_UPTAKE(agent, i) == TRUE) ? '1' : '0', out_fp);
    } 
    putc('\n', out_fp);
    
    fprintf(out_fp, "%s :", RESERVOIR_STR);
    for (i = 0; i < nresources; i++){
	fprintf(out_fp, " %d", AGENT_RESERVE(agent, i));
    }
    putc('\n', out_fp);
    
    for (i = 0; i < NCHROMOSOMES; i++){
	fprintf(out_fp, "%s : %s\n", chromosome_names[i], CHROMOSOME_STR( AGENT_CHROMOSOME(agent, i) ));
    }
    
    Fclose(out_fp);

    /*
     * Now we need to move the stuff in the new file into a file whose
     * name is the same as the old file. We could use rename() but that doesn't
     * work across filesystems. We could open for update, but that doesn't
     * work under system 5, so let's be a little old fashioned. Read, Write... etc.
     */

    in_fp = Fopen(tmp_file, "r");
    out_fp = Fopen(name, "w");
    
    while (fgets(line, BUF_SZ, in_fp)){
	fprintf(out_fp, "%s", line);
    }
    
    Fclose(in_fp);
    Fclose(out_fp);
    
    if (dir){
	Free(name);
    }
    
    Unlink(tmp_file);

    return;
}

void
write_agent_edit_to_file(dir, file, agent)
STRING dir;
STRING file;
AGENT_EDIT *agent;
{
    FILE *in_fp;
    FILE *out_fp;
    STRING name = file;
    register int i; 
    char tmp_file[MAXPATHLEN];
    char line[BUF_SZ];
    extern STRING mktemp();

#ifdef CHECK_ARGS
    if (!file){
	error("write_agent_to_file() called with NULL file argument.");
    }
#endif
    
    sprintf(tmp_file, "/tmp/__echo__XXXXXX");
    mktemp(tmp_file);

    if (!tmp_file){
	error("could not make temporary file in write_agent_to_file().");
    }
    
    out_fp = Fopen(tmp_file, "w");
    
    if (dir){
	if (dir[strlen(dir) - 1] == '/'){
	    name = Malloc(strlen(dir) + strlen(file) + 1);
	    sprintf(name, "%s%s", dir, file);
	}
	else {
	    name = Malloc(strlen(dir) + 1 + strlen(file) + 1);
	    sprintf(name, "%s/%s", dir, file);
	}
    }
    
    in_fp = fopen(name, "r");
    
    if (in_fp){
	/* Read the comment and blank lines at the start of the old file, if any. */
	while (fgets(line, BUF_SZ, in_fp)){
	    STRING tmp = line;
	    
	    while (*tmp == ' ' || *tmp == '\t'){
		tmp++;
	    }
	    
	    if (*tmp == '\n' || *tmp == '#'){
		fprintf(out_fp, "%s", line);
	    }
	    else {
		break;
	    }
	}
	
	Fclose(in_fp);
    }
    
    fprintf(out_fp, "%s : %s\n", NAME_STR, AGENT_EDIT_NAME(agent));
    fprintf(out_fp, "%s : %s\n", RESERVOIR_STR, AGENT_EDIT_RESERVE_LIST(agent));
    fprintf(out_fp, "%s : %s\n", TRADING_RESOURCE_STR, AGENT_EDIT_TRADING_RESOURCE(agent));
    fprintf(out_fp, "%s : %s\n", UPTAKE_MASK_STR, AGENT_EDIT_UPTAKE_MASK(agent));

    for (i = 0; i < NCHROMOSOMES; i++){
	fprintf(out_fp, "%s : %s\n", chromosome_names[i], AGENT_EDIT_CHROMOSOMES(agent)[i]);
    }
    
    Fclose(out_fp);

    /*
     * Now we need to move the stuff in the new file into a file whose
     * name is the same as the old file. We could use rename() but that doesn't
     * work across filesystems. We could open for update, but that doesn't
     * work under system 5, so let's be a little old fashioned. Read, Write... etc.
     */

    in_fp = Fopen(tmp_file, "r");
    out_fp = Fopen(name, "w");
    
    while (fgets(line, BUF_SZ, in_fp)){
	fprintf(out_fp, "%s", line);
    }
    
    Fclose(in_fp);
    Fclose(out_fp);
    
    if (dir){
	Free(name);
    }
    
    Unlink(tmp_file);

    return;
}

#undef BUF_SZ

#ifndef NO_SELF_REPLICATION
AGENT *
agent_self_replicate(agent)
AGENT *agent;
{
    AGENT *child;
    NRESOURCES resource;
    FRACTION share = WORLD_SELFREP_FRACTION(the_world);
    
    if DEBUG("R") printf("SELFREP: the following agent will now replicate!\n");
    if DEBUG("R") print_agent(agent, stdout);
    if DEBUG("r") printf("self-replication produces a %s%03d\n", AGENT_NAME(agent), agent_instance(agent));

    /* Make the child. */
    child = copy_agent(agent, TRUE);

    /*
     * We don't subtract the child's makeup from the parent's
     * reserve, since it may not have enough. This is possible
     * because we didn't decide to self-reproduce based on the
     * fact that the parent had enough, but rather because the
     * agent had been around for long enough.
     */
#if 0
    /* Take child's makeup resources from the parent. */
    sub_resources(AGENT_RESERVE_LIST(agent), AGENT_MAKEUP_LIST(agent));
#endif
    
    /* Give the child some fraction of the parent's reservoir. */
    for (resource = 0; resource < nresources; resource++){
	AGENT_RESERVE(child, resource) = (RESOURCE_SIZE) ((double) AGENT_RESERVE(agent, resource) * share);
    } 
    
    /* Subtract the child reserves (which we just set up) from the parent. */
    sub_resources(AGENT_RESERVE_LIST(agent), AGENT_RESERVE_LIST(child));

    
    if DEBUG("R") {
	printf("SELFREP: the following two agents resulted (original, child)\n");
	print_agent(agent, stdout);
	print_agent(child, stdout);
    }

    return child;
}
#endif

void
print_agent(agent, fp)
AGENT *agent;
FILE *fp;
{
    register int chromosome;
    register int i;
    NRESOURCES resource;

#if 0    /* This shit is way too verbose. */
    /* NAME */
    fprintf(fp, "Agent name                             : %s\n", AGENT_NAME(agent));
    
    
    /* TRADING RESOURCE */
    fprintf(fp, "Trading Resource                       : %c\n", AGENT_TRADING_RESOURCE(agent));
    
    
    /* RESERVES */
    fprintf(fp, "Agent reserves                         : ");
    print_resource_list(AGENT_RESERVE_LIST(agent), TRUE, fp);
    
    
    /* GENOME */
    fprintf(fp, "Agent genome                           : %s\n", AGENT_GENOME(agent));


    /* CHROMOSOMES */
    for (chromosome = 0; chromosome < NCHROMOSOMES; chromosome++){
	fprintf(fp, "%-39s: %s\n", chromosome_names[chromosome], CHROMOSOME_STR( AGENT_CHROMOSOME(agent, chromosome)));
    } 
    
    
    /* MAKEUP */
    fprintf(fp, "Agent resource totals (in chromosomes) : ");
    print_resource_list(AGENT_MAKEUP_LIST(agent), TRUE, fp);
    
    
    /* UPTAKE MASK */
    fprintf(fp, "Agent uptake mask (TRUE = can uptake)  : ");
    
    for (resource = 0; resource < nresources - 1; resource++){
	fprintf(fp, "[%c = %s]  ", 'a' + resource, AGENT_UPTAKE(agent, resource) == TRUE ? "TRUE" : "FALSE");
    } 
    fprintf(fp, "[%c = %s]\n", 'a' + resource, AGENT_UPTAKE(agent, resource) == TRUE ? "TRUE" : "FALSE");
    
    
    /* NEWBORN */
    fprintf(fp, "Born this generation                   : %s\n", AGENT_NEWBORN(agent) == TRUE ? "TRUE" : "FALSE");
    
    
    /* GAINED */
    fprintf(fp, "Gained resources this generation       : %s\n", AGENT_GAINED(agent) == TRUE ? "TRUE" : "FALSE");

#ifndef NO_MIGRATION
    /* MIGRATION */
    fprintf(fp, "Migration bits                         : ");
    
    for (i = 0; i < NMIGRATION_BITS; i++){
	putc(AGENT_MIGRATION_BIT(agent, i), fp);
    }

    putc('\n', fp);
#endif
#endif /* 0 */

    fprintf(fp, "%s: %s, reserves(", AGENT_NAME(agent), AGENT_GENOME(agent));
    print_resource_list(AGENT_RESERVE_LIST(agent), TRUE, fp);
    fprintf(fp, ")%s%s", AGENT_NEWBORN(agent) == TRUE ? ", newborn" : "", AGENT_GAINED(agent) == TRUE ? ", gained" : "");
    
#ifndef NO_MIGRATION
    /* MIGRATION */
    fprintf(fp, ", migration ");
    
    for (i = 0; i < NMIGRATION_BITS; i++){
	putc(AGENT_MIGRATION_BIT(agent, i), fp);
    }
#endif
    
    putc('\n', fp);
    
    return;
}

void
make_agent_genome(agent)
AGENT *agent;
{
    /*
     * The chars in the chromosomes, an '_' after each, one char trading resource,
     * one '_', uptake mask, one '_', migration bits.
     */
    RESOURCE_SIZE genome_length =
	total_resources(AGENT_MAKEUP_LIST(agent)) /* Total for Chromosomes. */ +
	    NCHROMOSOMES /* An underscore after each chromosome. */ +
		1 /* Trading Resource */ +
		    1 /* Underscore */ +
			nresources /* Uptake Mask */
#ifndef NO_MIGRATION
			    +
				1 /* Underscore */ +
				    NMIGRATION_BITS /* Migration Bits. */
#endif
					;

    static char resource_string[MAX_RESOURCES];

    if (AGENT_GENOME(agent)){
	if (AGENT_MAX_GENOME_LEN(agent) < genome_length){
	    /* It will not fit. */
	    Free(AGENT_GENOME(agent));
	    AGENT_GENOME(agent) = Malloc(genome_length + 1);
	    AGENT_GENOME_LEN(agent) = AGENT_MAX_GENOME_LEN(agent) = genome_length;
	}
	else {
	    AGENT_GENOME_LEN(agent) = genome_length;
	}
    }
    else {
	AGENT_GENOME(agent) = Malloc(genome_length + 1);
	AGENT_GENOME_LEN(agent) = AGENT_MAX_GENOME_LEN(agent) = genome_length;
    }
    
    /* We have enough space. Build the genome. This should be generalized, but for now this is faster. */
    
    if (NCHROMOSOMES == 6){
	register int i;
	
	for (i = 0; i < nresources; i++){
	    resource_string[i] = AGENT_UPTAKE(agent, i) == TRUE ? '1' : '0';
	}

	resource_string[i] = '\0';

#ifndef NO_MIGRATION
	sprintf(AGENT_GENOME(agent), "%s_%s_%s_%s_%s_%s_%c_%s_%s",
		AGENT_OFFENSE_TAG(agent), AGENT_DEFENSE_TAG(agent), AGENT_MATING_TAG(agent),
		AGENT_COMBAT_COND(agent), AGENT_TRADE_COND(agent), AGENT_MATING_COND(agent),
		AGENT_TRADING_RESOURCE(agent), resource_string, AGENT_MIGRATION(agent));
#else
	sprintf(AGENT_GENOME(agent), "%s_%s_%s_%s_%s_%s_%c_%s",
		AGENT_OFFENSE_TAG(agent), AGENT_DEFENSE_TAG(agent), AGENT_MATING_TAG(agent),
		AGENT_COMBAT_COND(agent), AGENT_TRADE_COND(agent), AGENT_MATING_COND(agent),
		AGENT_TRADING_RESOURCE(agent), resource_string);
#endif
	
    }
    else {
	error("oops! there are not 6 chromosomes. agents.c needs work. sorry.");
    }

    return;
}


AGENT_POPULATION_SIZE
agent_instance(agent)
AGENT *agent;
{
    void *hash_return;
    
    if ((hash_return = hash_search(genome_hash, AGENT_GENOME(agent), 0, 0)) == 0){
	/*
	 * This indicates that we couldn't find it in the genome hash table, which
	 * is because it was created this generation and hasn't been hashed yet.
	 */
	return 0;
	
	/*
	 * Here is the code I used to have here.
	 * error("could not find agent genome '%s' in genome hash table.", AGENT_GENOME(agent));
	 */
    }

    return ((GENOME_HASH_DATA *)hash_return)->instance;
}

#ifndef NO_SEX

#define swap_type(type, a, b)       {type tmp = (a); (a) = (b); (b) = tmp;}
#define swap_ints_if_gt(a, b)       if ((a) > (b)) {swap_type(int, (a), (b))}
#define swap_trading_resource(a, b) swap_type(RESOURCE, AGENT_TRADING_RESOURCE((a)), AGENT_TRADING_RESOURCE((b)))

#define IN_CHROMOSOMES(a)           (a < NCHROMOSOMES)
#define IN_TRADING_RESOURCE(a)      (a == NCHROMOSOMES)
#define IN_UPTAKE_MASK(a)           (a == NCHROMOSOMES + 1)
#define IN_MIGRATION_BITS(a)        (a == NCHROMOSOMES + 2)

#if 0
char *
my_strncat(s1, s2, n)
char *s1;
char *s2;
int n;
{
    printf("my_strncat(\"%s\", \"%s\", %d)\n", s1, s2, n);
    return strncat(s1, s2, n);
}
#endif

void
swap_uptake_mask(agent_1, agent_2, from, to)
AGENT *agent_1;
AGENT *agent_2;
int from;
int to;
{
    while (from < to){
	swap_type(BOOLEAN, AGENT_UPTAKE(agent_1, from), AGENT_UPTAKE(agent_2, from));
	from++;
    }
    
    return;
}

void
swap_migration_bits(agent_1, agent_2, from, to)
AGENT *agent_1;
AGENT *agent_2;
int from;
int to;
{
    while (from < to){
	swap_type(char, AGENT_MIGRATION_BIT(agent_1, from), AGENT_MIGRATION_BIT(agent_2, from));
	from++;
    }
    
    return;
}

void
agent_crossover(agent_1, agent_2, site)
AGENT *agent_1;
AGENT *agent_2;
SITE *site;
{
    /*
     * Perform two-point crossover on the two given agents. We choose two chromosomes 
     * to cut inside. Then in each agent we choose two locations to cut at. I don't
     * attempt to make the locations the same as, in general, the chromosomes will
     * have different lengths anyway.
     *
     * locus_* variables are named so that the first digit corresponds
     * to the agent in question and the second digit corresponds to the cut locus.
     * Thus each agent has two cut points.
     *
     * There are 4 regions on the genome, these are (in order)
     * chromosomes, trading resource, uptake mask and migration bits.
     * The cut points can fall in any of these regions, and so we have
     * ten cases:
     *
     *  1) IN_CHROMOSOMES(chrom_1)      && IN_CHROMOSOMES(chrom_2)
     *  2) IN_CHROMOSOMES(chrom_1)      && IN_TRADING_RESOURCE(chrom_2)
     *  3) IN_CHROMOSOMES(chrom_1)      && IN_UPTAKE_MASK(chrom_2)
     *  4) IN_CHROMOSOMES(chrom_1)      && IN_MIGRATION_BITS(chrom_2)
     *
     *  5) IN_TRADING_RESOURCE(chrom_1) && IN_TRADING_RESOURCE(chrom_2)
     *  6) IN_TRADING_RESOURCE(chrom_1) && IN_UPTAKE_MASK(chrom_2)
     *  7) IN_TRADING_RESOURCE(chrom_1) && IN_MIGRATION_BITS(chrom_2)
     *
     *  8) IN_UPTAKE_MASK(chrom_1)      && IN_UPTAKE_MASK(chrom_2)
     *  9) IN_UPTAKE_MASK(chrom_1)      && IN_MIGRATION_BITS(chrom_2)
     *
     * 10) IN_MIGRATION_BITS(chrom_1)   && IN_MIGRATION_BITS(chrom_2)
     *
     *
     * These are all accounted for with a series of ugly if...else steps.
     *
     */

    int chrom_1;
    int chrom_2;
    register int i;

    /* We add 3 on here to handle the trading resource, uptake mask and migration bits. */
    chrom_1 = uniform(NCHROMOSOMES + 3);
    chrom_2 = uniform(NCHROMOSOMES + 3);

    swap_ints_if_gt(chrom_1, chrom_2);

    if DEBUG("X"){
	make_agent_genome(agent_1);
	make_agent_genome(agent_2);
	printf("CROSSOVER between %s and %s\n", AGENT_GENOME(agent_1), AGENT_GENOME(agent_2));
	printf("Chromosomes chosen for crossover are %d and %d\n", chrom_1, chrom_2);
    }

    /* -------------------------------------- CASE 1 ----------------------------------------- */
    if (IN_CHROMOSOMES(chrom_1) && IN_CHROMOSOMES(chrom_2)){
	/*
	 * Both chromosomes are in the "normal" chromosome area. I.e. they are
	 * not trading resources or uptake masks.
	 */

	int locus_1_1 = uniform( CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_1, chrom_1) ) );
	int locus_1_2 = uniform( CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_1, chrom_2) ) );
	int locus_2_1 = uniform( CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_2, chrom_1) ) );
	int locus_2_2 = uniform( CHROMOSOME_LEN( AGENT_CHROMOSOME(agent_2, chrom_2) ) );
	
	/* Swap the chromosomes before the first cut. */
	for (i = 0; i < chrom_1; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}
	
	/* Swap the chromosomes after the second cut. */
	for (i = chrom_2 + 1; i < NCHROMOSOMES; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}
	
	/* Swap the chromosomes between the cuts. */
	for (i = chrom_1 + 1; i < chrom_2; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}
	
	/*
	 * We have to check to see if the crossover points fell in the same chromosome
	 * and treat that case differently.
	 */
	
	if (chrom_1 == chrom_2){
	    /* These are the lengths of the new chromosomes. */
	    int length_1;
	    int length_2;
	    char *new_1;
	    char *new_2;
	    
	    /* Now we need to make sure that the actual loci are in order. */
	    
	    swap_ints_if_gt(locus_1_1, locus_1_2);
	    swap_ints_if_gt(locus_2_1, locus_2_2);

	    if DEBUG("X"){
		printf("The cut loci in chromosome %d are %d and %d in parent_1\n", chrom_1, locus_1_1, locus_1_2);
		printf("The cut loci in chromosome %d are %d and %d in parent_2\n", chrom_1, locus_2_1, locus_2_2);
	    }
	    
	    length_1 = locus_1_1 + (locus_2_2 - locus_2_1) + (CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_1)) - locus_1_2);
	    length_2 = locus_2_1 + (locus_1_2 - locus_1_1) + (CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_1)) - locus_2_2);
	    
	    new_1 = Malloc(length_1 + 1);
	    new_2 = Malloc(length_2 + 1);
	    
	    new_1[0] = '\0';
	    new_2[0] = '\0';
	    
	    strncat(new_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_1)), locus_1_1);
	    strncat(new_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_1)) + locus_2_1, locus_2_2 - locus_2_1);
	    strncat(new_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_1)) + locus_1_2,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_1)) - locus_1_2);
	    
	    strncat(new_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_1)), locus_2_1);
	    strncat(new_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_1)) + locus_1_1, locus_1_2 - locus_1_1);
	    strncat(new_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_1)) + locus_2_2,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_1)) - locus_2_2);
	    
	    set_chromosome(AGENT_CHROMOSOME(agent_1, chrom_1), new_1);
	    set_chromosome(AGENT_CHROMOSOME(agent_2, chrom_1), new_2);
	    
	    Free(new_1);
	    Free(new_2);
	}
	else {
	    /*
	     * No resulting chromosome can be longer than the following. We could
	     * do more work and figure out the actual lengths needed, but that is
	     * a waste of computation time since we have the upper bounds.
	     *
	     * We need to do these separately, since updating the chromosomes
	     * as we go will not allow us to get the right values for other
	     * things later on.
	     */
	    
	    char *new_1_1 = Malloc(CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_1)) +
				   CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_1)) + 1);
	    char *new_1_2 = Malloc(CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_2)) +
				   CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_2)) + 1);
	    char *new_2_1 = Malloc(CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_1)) +
				   CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_1)) + 1);
	    char *new_2_2 = Malloc(CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_2)) +
				   CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_2)) + 1);
	    if DEBUG("X"){
		printf("The cut loci in chromosome %d are %d in parent_1 and %d in parent_2\n", chrom_1, locus_1_1, locus_2_1);
		printf("The cut loci in chromosome %d are %d in parent_1 and %d in parent_2\n", chrom_2, locus_1_2, locus_2_2);
	    }
	    
	    new_1_1[0] = '\0';
	    new_1_2[0] = '\0';
	    new_2_1[0] = '\0';
	    new_2_2[0] = '\0';
	    
	    strncat(new_1_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_1)), locus_1_1);
	    strncat(new_1_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_1)) + locus_2_1,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_2, chrom_1)) - locus_2_1);
	    
	    strncat(new_1_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_2)), locus_2_2);
	    strncat(new_1_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_2)) + locus_1_2,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_2)) - locus_1_2);
	    
	    strncat(new_2_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_1)), locus_2_1);
	    strncat(new_2_1, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_1)) + locus_1_1,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_1)) - locus_1_1);
	    
	    strncat(new_2_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_1, chrom_2)), locus_1_2);
	    strncat(new_2_2, CHROMOSOME_STR(AGENT_CHROMOSOME(agent_2, chrom_2)) + locus_2_2,
		    CHROMOSOME_LEN(AGENT_CHROMOSOME(agent_1, chrom_2)) - locus_2_2);
	    
	    set_chromosome(AGENT_CHROMOSOME(agent_1, chrom_1), new_1_1);
	    set_chromosome(AGENT_CHROMOSOME(agent_1, chrom_2), new_1_2);
	    set_chromosome(AGENT_CHROMOSOME(agent_2, chrom_1), new_2_1);
	    set_chromosome(AGENT_CHROMOSOME(agent_2, chrom_2), new_2_2);
	    
	    Free(new_1_1);
	    Free(new_1_2);
	    Free(new_2_1);
	    Free(new_2_2);
	}
    }
    /* -------------------------------------- CASE 2 ----------------------------------------- */
    else if (IN_CHROMOSOMES(chrom_1) && IN_TRADING_RESOURCE(chrom_2)){

	if DEBUG("X"){
	    printf("Two point crossover between chromosome %d and the trading resource.\n", chrom_1);
	}
	    
	/* Start the crossover in chrom_1. */
	one_point_crossover(agent_1, agent_2, chrom_1);

	/* Swap the chromosomes between the cuts. */
	for (i = chrom_1 + 1; i < NCHROMOSOMES; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}

	swap_trading_resource(agent_1, agent_2);
    }
    /* -------------------------------------- CASE 3 ----------------------------------------- */
    else if (IN_CHROMOSOMES(chrom_1) && IN_UPTAKE_MASK(chrom_2)){

	int locus = uniform(nresources);
	
	if DEBUG("X"){
	    printf("Two point crossover between chromosome %d and locus %d in the uptake mask.\n", chrom_1, locus);
	}

	/* Start the crossover in chrom_1. */
	one_point_crossover(agent_1, agent_2, chrom_1);

	/* Swap the chromosomes between the cuts. */
	for (i = chrom_1 + 1; i < NCHROMOSOMES; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}

	swap_trading_resource(agent_1, agent_2);

	/* Swap part of the uptake mask. */
	swap_uptake_mask(agent_1, agent_2, 0, locus);
    }
    /* -------------------------------------- CASE 4 ----------------------------------------- */
    else if (IN_CHROMOSOMES(chrom_1) && IN_MIGRATION_BITS(chrom_2)){

	int locus = uniform(NMIGRATION_BITS);

	if DEBUG("X"){
	    printf("Two point crossover between chromosome %d and locus %d in migration bits.\n", chrom_1, locus);
	}

	/* Start the crossover in chrom_1. */
	one_point_crossover(agent_1, agent_2, chrom_1);

	/* Swap the chromosomes between the cuts. */
	for (i = chrom_1 + 1; i < NCHROMOSOMES; i++){
	    swap_chromosomes(AGENT_CHROMOSOME(agent_1, i), AGENT_CHROMOSOME(agent_2, i));
	}

	swap_trading_resource(agent_1, agent_2);

	/* Swap the entire uptake mask. */
	swap_uptake_mask(agent_1, agent_2, 0, nresources);

	/* Swap some number of the migration bits. */
	swap_migration_bits(agent_1, agent_2, 0, locus);
    }
    /* -------------------------------------- CASE 5 ----------------------------------------- */
    else if (IN_TRADING_RESOURCE(chrom_1) && IN_TRADING_RESOURCE(chrom_2)){
	if DEBUG("X"){
	    printf("Two point crossover, both points fall in the trading resource.\n");
	}

	swap_trading_resource(agent_1, agent_2);
    }
    /* -------------------------------------- CASE 6 ----------------------------------------- */
    else if (IN_TRADING_RESOURCE(chrom_1) && IN_UPTAKE_MASK(chrom_2)){
	int locus = uniform(nresources);

	if DEBUG("X"){
	    printf("Two point crossover from trading resource to point %d in uptake mask.\n", locus);
	}

	swap_trading_resource(agent_1, agent_2);
	swap_uptake_mask(agent_1, agent_2, 0, locus);
    }
    /* -------------------------------------- CASE 7 ----------------------------------------- */
    else if (IN_TRADING_RESOURCE(chrom_1) && IN_MIGRATION_BITS(chrom_2)){
	int locus = uniform(NMIGRATION_BITS);

	if DEBUG("X"){
	    printf("Two point crossover from trading resource to point %d in uptake mask.\n", locus);
	}

	swap_trading_resource(agent_1, agent_2);
	swap_uptake_mask(agent_1, agent_2, 0, nresources);
	swap_migration_bits(agent_1, agent_2, 0, locus);
    }
    /* -------------------------------------- CASE 8 ----------------------------------------- */
    else if (IN_UPTAKE_MASK(chrom_1) && IN_UPTAKE_MASK(chrom_2)){
	int locus_1;
	int locus_2 ;

	if (nresources == 0){
	}

	if (nresources == 1){
	    locus_1 = 0;
	    locus_2 = 1;
	}
	else {
	    do {
		locus_1 = uniform(nresources);
		locus_2 = uniform(nresources);
	    } while (locus_1 == locus_2);

	    swap_ints_if_gt(locus_1, locus_2);
	}
	
	if DEBUG("X"){
	    printf("Two point crossover in uptake mask at loci %d and %d \n", locus_1, locus_2);
	}

	swap_uptake_mask(agent_1, agent_2, locus_1, locus_2);
    }
    /* -------------------------------------- CASE 9 ----------------------------------------- */
    else if (IN_UPTAKE_MASK(chrom_1) && IN_MIGRATION_BITS(chrom_2)){
	int locus = uniform(nresources);

	swap_uptake_mask(agent_1, agent_2, locus, nresources);

	locus = uniform(NMIGRATION_BITS);

	swap_migration_bits(agent_1, agent_2, 0, locus);
    }
    /* -------------------------------------- CASE 10 ---------------------------------------- */
    else { /* (IN_MIGRATION_BITS(chrom_1) && IN_MIGRATION_BITS(chrom_2) */

	int locus_1;
	int locus_2;

	if (NMIGRATION_BITS == 0){
	    error("NMIGRATION_BITS is set to 0! In agent_crossover().");
	}
	    
	if (NMIGRATION_BITS == 1){
	    locus_1 = 0;
	    locus_2 = 1;
	}
	else {
	    int locus_1;
	    int locus_2;

	    do {
		locus_1 = uniform(NMIGRATION_BITS);
		locus_2 = uniform(NMIGRATION_BITS);
	    } while (locus_1 == locus_2);

	    swap_ints_if_gt(locus_1, locus_2);
	}

	if DEBUG("X"){
	    printf("Two point crossover in migration bits at loci %d and %d \n", locus_1, locus_2);
	}

	swap_migration_bits(agent_1, agent_2, locus_1, locus_2);
    }


    /* Work out the new makeup of each agent. */
    calculate_makeup(agent_1);
    calculate_makeup(agent_2);

    /* Reconstruct the genome from the new chromosomes. */
    make_agent_genome(agent_1);
    make_agent_genome(agent_2);

    /* Mark these agents as newborn. */    
    AGENT_NEWBORN(agent_1) = TRUE;
    AGENT_NEWBORN(agent_2) = TRUE;

    /* Share their resources. */
    add_resources(AGENT_RESERVE_LIST(agent_1), AGENT_RESERVE_LIST(agent_2));

    for (i = 0; i < nresources; i++){
	int half = AGENT_RESERVE(agent_1, i) >> 1;

	/* If there are an odd number, give the extra one away randomly. */
	if (AGENT_RESERVE(agent_1, i) % 2 && uniform(2)){
	    half++;
	}

	AGENT_RESERVE(agent_2, i) = half;
    }

    sub_resources(AGENT_RESERVE_LIST(agent_1), AGENT_RESERVE_LIST(agent_2));

    
    if DEBUG("X"){
	printf("AFTER CROSSOVER, agents are %s and %s\n", AGENT_GENOME(agent_1), AGENT_GENOME(agent_2));
    }
    
    return;
}

#undef swap_ints_if_gt

#endif /* NO_SEX */

int
agent_distance(agent1, agent2)
AGENT *agent1;
AGENT *agent2;
{
    register int i;
    register int distance = 0;

    for (i = 0; i < NCHROMOSOMES; i++){
	distance += chromosome_distance(AGENT_CHROMOSOME(agent1, i), AGENT_CHROMOSOME(agent2, i));
    } 
    
    for (i = 0; i < nresources; i++){
	if (AGENT_UPTAKE(agent1, i) != AGENT_UPTAKE(agent2, i)){
	    distance++;
	}
    }
    
    if (AGENT_TRADING_RESOURCE(agent1) != AGENT_TRADING_RESOURCE(agent2)){
	distance++;
    }

    return distance;
}
