#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/free.c,v 1.7 1992/10/22 04:14:17 terry Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "globals.h"
#include "free.h"
#include "syscalls.h"

/*
 * All the freeing routines in here MUST work on data structures
 * that are not copmpletely specified. For example, free_world()
 * may be called with a WORLD data structure that has only some of
 * its sites assigned. For this reason we use Free() instead of free().
 * care must be taken elsewhere when creating new objects to ensure
 * that all of their fields which have dynamic size are set to 0
 * so that we don't go freeing random stuff.
 */

void
free_agent(agent)
AGENT *agent;
{
    /* Free the space associated with an agent. */
    register int i;

	rcsid = rcsid;
    
    if (!agent){
	return;
    }

    Free(AGENT_NAME(agent));

    Free(AGENT_RESERVE_LIST(agent));
    Free(AGENT_MAKEUP_LIST(agent));
    Free(AGENT_UPTAKE_MASK(agent));
    Free(AGENT_GENOME(agent));

    for (i = 0; i < NCHROMOSOMES; i++){
	free_chromosome(AGENT_CHROMOSOME(agent, i));
    } 
    
    Free(agent);
    return;
}

void
free_agent_edit(agent)
AGENT_EDIT *agent;
{
    /* Free the space associated with an agent. */
    register int i;
    
    if (!agent){
	return;
    }

    Free(AGENT_EDIT_NAME(agent));
    
    for (i = 0; i < NCHROMOSOMES; i++){
	Free(AGENT_EDIT_CHROMOSOMES(agent)[i]);
    }
    
    Free(AGENT_EDIT_TRADING_RESOURCE(agent));
    Free(AGENT_EDIT_UPTAKE_MASK(agent));

    Free(agent);
    return;
}

void
free_site(site)
SITE *site;
{
    /* Free the space associated with a site. */
    
    register int i;
    
    if (!site){
	return;
    }
    
    Free(SITE_NAME(site));
    
    for (i = 0; i < site->nagents; i++){
	free_agent(SITE_AGENT(site, i));
    }

    Free(SITE_PRODUCE_LIST(site));
    Free(SITE_MAXIMUM_LIST(site));
    Free(SITE_RESOURCE_LIST(site));
    Free(SITE_MAINTENANCE_LIST(site));

    Free(site);
    return;
}


void
free_site_edit(site)
SITE_EDIT *site;
{
    /* Free the space associated with a site_edit. */
    
    if (!site){
	return;
    }
    
    Free(SITE_EDIT_NAME(site));
    Free(SITE_EDIT_AGENTS(site));
    Free(SITE_EDIT_P_MUTATION(site));
    Free(SITE_EDIT_P_CROSSOVER(site));
    Free(SITE_EDIT_P_RANDOM_DEATH(site));
    Free(SITE_EDIT_PRODUCE_LIST(site));
    Free(SITE_EDIT_MAXIMUM_LIST(site));
    Free(SITE_EDIT_RESOURCE_LIST(site));
    Free(SITE_EDIT_MAINTENANCE_LIST(site));

    Free(site);
    return;
}

void
free_world(world)
WORLD *world;
{
    /* Free the space associated with a world. */
    
    COORD row;
    COORD col;
    NRESOURCES resource;
  
    if (!world){
	return;
    }
    
    Free(WORLD_NAME(world));
    
    for (row = 0; row < WORLD_ROWS(world); row++){
	for (col = 0; col < WORLD_COLS(world); col++){
	    free_site(WORLD_SITE(world, row, col));
	}
    }
    
    for (row = 0; row < WORLD_ROWS(world); row++){
	Free(WORLD_ROW(world, row));
    }
    
    Free(WORLD_SITES(world));
    
    for (resource = 0; resource < WORLD_NRESOURCES(world); resource++){
	Free(WORLD_COMBAT(world)[resource]);
    } 

    Free(WORLD_COMBAT(world));
    
    Free(world);
    return;
}

void
free_world_edit(world)
WORLD_EDIT *world;
{
    /* Free the space associated with a world_edit. */
    
    register NRESOURCES resource;
    register NRESOURCES resources_in_world;
    
    if (!world){
	return;
    }
    
    resources_in_world = (NRESOURCES) atoi(WORLD_EDIT_NRESOURCES(world));
    
    for (resource = 0; resource < resources_in_world; resource++){
	Free(WORLD_EDIT_COMBAT(world)[resource]);
    } 

    Free(WORLD_EDIT_NAME(world));
    Free(WORLD_EDIT_SITES(world));
    Free(WORLD_EDIT_NRESOURCES(world));
    Free(WORLD_EDIT_ROWS(world));
    Free(WORLD_EDIT_COLS(world));
    Free(WORLD_EDIT_COMBAT(world));
    Free(WORLD_EDIT_TRADE_FRACTION(world));
    Free(WORLD_EDIT_C(world));
    Free(WORLD_EDIT_P_MAINTENANCE(world));

    Free(world);
    return;
}

void
free_chromosome(chromosome)
CHROMOSOME *chromosome;
{
    /* Free the space associated with a chromosome. */
    
    if (!chromosome){
	return;
    }
    
    Free(CHROMOSOME_STR(chromosome));
    Free(chromosome);
    return;
}
