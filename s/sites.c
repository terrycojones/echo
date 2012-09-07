#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/winnetou/disks/disk3/Users/terry/r/echo/s/RCS/sites.c,v 1.13 1995/04/19 02:33:43 terry Exp terry $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "types.h"
#include "sites.h"
#include "agents.h"
#include "worlds.h"
#include "chromosomes.h"
#include "graphics.h"
#include "resources.h"
#include "interaction.h"
#include "free.h"
#include "syscalls.h"
#include "random.h"
#include "globals.h"
#include "defaults.h"
#include "regexp.h"

#define BUF_SZ 8092

SITE *
new_site(nagents)
AGENT_POPULATION_SIZE nagents;
{
    /*
     * Create a new site with room for nagents agents. Note that this
     * is NOT the number of agents actually at the site. There are zero
     * here until someone else puts them into the agents array.
     */

    SITE *new;
    register int i;

	rcsid = rcsid;
    
    if (nagents == (AGENT_POPULATION_SIZE)0){
	nagents = DEF_SITE_NAGENTS;
    }
#ifdef CHECK_ARGS
    else {
	if (nagents < (AGENT_POPULATION_SIZE)0){
	    error("new_site() called with negative nagents.");
	}
    }
#endif /* CHECK_ARGS */
    
    new = (SITE *)Malloc(sizeof(SITE));
    
    SITE_AGENTS(new) = (AGENT **)Malloc(nagents * sizeof(AGENT *));
    SITE_NAGENTS(new) = (AGENT_POPULATION_SIZE) 0;
    SITE_MAX_AGENTS(new) = nagents;
    SITE_P_MUTATION(new) = DEFAULT_MUTATION_P;
    SITE_P_CROSSOVER(new) = DEFAULT_CROSSOVER_P;
    SITE_P_RANDOM_DEATH(new) = DEFAULT_RANDOM_DEATH_P;
    
    for (i = 0; i < nagents; i++){
	SITE_AGENT(new, i) = (AGENT *)0;
    }

    SITE_PRODUCE_LIST(new) = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    SITE_MAXIMUM_LIST(new) = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    SITE_RESOURCE_LIST(new) = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    SITE_MAINTENANCE_LIST(new) = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));

    zero_resources(SITE_PRODUCE_LIST(new));
    zero_resources(SITE_MAXIMUM_LIST(new));
    zero_resources(SITE_RESOURCE_LIST(new));
    zero_resources(SITE_MAINTENANCE_LIST(new));

    return new;
}

SITE_EDIT *
new_site_edit()
{
    SITE_EDIT *new;
    
    new = (SITE_EDIT *)Malloc(sizeof(SITE_EDIT));
    
    SITE_EDIT_NAME(new) = "";
    SITE_EDIT_AGENTS(new) = "";
    SITE_EDIT_P_MUTATION(new) = "";
    SITE_EDIT_P_CROSSOVER(new) = "";
    SITE_EDIT_P_RANDOM_DEATH(new) = "";
    
    SITE_EDIT_PRODUCE_LIST(new) = "";
    SITE_EDIT_MAXIMUM_LIST(new) = "";
    SITE_EDIT_RESOURCE_LIST(new) = "";
    SITE_EDIT_MAINTENANCE_LIST(new) = "";

    return new;
}


void
grow_site(site, increment)
SITE *site;
AGENT_POPULATION_SIZE increment;
{
    /*
     * Add to the length of a site. This is a bit ugly, I
     * hate reallocing and I hope we won't have to do it too often.
     * If we do, we can increase the default sizes.
     */

    register AGENT_POPULATION_SIZE i;
    register AGENT_POPULATION_SIZE new_max_agents;
    AGENT **new_agents;
    
    if (increment == (AGENT_POPULATION_SIZE)0){
	increment = DEF_SITE_GROWTH_INC;
    }
#ifdef CHECK_ARGS
    else {
	if (increment < (AGENT_POPULATION_SIZE)0){
	    error("grow_site() called with negative increment.");
	}
    }
    
    if (!site){
	error("grow_site() called with NULL site!");
    }
#endif /* CHECK_ARGS */

    new_max_agents = SITE_MAX_AGENTS(site) + increment;
    new_agents = (AGENT **)Malloc(new_max_agents * sizeof(AGENT *));
    
    /* printf("site (%d, %d) grows by %d to size %d\n", SITE_ROW(site), SITE_COL(site), increment, new_max_agents); */

    /* Copy the old pointers. */
    for (i = 0; i < SITE_NAGENTS(site); i++){
	new_agents[i] = SITE_AGENT(site, i);
    }
    
    /* Set the new ones to zippo. */
    for (; i < new_max_agents; i++){
	new_agents[i] = (AGENT *)0;
    }
    
    Free(SITE_AGENTS(site));
    SITE_AGENTS(site) = new_agents;
    SITE_MAX_AGENTS(site) = new_max_agents;
    
    return;
}

void
site_produce(site)
SITE *site;
{
    /*
     * Add resources to a site according to what it produces each
     * generation. Do not exceed site maximums.
     */
    
    register int i;
    
#ifdef CHECK_ARGS
    if (!site){
	error("site_produce() called with NULL site!");
    }
#endif /* CHECK_ARGS */
    
    add_resources(SITE_RESOURCE_LIST(site), SITE_PRODUCE_LIST(site));
    
    for (i = 0; i < nresources; i++){
	if (SITE_RESOURCE(site, i) > SITE_MAXIMUM(site, i)){
	    SITE_RESOURCE(site, i) = SITE_MAXIMUM(site, i);
	}
    } 
    
    return;
}

void
site_charge_maintenance(site)
SITE *site;
{
    /*
     * Charge a maintenance fee to all agents at a site.
     * If they can't pay the price, delete them. The levy is imposed
     * probabilistically.
     */
    
    AGENT_POPULATION_SIZE agent;
    PROBABILITY p_maintenance = WORLD_P_MAINTENANCE(the_world);
    
#ifdef CHECK_ARGS
    if (!site){
	error("site_charge_maintenance() called with NULL site!");
    }
#endif /* CHECK_ARGS */
    
    for (agent = SITE_NAGENTS(site) - 1; agent >= 0; agent--){
	if (biased_coin_flip(p_maintenance)){
	    if (can_pay_maintenance(SITE_MAINTENANCE_LIST(site), SITE_AGENT(site, agent))){
		sub_resources(AGENT_RESERVE_LIST(SITE_AGENT(site, agent)), SITE_MAINTENANCE_LIST(site));
		if DEBUG("x") printf("%s%03d at (%d, %d) was taxed.\n", AGENT_NAME( SITE_AGENT(site, agent)), 
		       agent_instance(SITE_AGENT(site, agent)), SITE_ROW(site), SITE_COL(site));
	    }
	    else {
		if DEBUG("xbD") {
		    printf("%s%03d at (%d, %d) is bankrupt.", AGENT_NAME( SITE_AGENT(site, agent)), 
			   agent_instance(SITE_AGENT(site, agent)), SITE_ROW(site), SITE_COL(site));
		    print_terse_resource_list(AGENT_MAKEUP_LIST( SITE_AGENT(site, agent) ), FALSE, " (", " ", "+", stdout);
		    putc('+', stdout);
		    print_terse_resource_list(AGENT_RESERVE_LIST( SITE_AGENT(site, agent) ), FALSE, " ", " ", "+", stdout);


		    printf("= %d resources %s).\n",
			   total_resources( AGENT_RESERVE_LIST (SITE_AGENT(site, agent)) ) + 
			   total_resources( AGENT_MAKEUP_LIST  (SITE_AGENT(site, agent)) ),
#ifdef BANKRUPTCY_DELETES_RESOURCES
			   "vanish"
#else
			   "return to the environment"
#endif
			   );
	    }


#ifdef BANKRUPTCY_DELETES_RESOURCES
		site_delete_agent(site, agent, TRUE, FALSE);
#else
		site_delete_agent(site, agent, TRUE, TRUE);
#endif
	    }
	}
    } 
    
    return;
}

void
site_reset_gain_flags(site)
SITE *site;
{
    register AGENT_POPULATION_SIZE agent;
    register AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);    
    register AGENT                 **agents = SITE_AGENTS(site);
    
    for (agent = 0; agent < nagents; agent++){
	AGENT_GAINED(agents[agent]) = FALSE;
    }

    return;
}

void
site_distribute_resources(site)
SITE *site;
{
    /*
     * Distribute the resources at this site out to the agents who are here and who 
     * can take up the resources (as determined by their uptake masks. We give
     * each of the agents who can take up a particular resource an equal share
     * of the available quantity of that resource. If there are some resources
     * left over, we give them out, one by one, to random agents at the site.
     * This is not as easy as it sounds, we have to make sure we give the remainder
     * only to those who can take it.
     *
     * We also set up the gained_resources flag for each agent that gains.
     */

    register NRESOURCES            resource;
    register AGENT_POPULATION_SIZE agent;
    register AGENT_POPULATION_SIZE agents_at_site;
    register AGENT                 **agents;
    RESOURCE_SIZE                  shares[MAX_RESOURCES];
    AGENT_POPULATION_SIZE          ntakers[MAX_RESOURCES];
    static AGENT_POPULATION_SIZE   *takers[MAX_RESOURCES];
    static AGENT_POPULATION_SIZE   *space = (AGENT_POPULATION_SIZE *)0;
    static AGENT_POPULATION_SIZE   max_site_pop_so_far = (AGENT_POPULATION_SIZE)0;

#ifdef CHECK_ARGS
    if (!site){
	error("site_produce() called with NULL site!");
    }
#endif /* CHECK_ARGS */


    agents = SITE_AGENTS(site);
    agents_at_site = SITE_NAGENTS(site);
    
    /*
     * This is hacked for speed. We need to keep track of which
     * agents can take up which resource. This would call for a 2D array
     * of size nresources x agents_at_site, but this size is dynamic.
     * Instead, I set up a 1D array and use takers to index into it at
     * the right locations (The 1D array means only 1 call to malloc).
     *
     * Further, we use the array from last time unless we see more
     * agents at this site than at any previous time. So hopefully this
     * wont cost us too dreadfully, as we'll increase a few times and then
     * stay the same for a long time. If that makes sense.
     */

    if (agents_at_site > max_site_pop_so_far){
	max_site_pop_so_far = agents_at_site;
	if (space){
	    Free(space);
	}
	
	space = (AGENT_POPULATION_SIZE *) Malloc(nresources * agents_at_site * sizeof(AGENT_POPULATION_SIZE));
	
	for (resource = 0; resource < nresources; resource++){
	    takers[resource] = space + (resource * agents_at_site);
	} 
    }

    /*
     * Now find out which agents want what. Note that we may
     * fill the takers arrays, but we will not exceed them.
     * It will be filled iff every agent wants every resource
     * and this is the biggest population we have seen yet.
     *
     * The number of takers of each resource is recorded in ntakers.
     */
    for (resource = 0; resource < nresources; resource++){
	register int pos = 0;

	ntakers[resource] = (AGENT_POPULATION_SIZE)0;
	
	for (agent = 0; agent < agents_at_site; agent++){
	    if (AGENT_UPTAKE(agents[agent], resource) == TRUE){
		ntakers[resource]++;
		takers[resource][pos++] = agent;
	    }
	}
    } 
    
    /* Figure out how much each agent that wants the resource will get. Integers! */
    for (resource = 0; resource < nresources; resource++){
	if (ntakers[resource]){
	    shares[resource] = SITE_RESOURCE(site, resource) / ntakers[resource];
	}
	else {
	    shares[resource] = 0;
	}
    } 
    
    /*
     * Run through the agents and give them the resources they all get.
     * Subtract these resources from the sites and mark the agents as
     * having gained.
     */
    for (resource = 0; resource < nresources; resource++){
	if (shares[resource]){
	    for (agent = 0; agent < ntakers[resource]; agent++){
		register AGENT_POPULATION_SIZE who = takers[resource][agent];
		AGENT_RESERVE(agents[who], resource) += shares[resource];
		AGENT_GAINED(agents[who]) = TRUE;
	    } 
	    SITE_RESOURCE(site, resource) -= ntakers[resource] * shares[resource];
	}
    } 
    
    /* Now we give the remainders to random agents. */
    for (resource = 0; resource < nresources; resource++){
	
	register AGENT_POPULATION_SIZE remainder = SITE_RESOURCE(site, resource);
	
	if (ntakers[resource] && remainder){
	    register int i;
	    
	    for (i = 0; i < remainder; i++){
		/* Find someone who can take this resource. */
		register AGENT_POPULATION_SIZE who =
		    (AGENT_POPULATION_SIZE) takers[resource][uniform(ntakers[resource])];

		/* Sanity check. */
		if (AGENT_UPTAKE(agents[who], resource) == FALSE){
		    error("Agent at site is picked to receive resource it cannot pick up!");
		}
		
		AGENT_RESERVE(agents[who], resource)++;
		/* We still need to update this; the share for this resource may have been 0. */
		AGENT_GAINED(agents[who]) = TRUE;
	    } 
	    SITE_RESOURCE(site, resource) = (RESOURCE_SIZE)0;
	}
    } 

    return;
}

#ifndef NO_ZAP
void
site_kill_agents_without_cause(site, prob)
SITE *site;
PROBABILITY prob;
{
    /*
     * Run through all the agents at this site and kill them with
     * probability prob.
     */

    register AGENT_POPULATION_SIZE agent;

#ifdef CHECK_ARGS
    if (!site){
	error("kill_agents_without_cause() called with NULL site!");
    }
#endif /* CHECK_ARGS */

    for (agent = 0; agent < SITE_NAGENTS(site); agent++){
	if (biased_coin_flip(prob)){
	    
	    if DEBUG("zD") printf("ZAP! agent %s at site (%d, %d) killed for no reason!\n", 
			      AGENT_NAME( SITE_AGENT(site, agent)), SITE_ROW(site), SITE_COL(site));

#ifdef ZAPPING_DELETES_RESOURCES
	    site_delete_agent(site, agent, TRUE, FALSE);
#else
	    site_delete_agent(site, agent, TRUE, TRUE);
#endif
	    
	    /*
	     * This horribleness is here because site_delete_agent() will remove the
	     * agent at location 'agent', and replace it with another agent. Therefore
	     * we want to consider this location again, so we decrement the loop index.
	     * Not nice, but it works :-) Also note that the loop termination condition
	     * will change as agents are deleted.
	     */
	    agent--;
	}
    } 
    
    return;
}
#endif

SITE *
read_site_from_file(dir, file)
char *dir;
char *file;
{
    /*
     * Read a site from a file, create a new site data structure
     * and fill in the fields. 'dir' may be empty, in which case we
     * just open 'file'. If not, we concatenate the two and open the
     * result.
     */
     
    SITE *new;
    SITE_EDIT *edit;
    AGENT_POPULATION_SIZE nagents;
    AGENT_POPULATION_SIZE agent;
    char *agents;
    int len;

#ifdef CHECK_ARGS
    if (!file){
	error("read_site_from_file() called with NULL file argument.");
    }
#endif

    edit = read_site_edit_from_file(dir, file);
    
    if (!edit){
	return (SITE *) 0;
    }
    
    agents = SITE_EDIT_AGENTS(edit);
    
    /* Zap trailing space and newlines in the agents. */
    len = strlen(agents);

    len--;
    
    while (len >= 0 && (agents[len] == ' ' || agents[len] == '\t' || agents[len] == '\n')){
	agents[len] = '\0';
	len--;
    }
    
    if (!len){
	/* No agents at this site. This is legal enough. */
	nagents = 0;
    }
    else {
	if (count_agents(SITE_EDIT_AGENTS(edit), &nagents) == FALSE){
	    free_site_edit(edit);
	    return (SITE *) 0;
	}
    }

    /* Waste some space to make things faster if the # of agents at the site grows. */
    new = new_site(nagents * DEF_SITE_NAGENTS_MULTIPLIER);
    
    SITE_NAME(new) = strdup( SITE_EDIT_NAME(edit) );
    SITE_NAGENTS(new) = nagents;
    
    /* Get and check the mutation rate. */
    SITE_P_MUTATION(new) = atof( SITE_EDIT_P_MUTATION(edit) );
    
    if (SITE_P_MUTATION(new) < 0.0 || SITE_P_MUTATION(new) > 1.0){
	popup_message("the mutation probability must be in the range 0.0 to 1.0\n(it is given as %f in '%s').", 
		      SITE_P_MUTATION(edit), file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the crossover rate. */
    SITE_P_CROSSOVER(new) = atof( SITE_EDIT_P_CROSSOVER(edit) );
    
    if (SITE_P_CROSSOVER(new) < 0.0 || SITE_P_CROSSOVER(new) > 1.0){
	popup_message("the crossover probability must be in the range 0.0 to 1.0\n(it is given as %f in '%s').", 
		      SITE_P_CROSSOVER(edit), file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the random death rate. */
    SITE_P_RANDOM_DEATH(new) = atof( SITE_EDIT_P_RANDOM_DEATH(edit) );
    
    if (SITE_P_RANDOM_DEATH(new) < 0.0 || SITE_P_RANDOM_DEATH(new) > 1.0){
	popup_message("the random death probability must be in the range 0.0 to 1.0\n(it is given as %f in '%s').", 
		      SITE_P_RANDOM_DEATH(edit), file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the production function. */
    if (str_to_resource_list(SITE_EDIT_PRODUCE_LIST(edit), SITE_PRODUCE_LIST(new), nresources) == FALSE){
	popup_message("could not convert site produce list into\n%d integers in file '%s'.", nresources, file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the maximum resource levels. */
    if (str_to_resource_list(SITE_EDIT_MAXIMUM_LIST(edit), SITE_MAXIMUM_LIST(new), nresources) == FALSE){
	popup_message("could not convert site maximum list into\n%d integers in file '%s'.", nresources, file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the maintenance charges. */
    if (str_to_resource_list(SITE_EDIT_MAINTENANCE_LIST(edit), SITE_MAINTENANCE_LIST(new), nresources) == FALSE){
	popup_message("could not convert site maintenance list into\n%d integers in file '%s'.", nresources, file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }

    /* Get and check the initial resource levels. */
    if (str_to_resource_list(SITE_EDIT_RESOURCE_LIST(edit), SITE_RESOURCE_LIST(new), nresources) == FALSE){
	popup_message("could not convert site resource list into\n%d integers in file '%s'.", nresources, file);
	free_site_edit(edit);
	free_site(new);
	return (SITE *) 0;
    }
    
    /*
     * Now convert the agents that are given as a string into the agents list
     * in the new site...
     */
    
    for (agent = 0; agent < nagents; agent++){
	char *newline = index(agents, '\n');
	char *count;

	if (newline){
	    *newline = '\0';
	}
	
	skip_white(agents);
	count = strpbrk(agents, " \t");
	
	if (!count){
	    /* No spaces, therefore no count. */
	    SITE_AGENT(new, agent) = read_agent_from_file("agents", agents);
	    
	    if (!SITE_AGENT(new, agent)){
		free_site_edit(edit);
		free_site(new);
		return (SITE *) 0;
	    }

	    agents = newline + 1;
	    continue;
	}
	
	*count = '\0';
	count++;
	skip_white(count);

	if (count == newline){
	    /* No count, there was just some whitespace after the agent name. */
	    SITE_AGENT(new, agent) = read_agent_from_file("agents", agents);
	    
	    if (!SITE_AGENT(new, agent)){
		free_site_edit(edit);
		free_site(new);
		return (SITE *) 0;
	    }
	}
	else {
	    AGENT_POPULATION_SIZE n;
	    AGENT_POPULATION_SIZE i;

	    SITE_AGENT(new, agent) = read_agent_from_file("agents", agents);
	    
	    if (!SITE_AGENT(new, agent)){
		free_site_edit(edit);
		free_site(new);
		return (SITE *) 0;
	    }
	    
	    if (!isdigit(*count)){
		popup_message("expected a digit following the agent name\nin read_site_from_file() (saw '%s').", count);
	    }
	    
	    n = (AGENT_POPULATION_SIZE) atoi(count);
	    
	    if (n <= 0){
		error("the site in '%s' specifies\nan agent (%s) with illegal count %d.", file, agents, n);
		free_site_edit(edit);
		free_site(new);
		return (SITE *) 0;
	    }
	    
	    n--;

	    for (i = 0; i < n; i++){
		agent++;
		SITE_AGENT(new, agent) = copy_agent(SITE_AGENT(new, agent - 1));
	    } 
	}

	agents = newline + 1;
    }

    free_site_edit(edit);
    return new;
}

BOOLEAN
str_to_resource_list(string, i, n)
char *string;
RESOURCE_LIST i;
int n;
{
    /*
     * The string 'string' contains integers separated by whitespace. We read them
     * and put them into the array i. There must be at least n of them.
     * Make a copy of the string and then free it since strtok is going
     * to stomp on stuff that might be wanted by the caller.
     */

    int count = 0;
    char *next_int;
    char *s;
    
    if (!n){
	return FALSE;
    }
    
    s = strdup(string);

    next_int = strtok(s, " \t");

    do {
	if (!next_int){
	    return FALSE;
	}
	
	i[count] = (RESOURCE_SIZE) atoi(next_int);
	next_int = strtok(NULL, " \t");
	count++;
    } while (count < n);
    
    Free(s);
    return TRUE;
}

BOOLEAN
count_agents(agents, count)
char *agents;
AGENT_POPULATION_SIZE *count;
{
    /*
     * Count the number of agents in 'sgents'. The agent names will be
     * separated by a newline, but each may have a count following it.
     * This must not destroy sgents, so we make a copy of it.
     *
     * For example, the string may look like
     *
     * "agent_a 3 \n agent_b \n agent_c 17 \n agent_d \n agent_e"
     *
     * in which case we return 3 + 1 + 17 + 1 + 1 = 23.
     *
     * We first zap trailing whitespace and newlines so that we can
     * end properly.
     *
     * Return TRUE if all goes well.
     */
     
    char *s = strdup(agents);
    char *to_free = s;
    BOOLEAN done = FALSE;

    *count = (AGENT_POPULATION_SIZE) 0;

    while (done == FALSE){
	char *newline = index(s, '\n');

	if (!newline){
	    done = TRUE;
	}
	else {
	    *newline = '\0';
	}
	
	skip_white(s);
	skip_to_white(s);

	if (*s == '\0'){
	    /* There was no count, we hit the zapped newline (or the string end). */
	    (*count)++;
	}
	else {
	    skip_white(s);

	    if (!isdigit(*s)){
		popup_message("Expected a digit following the agent name\nin count_agents() (saw '%s').", s);
		return FALSE;
	    }
	    
	    /* There was a count, add it on. */
	    *count += atoi(s);
	}
	
	s = newline + 1;
    }

    Free(to_free);
    return TRUE;
}

SITE_EDIT *
read_site_edit_from_file(dir, file)
char *dir;
char *file;
{
    /*
     * Read a site from a file, create a new site_edit data structure
     * and fill in the fields. 'dir' may be empty, in which case we
     * just open 'file'. If not, we concatenate the two and open the
     * result.
     */

    FILE *fp;
    char *name = file;
    SITE_EDIT *new;
    char line[BUF_SZ];
    int line_num = 0;
    struct stat sbuf;
    
#ifdef CHECK_ARGS
    if (!file){
	error("read_site_edit_from_file() called with NULL file argument.");
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
    
    Stat(name, &sbuf);
    fp = Fopen(name, "r");
    new = new_site_edit();

    while (fgets(line, BUF_SZ, fp)){
	char *variable;
	char *value;
	BOOLEAN recognized = FALSE;

	line_num++;
	
	variable = strtok(line, " :\t\n");
	
	/* Watch for empty or comment lines. */
	if (!variable || *variable == '#'){
	    continue;
	}

	if (!strcmp(AGENTS_STR, variable)){
	    /* Read the rest of the file... */
	    long length = (long) sbuf.st_size - Ftell(fp);
	    int nread;

	    SITE_EDIT_AGENTS(new) = Malloc(length + 1);
	    
	    if ((nread = fread(SITE_EDIT_AGENTS(new), sizeof(char), length, fp)) != length){
		error("fread returns %d bytes (expected %d) at end of '%s' site file.", nread, length, name);
	    }
	    
	    SITE_EDIT_AGENTS(new)[length] = '\0';

	    /* We may as well break as this stuff read to the end of the file. */
	    break;
	}
	
	/* Is this the name? */
	if (!strcmp(variable, NAME_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		error("variable '%s' on line %d of '%s' does not have a value!", variable, line_num, name);
	    }
	
	    SITE_EDIT_NAME(new) = strdup(value);
	    continue;
	}

	/* Is this the mutation probability? */
	if (!strcmp(variable, MUTATION_PROB_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		error("variable '%s' on line %d of '%s' does not have a value!", variable, line_num, name);
	    }
	
	    SITE_EDIT_P_MUTATION(new) = strdup(value);
	    continue;
	}
	
	/* Is this the crossover probability? */
	if (!strcmp(variable, CROSSOVER_PROB_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	
	    SITE_EDIT_P_CROSSOVER(new) = strdup(value);
	    continue;
	}
	
	/* Is this the random death probability? */
	if (!strcmp(variable, RANDOM_DEATH_PROB_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	
	    SITE_EDIT_P_RANDOM_DEATH(new) = strdup(value);
	    continue;
	}
	
	/* Is this the production function? */
	if (!strcmp(variable, PRODUCTION_FUNCTION_STR)){
	    register char *nl;

	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		popup_message("Could not find newline character\non line %d of '%s'!", line_num, name);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	    
	    *nl = '\0';

	    SITE_EDIT_PRODUCE_LIST(new) = strdup(value);
	    continue;
	}
	
	/* Is this the maximums? */
	if (!strcmp(variable, MAXIMUMS_STR)){
	    register char *nl;

	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		popup_message("Could not find newline character\non line %d of '%s'!", line_num, name);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	    
	    *nl = '\0';

	    SITE_EDIT_MAXIMUM_LIST(new) = strdup(value);
	    continue;
	}
	
	/* Is this the maintenance? */
	if (!strcmp(variable, MAINTENANCE_STR)){
	    register char *nl;

	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		popup_message("Could not find newline character\non line %d of '%s'!", line_num, name);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	    
	    *nl = '\0';

	    SITE_EDIT_MAINTENANCE_LIST(new) = strdup(value);
	    continue;
	}
	
	/* Is this the resources? */
	if (!strcmp(variable, SITE_RESOURCES_STR)){
	    register char *nl;

	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		popup_message("Could not find newline character\non line %d of '%s'!", line_num, name);
		free_site_edit(new);
		return (SITE_EDIT *) 0;
	    }
	    
	    *nl = '\0';

	    SITE_RESOURCE_LIST(new) = strdup(value);
	    continue;
	}
	
	if (recognized == FALSE){
	    /* We don't know what it is... */
	    popup_message("Unrecognized line (%d) in\nfile '%s'.", line_num, name);
	    free_site_edit(new);
	    return (SITE_EDIT *) 0;
	}
    }

    if (dir){
	Free(name);
    }

    Fclose(fp);
    return new;
}

void
write_site_to_file(dir, file, site)
char *dir;
char *file;
SITE *site;
{
    FILE *in_fp;
    FILE *out_fp;
    char *name = file;
    char tmp_file[MAXPATHLEN];
    char line[BUF_SZ];
    extern char *mktemp();

#ifdef CHECK_ARGS
    if (!file){
	error("write_site_to_file() called with NULL file argument.");
    }
#endif
    
    sprintf(tmp_file, "/tmp/__echo__XXXXXX");
    mktemp(tmp_file);

    if (!tmp_file){
	error("could not make temporary file in write_site_to_file().");
    }
    
    out_fp = fopen(tmp_file, "w");
    
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
	    char *tmp = line;
	    
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
    
    /* fprintf(out_fp, "%s : %s\n", NAME_STR, SITE_NAME(site) ? SITE_NAME(site) : ""); */

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
write_site_edit_to_file(dir, file, site)
char *dir;
char *file;
SITE_EDIT *site;
{
    FILE *in_fp;
    FILE *out_fp;
    char *name = file;
    register int i; 
    char tmp_file[MAXPATHLEN];
    char line[BUF_SZ];
    extern char *mktemp();

#ifdef CHECK_ARGS
    if (!file){
	error("write_site_to_file() called with NULL file argument.");
    }
#endif
    
    sprintf(tmp_file, "/tmp/__echo__XXXXXX");
    mktemp(tmp_file);

    if (!tmp_file){
	error("could not make temporary file in write_site_to_file().");
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
	    char *tmp = line;
	    
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
    
    fprintf(out_fp, "%s : %s\n", NAME_STR, SITE_EDIT_NAME(site));
    fprintf(out_fp, "%s : %s\n", MUTATION_PROB_STR, SITE_EDIT_P_MUTATION(site));
    fprintf(out_fp, "%s : %s\n", CROSSOVER_PROB_STR, SITE_EDIT_P_CROSSOVER(site));
    fprintf(out_fp, "%s : %s\n", RANDOM_DEATH_PROB_STR, SITE_EDIT_P_RANDOM_DEATH(site));
    fprintf(out_fp, "%s : %s\n", PRODUCTION_FUNCTION_STR, SITE_EDIT_PRODUCE_LIST(site));
    fprintf(out_fp, "%s : %s\n", SITE_RESOURCES_STR, SITE_RESOURCE_LIST(site));
    fprintf(out_fp, "%s : %s\n", MAXIMUMS_STR, SITE_EDIT_MAXIMUM_LIST(site));
    fprintf(out_fp, "%s : %s\n", MAINTENANCE_STR, SITE_EDIT_MAINTENANCE_LIST(site));
    
    /* Zap trailing newlines, if any. */
    i = strlen(SITE_EDIT_AGENTS(site)) - 1;
    
    while (i >= 0 && SITE_EDIT_AGENTS(site)[i] == '\n'){
	SITE_EDIT_AGENTS(site)[i] = '\0';
	i--;
    }

    fprintf(out_fp, "%s : \n%s", AGENTS_STR, SITE_EDIT_AGENTS(site));

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
site_perform_interactions(site)
SITE *site;
{
    /*
     * Call the interaction function as many times as necessary with
     * pairs of agents chosen from this site. The agents must not
     * have been born this generation.
     */
    
    register AGENT_POPULATION_SIZE interaction;
    register AGENT_POPULATION_SIZE interactions;
    register AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    
    if (nagents == (AGENT_POPULATION_SIZE)0){
	return;
    }

    if (nagents == (AGENT_POPULATION_SIZE)1){
	/* Let the only agent interact with itself? */
	interact(0, 0, site, site, WORLD_TRADE_FRACTION(the_world));
	return;
    }
    
    interactions = SITE_INTERACTIONS(site);

    for (interaction = 0; interaction < interactions; interaction++){
	
	AGENT_POPULATION_SIZE agent1;
	AGENT_POPULATION_SIZE agent2;
	
	/* 
	 * Find a non-newborn agent at this site. We can use nagents here without
	 * having to worry that the number of agents at the site may change under us.
	 * Indeed it may, but if this is so, it will just be to add newborn 
	 */
	do {
	    agent1 = (AGENT_POPULATION_SIZE) uniform( nagents );
	} while ( AGENT_NEWBORN( SITE_AGENT(site, agent1) ) == TRUE);
	
	
	/* Find a non-newborn agent at this site that is also not the first agent. */
	do {
	    PROBABILITY d = knuth_random();
	    AGENT_POPULATION_SIZE offset = d * d * d * nagents;

	    if (agent1 + offset < nagents){
		
		/* Can choose above. */
		if (agent1 - offset >= 0){
		    
		    /* Can choose below. */
		    if (random_boolean() == TRUE){
			/* Go above. */
			agent2 = agent1 + offset;
		    }
		    else {
			/* Go below. */
			agent2 = agent1 - offset;
		    }
		}
		else {
		    
		    /* Can't choose below, must go above. */
		    agent2 = agent1 + offset;
		}
	    }
	    else {
		
		/* Can't go above. */
		if (agent1 - offset >= 0){
		    /* Can go below. */
		    agent2 = agent1 - offset;
		}
		else {
		    /* Can't go below either. Make sure the do loop repeats. */
		    agent2 = agent1;
		}
	    }
	    
	} while (agent1 == agent2 || AGENT_NEWBORN( SITE_AGENT(site, agent2) ) == TRUE);
	
	/* Now we have two agents that can interact. */
	site_move_agents_together(agent1, agent2, site);
	interact(agent1, agent2, site, WORLD_TRADE_FRACTION(the_world));
	
	/* The number of agents at a site may change after interact(). */
	nagents = SITE_NAGENTS(site);
    } 
    
    return;
}

void
site_add_agent(agent, site)
AGENT *agent;
SITE *site;
{
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    
    if (nagents == SITE_MAX_AGENTS(site)){
	grow_site(site, (AGENT_POPULATION_SIZE)0);
    }
    
    SITE_AGENT(site, nagents) = agent;
    SITE_NAGENTS(site)++;
    /* I think this is not needed now. Running.c does it. */
    /* SITE_INTERACTIONS(site) = (AGENT_POPULATION_SIZE) (WORLD_C(the_world) * (double) SITE_NAGENTS(site)); */
    
    return;
}

void
site_delete_agent(site, who, free_after_deleting, return_resources_to_environment)
SITE *site;
AGENT_POPULATION_SIZE who;
BOOLEAN free_after_deleting;
BOOLEAN return_resources_to_environment;
{
    /* 
     * Remove the agent indexed by who from this site.
     * Move the following agents down in the stack.
     *
     * If free_after_deleting is TRUE, then reclaim the space allocated to this agent.
     */
     
    AGENT_POPULATION_SIZE agent;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    
#ifdef CHECK_ARGS
    if (!site){
	error("site_delete_agent() called with NULL site!");
    }
    
    if (SITE_AGENT(site, who) == (AGENT *)0){
	error("site_delete_agent() requested to delete already empty site!");
    }
#endif /* CHECK_ARGS */

    if (return_resources_to_environment == TRUE){
	register int i;
	
	add_resources(SITE_RESOURCE_LIST(site), AGENT_MAKEUP_LIST( SITE_AGENT(site, who)));
	add_resources(SITE_RESOURCE_LIST(site), AGENT_RESERVE_LIST( SITE_AGENT(site, who)));
    
	for (i = 0; i < nresources; i++){
	    if (SITE_RESOURCE(site, i) > SITE_MAXIMUM(site, i)){
		SITE_RESOURCE(site, i) = SITE_MAXIMUM(site, i);
	    }
	} 
    }
    
    if (free_after_deleting == TRUE){
	free_agent(SITE_AGENT(site, who));
    }

    for (agent = who; agent < nagents - 1; agent++){
	SITE_AGENT(site, agent) = SITE_AGENT(site, agent + 1);
    } 

    SITE_AGENT(site,  nagents - 1) = (AGENT *)0; /* Should be unnecessary, but it's safer. */
    SITE_NAGENTS(site)--;
    SITE_INTERACTIONS(site) = (AGENT_POPULATION_SIZE) (WORLD_C(the_world) * (double) SITE_NAGENTS(site));
    
    return;
}
    

void
site_make_newborns_old(site)
SITE *site;
{
    AGENT_POPULATION_SIZE agent;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT **agents = SITE_AGENTS(site);

    for (agent = 0; agent < nagents; agent++){
	AGENT_NEWBORN(agents[agent]) = FALSE;
    } 
    
    return;
}

#if 0
#ifndef NO_MUTATION
void
site_mutate_agents(site)
SITE *site;
{
    /*
     * Mutate the agents at a site. We do not differentiate between
     * the old and the newborn. All are equally likely to be mutated.
     * There is no need to do them at separate places since doing the
     * newborn when they are born will not change things as they do not
     * interact during the rest of the generation.
     */
    AGENT_POPULATION_SIZE agent;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT **agents = SITE_AGENTS(site);

    for (agent = 0; agent < nagents; agent++){
	mutate_agent(agents[agent], SITE_P_MUTATION(site));
    } 
    
    return;
}
#endif
#endif

#ifndef NO_SELF_REPLICATION
void
site_self_replicate_agents(site)
SITE *site;
{
    /*
     * Self replicate the agents that can do so. We make sure that the
     * offspring wind up next to the parent on the stack. This is done
     * by first figuring out how many agents can selfrep and then using
     * using this number to figure out the new location of each agent in
     * the stack. The number gets decremented when we do a selfrep.
     */

    AGENT_POPULATION_SIZE nreplicators = 0;
    AGENT_POPULATION_SIZE agent;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT **agents = SITE_AGENTS(site);

    for (agent = 0; agent < nagents; agent++){
	if (can_self_replicate(agents[agent]) == TRUE){
	    AGENT_CAN_SELF_REPLICATE(agents[agent]) = TRUE;
	    nreplicators++;
	}
	else {
	    AGENT_CAN_SELF_REPLICATE(agents[agent]) = FALSE;
	}
    } 
    
    if DEBUG("r") printf("%d agents can self replicate.\n", nreplicators);
    
    if (!nreplicators){
	return;
    }

    if (SITE_MAX_AGENTS(site) - SITE_NAGENTS(site) < nreplicators){
	/* Probably more than we need, but who cares? */
	grow_site(site, nreplicators);
	
	/* This may have now changed. */
	agents = SITE_AGENTS(site);
    }
    
    SITE_NAGENTS(site) = nagents + nreplicators;
    
    for (agent = nagents - 1; agent >= 0 && nreplicators; agent--){
	if (AGENT_CAN_SELF_REPLICATE(agents[agent]) == TRUE){
	    if DEBUG("r") 
		printf("agent %d copied to position %d and its child goes into %d\n", agent, agent + nreplicators - 1, agent + nreplicators);
	    agents[agent + nreplicators - 1] = agents[agent];
	    agents[agent + nreplicators] = agent_self_replicate(agents[agent + nreplicators - 1]);

	    /* Now mutate the new copy. */
	    mutate_agent(agents[agent + nreplicators], SITE_P_MUTATION(site));
	    
	    nreplicators--;
	}
	else {
	    agents[agent + nreplicators] = agents[agent];
	}
    } 
    
    if (nreplicators){
	error("site_self_replicate_agents() does not do all replications!");
    }
    
    return;
}
#endif

#ifndef NO_MIGRATION
void
site_migrate_agents(site)
SITE *site;
{
    /*
     * Migrate those agents that have not gained this round.
     * Make sure to set their gained flag so we don't go on migrating
     * them when we get to the next site (which they may have been moved to).
     * This is another one of those situations where the number of agents
     * at a site changes and yet that is part of the loop condition.
     */
    AGENT_POPULATION_SIZE agent;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT **agents = SITE_AGENTS(site);

    for (agent = 0; agent < nagents; agent++){
	if (AGENT_GAINED(agents[agent]) == FALSE){
	    AGENT_GAINED(agents[agent]) = TRUE;
	    migrate_agent(site, agent);
	    
	    /* There is now one less agent at this site. */
	    nagents--;
	    
	    /* migrate_agent() calls site_delete_agent() which means we use this index again. */
	    agent--;
	}
    } 
    
    return;
}
#endif

void 
print_site(site, fp)
SITE *site;
FILE *fp;
{
    AGENT_POPULATION_SIZE agent;
    
    fprintf(fp, "Site name                : %s\n", SITE_NAME(site));
    fprintf(fp, "Site coordinates         : row %d, column %d\n", SITE_ROW(site), SITE_COL(site));
    fprintf(fp, "Agents at Site           : %d\n", SITE_NAGENTS(site));
    fprintf(fp, "Mutation probability     : %f\n", SITE_P_MUTATION(site));
    fprintf(fp, "Crossover probability    : %f\n", SITE_P_CROSSOVER(site));
    fprintf(fp, "Random death probability : %f\n", SITE_P_RANDOM_DEATH(site));
    
    
    /* PRODUCTION FUNCTION */
    fprintf(fp, "Production function      : ");
    print_resource_list(SITE_PRODUCE_LIST(site), TRUE, fp);
    
    /* RESOURCE LEVELS */
    fprintf(fp, "Current resource levels  : ");
    print_resource_list(SITE_RESOURCE_LIST(site), TRUE, fp);
    
    /* MAXIMUMS */
    fprintf(fp, "Maximum resource levels  : ");
    print_resource_list(SITE_MAXIMUM_LIST(site), TRUE, fp);
    
    /* MAINTENANCE FUNCTION */
    fprintf(fp, "Maintenance function     : ");
    print_resource_list(SITE_MAINTENANCE_LIST(site), TRUE, fp);
    
    
    /* AGENTS */
    
    if (SITE_NAGENTS(site) != (AGENT_POPULATION_SIZE)0){
	fprintf(fp, "Agents                   :\n");
	for (agent = 0; agent < SITE_NAGENTS(site); agent++){
	    fprintf(fp, "Agent %d:\n", agent + 1);
	    print_agent(SITE_AGENT(site, agent), fp);
	    putc('\n', fp);
	} 
    }
    
    return;
}

RESOURCE_SIZE *
site_resource_levels(site)
SITE *site;
{
    static RESOURCE_SIZE *levels = (RESOURCE_SIZE *) 0;
    register AGENT **agents = SITE_AGENTS(site);
    register AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    register AGENT_POPULATION_SIZE agent;

    if (levels == (RESOURCE_SIZE *) 0){
	levels = (RESOURCE_SIZE *) Malloc(nresources * sizeof(RESOURCE_SIZE));
    }

    /* Get the resources lying around at the site. */
    set_resources(levels, SITE_RESOURCE_LIST(site));
    
    /* Now get the resource in the agents - chromosomes and reservoirs. */
    for (agent = 0; agent < nagents; agent++){
	add_resources(levels, AGENT_MAKEUP_LIST(agents[agent]));
	add_resources(levels, AGENT_RESERVE_LIST(agents[agent]));
    } 
    
    return levels;
}

void
site_move_agents_together(agent1, agent2, site)
AGENT_POPULATION_SIZE agent1;
AGENT_POPULATION_SIZE agent2;
SITE *site;
{
    /* Move agent1 next to agent2 in the site's agent stack. */
    
    register int i; 
    register AGENT *tmp;
    register AGENT **agents;

    if (agent1 - agent2 == 1 || agent2 - agent1 == 1){
	return;
    }
    
    agents = SITE_AGENTS(site);
    tmp = agents[agent1];

    if (agent1 > agent2){
	for (i = agent1 - 1; i > agent2; i--){
	    agents[i + 1] = agents[i];
	} 
	agents[agent2 + 1] = tmp;
    }
    else {
	for (i = agent1 + 1; i < agent2; i++){
	    agents[i - 1] = agents[i];
	}
	agents[agent2 - 1] = tmp;
    }
    
    return;
}


void
site_hash_genomes(site, generation)
SITE *site;
GENERATION generation;
{
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT_POPULATION_SIZE agent;
    register AGENT **agents = SITE_AGENTS(site);
    void *hash_return;
    SPECIES_NAME_HASH_DATA *genome_name_node = NULL;

    for (agent = 0; agent < nagents; agent++){
	if ((hash_return = hash_search(species_name_hash, AGENT_NAME(agents[agent]), 0, 0)) == 0){
		error("could not find agent name '%s' in species name hash table.", AGENT_NAME(agents[agent]));
	}
	else {
	    genome_name_node = (SPECIES_NAME_HASH_DATA *)hash_return;
	}
	
	if ((hash_return = hash_search(genome_hash, AGENT_GENOME(agents[agent]), 0, 0)) == 0){
	    /* Not in hash table. */
	    GENOME_HASH_DATA *new = (GENOME_HASH_DATA *) Malloc(sizeof(GENOME_HASH_DATA));
	    
	    genome_name_node->instance_count++;
	    genome_name_node->variants_alive++;
	    genome_name_node->number_alive++;
	    
	    new->alive = TRUE;
	    new->name = strdup(AGENT_NAME(agents[agent]));
	    new->genome = strdup(AGENT_GENOME(agents[agent]));
	    new->first_found = new->this_generation = generation;
	    new->instance = genome_name_node->instance_count;
	    new->count = 1;
	
	    /* Insert it. */
	    hash_search(genome_hash, AGENT_GENOME(agents[agent]), (void *)new, 0);
	}
	else {
	    /* Genome is in the hash table. */
#if !defined(NO_TRANSMUTATE) || defined(SHOW_TRANSMUTATIONS) 
	    if (strcmp(((GENOME_HASH_DATA *)hash_return)->name, AGENT_NAME(agents[agent]))){
		/* This agent has mutated to something of another type! */
#ifdef SHOW_TRANSMUTATIONS
		printf("Agent %d at site (%d, %d) has mutated from a '%s' to a '%s'!\n",
		       agent, SITE_ROW(site), SITE_COL(site), AGENT_NAME(agents[agent]), 
		       ((GENOME_HASH_DATA *)hash_return)->name);
#endif
		/* Change the type of the agent. */
		Free(AGENT_NAME(agents[agent]));
		AGENT_NAME(agents[agent]) = strdup(((GENOME_HASH_DATA *)hash_return)->name);
	    }
#endif
		
	    if (((GENOME_HASH_DATA *)hash_return)->this_generation == generation){
		((GENOME_HASH_DATA *)hash_return)->count++;
	    }
	    else {
		((GENOME_HASH_DATA *)hash_return)->this_generation = generation;
		((GENOME_HASH_DATA *)hash_return)->count = 1;
		((GENOME_HASH_DATA *)hash_return)->alive = TRUE;
		genome_name_node->variants_alive++;
	    }
	    
	    genome_name_node->number_alive++;
	}
    } 

    return;
}


void
site_assign_species_hash_names(site)
SITE *site;
{
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT_POPULATION_SIZE agent;
    register AGENT **agents = SITE_AGENTS(site);
    void *hash_return;

    for (agent = 0; agent < nagents; agent++){
	if ((hash_return = hash_search(species_name_hash, AGENT_NAME(agents[agent]), 0, 0)) == 0){
	    SPECIES_NAME_HASH_DATA *new = (SPECIES_NAME_HASH_DATA *) Malloc(sizeof(SPECIES_NAME_HASH_DATA));

	    new->name = strdup(AGENT_NAME(agents[agent]));
	    new->number_alive = new->variants_alive = new->instance_count = 0;
	    
	    /* Insert it. */
	    hash_search(species_name_hash, AGENT_NAME(agents[agent]), (void *)new, 0);
	}
    }
    
    return;
}

void
site_list_agents(site, header, fp)
SITE *site;
BOOLEAN header;
FILE *fp;
{
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT_POPULATION_SIZE agent;
    register AGENT **agents = SITE_AGENTS(site);
    
    if (header == TRUE){
	fprintf(fp, "Agent stack for site (%d, %d)\n", SITE_ROW(site), SITE_COL(site));
	fprintf(fp, "Pos  Type      Genome              Makeup     Reserve\n");
    }
    
    for (agent = 0; agent < nagents; agent++){
	
	if (header == TRUE){
	    fprintf(fp, "%3d  %s%03d  %20s  ", agent, AGENT_NAME(agents[agent]), agent_instance(agents[agent]), 
		    AGENT_GENOME(agents[agent]));
	}
	else {
	    fprintf(fp, "%s%03d  %20s  ", AGENT_NAME(agents[agent]), agent_instance(agents[agent]), 
		    AGENT_GENOME(agents[agent]));
	}
	
	print_terse_resource_list(AGENT_MAKEUP_LIST(agents[agent]), FALSE, "(", ")", ",", fp);
	
	fprintf(fp, "  ");
	
	print_terse_resource_list(AGENT_RESERVE_LIST(agents[agent]), TRUE, "(", ")", ",", fp);
    }
    
    return;
}


BOOLEAN
site_read_stack_from_file(site, fp, file)
SITE *site;
FILE *fp;
STRING file;
{
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT_POPULATION_SIZE agent;
    register AGENT **agents = SITE_AGENTS(site);
    char line[BUF_SZ];
    AGENT_POPULATION_SIZE line_num = 0;
    
    /* Free all the old agents. This may not be most efficient, but it is certainly simplest. */
    for (agent = 0; agent < nagents; agent++){
	free_agent(agents[agent]);
    }
    
    /* This will save us in case the file can't be read and they continue anyway. */
    SITE_NAGENTS(site) = 0;
    
    while (fgets(line, BUF_SZ, fp)){
	STRING tmp = line;
	STRING tmp2;
	AGENT *new = new_agent();
	register int i;
	
	line_num++;
	skip_to_white(tmp);
	
	if (*tmp == '\0'){
	    popup_message("The agent name must be followed by white space! (line %d)", line_num);
	    return FALSE;
	}
	
	*tmp = '\0';
	tmp2 = tmp - 1;
	
	while (tmp2 >= line && isdigit(*tmp2)){
	    *tmp2 = '\0';
	    tmp2--;
	}
	
	if (line[0] == '\0'){
	    popup_message("The agent must have a name! (line %d)", line_num);
	    return FALSE;
	}
	
	AGENT_NAME(new) = strdup(line);
	
	tmp++;
	
	skip_white(tmp);
	
	/* Now we are at the start of the genome. */
	
	tmp2 = tmp;
	skip_to_white(tmp2);

	if (*tmp2 == '\0'){
	    popup_message("The chromosomes must be followed by white space! (line %d)", line_num);
	    return FALSE;
	}
	
	*tmp2 = '\0';
	
	AGENT_GENOME(new) = strdup(tmp);

	for (i = 0; i < NCHROMOSOMES; i++){
	    STRING underscore = index(tmp, '_');

	    if (!underscore){
		popup_message("Chromosomes must be separated by underscores! (line %d, chromosome %d)", line_num, i + 1);
		return FALSE;
	    }
	    
	    *underscore = '\0';

	    set_chromosome(AGENT_CHROMOSOME(new, i), tmp);

	    tmp = underscore + 1;
	} 
	
	calculate_makeup(new);
	
	if (!IS_VALID_RESOURCE(*tmp)){
	    popup_message("The trading resource, '%c', on line %d is illegal", *tmp, line_num);
	    return FALSE;
	}

	AGENT_TRADING_RESOURCE(new) = *tmp;
	
	tmp++;

	if (*tmp != '_'){
	    popup_message("Chromosomes must be separated by underscores! (line %d)", line_num);
	    return FALSE;
	}

	tmp++;
	
	for (i = 0; i < nresources; i++, tmp++){
	    switch (*tmp){
		case '0': {
		    AGENT_UPTAKE(new, i) = FALSE;
		    break;
		}
		
		case '1': {
		    AGENT_UPTAKE(new, i) = TRUE;
		    break;
		}
		
		default: {
		    popup_message("Resource uptake mask bits must be 0 or 1! (line %d)", line_num);
		    return FALSE;
		}
	    }
	} 
	
	tmp = tmp2 + 1;
	skip_white(tmp);
	
	/* 
	 * We should now be on the makeup list. e.g. (1,23,11,10). We skip this
	 * since it is calculated from the genome already. If they made a mistake
	 * we (silently) go with the genome counts.
	 */

	if (*tmp != '('){
	    popup_message("The agent's makeup list must be enclosed in parentheses! (line %d)", line_num);
	    return FALSE;
	}
	
	tmp = index(tmp, ')');

	if (!tmp){
	    popup_message("The agent's makeup list must be enclosed in parentheses! (line %d)", line_num);
	    return FALSE;
	}
	
	tmp++;
	
	skip_white(tmp);

	/* Should now be on the reserve list. e.g. (1,23,11,10) */
	if (*tmp != '('){
	    popup_message("The agent's reserve list must be enclosed in parentheses! (line %d)", line_num);
	    return FALSE;
	}
	
	tmp++;
	
	for (i = 0; i < nresources - 1; i++){
	    AGENT_RESERVE(new, i) = (RESOURCE_SIZE) atoi(tmp);

	    while (isdigit(*tmp)){
		tmp++;
	    }

	    skip_white(tmp);

	    if (*tmp != ','){
		popup_message("The agent's reserve list entries must be separated by a comma! (line %d)", line_num);
		return FALSE;
	    }
	    
	    tmp++;

	    skip_white(tmp);
	}
	    
	AGENT_RESERVE(new, i) = (RESOURCE_SIZE) atoi(tmp);
	

	/* Now add the new agent to the site. */
	
	if (SITE_NAGENTS(site) == SITE_MAX_AGENTS(site)){
	    grow_site(site);
	}
	
	SITE_AGENT(site, SITE_NAGENTS(site)) = new;
	SITE_NAGENTS(site)++;
    }
    
    site_hash_genomes(site, WORLD_GENERATION(the_world));
    return TRUE;
}

AGENT_POPULATION_SIZE
site_count_schema(schema, site)
STRING schema;
SITE *site;
{
    AGENT_POPULATION_SIZE count = 0;
    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
    AGENT_POPULATION_SIZE agent;
    register AGENT **agents = SITE_AGENTS(site);
    
    for (agent = 0; agent < nagents; agent++){
	if (re_match(schema, AGENT_GENOME(agents[agent])) == TRUE){
	    count++;
	}
    }

    return count;
}
