#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/resources.c,v 1.4 1992/10/22 04:14:17 terry Exp $";
#endif

#include <stdio.h>

#include "types.h"
#include "globals.h"

void
add_resources(r1, r2)                  
RESOURCE_LIST r1;
RESOURCE_LIST r2;
{
    /* 
     * Add the quantities in r2 onto the quantities in r1. 
     * Ignore overflow... 
     */
    register NRESOURCES resource;

	rcsid = rcsid;
	
    for (resource = 0; resource < nresources; resource++) {         
	r1[resource] += r2[resource];
    }                                                
    
    return;
}

void
sub_resources(r1, r2)                  
RESOURCE_LIST r1;
RESOURCE_LIST r2;
{
    /* 
     * Subtract the quantities in r2 from the quantities in r1. 
     * Make sure the quantities do not get < 0.
     */
    register NRESOURCES resource;
	
    for (resource = 0; resource < nresources; resource++) {         
	r1[resource] -= r2[resource];
	if (r1[resource] < (RESOURCE_SIZE)0){
	    r1[resource] = (RESOURCE_SIZE)0;
	}
    }                                                
    
    return;
}

void
zero_resources(r)                  
RESOURCE_LIST r;
{
    /* 
     * Set the resource list to contain zeroes.
     */
    register NRESOURCES resource;
	
    for (resource = 0; resource < nresources; resource++) {         
	r[resource] = (RESOURCE_SIZE)0;
    }                                                
    
    return;
}

void
set_resources(r1, r2)                  
RESOURCE_LIST r1;
RESOURCE_LIST r2;
{
    /* 
     * Set the quantities in r1 from the quantities in r2. 
     */
    register NRESOURCES resource;
	
    for (resource = 0; resource < nresources; resource++) {         
	r1[resource] = r2[resource];
    }                                                
    
    return;
}

RESOURCE_SIZE
total_resources(r)                  
RESOURCE_LIST r;
{
    /* 
     * Add the quantities in r.
     */
    register NRESOURCES resource;
    register RESOURCE_SIZE total = (RESOURCE_SIZE) 0;
	
    for (resource = 0; resource < nresources; resource++) {         
	total += r[resource];
    }                                                
    
    return total;
}


void
print_resource_list(r, newline, fp)
RESOURCE_LIST r;
BOOLEAN newline;
FILE *fp;
{
    /*
     * Print the resource quantities in the list.
     * Each is preceeded by the resource name and (except for the last)
     * followed by a comma and a space. The last is followed by a newline
     * if newline == TRUE.
     */
    register NRESOURCES resource;

    for (resource = 0; resource < nresources - 1; resource++){
	fprintf(fp, "[%c = %d]  ", 'a' + (int) resource, (int) r[resource]);
    } 
    
    fprintf(fp, "[%c = %d]%s", 'a' + (int) resource, (int) r[resource], newline == TRUE ? "\n" : "");
    return;
}

void
print_terse_resource_list(r, newline, left, right, sep, fp)
RESOURCE_LIST r;
BOOLEAN newline;
STRING left;
STRING right;
STRING sep;
FILE *fp;
{
    /*
     * Print (tersely) the resource quantities in the list.
     * The last is followed by a newline if newline == TRUE.
     */
    register NRESOURCES resource;

    if (nresources == 1){
	fprintf(fp, "%s%d%s%s", left ? left : "", (int) r[0], right ? right : "", newline == TRUE ? "\n" : "");
    }
    else {
	fprintf(fp, "%s%d%s", left ? left : "", (int) r[0], sep ? sep : "");
    
	for (resource = 1; resource < nresources - 1; resource++){
	    fprintf(fp, "%d%s", (int) r[resource], sep ? sep : "");
	} 
    
	fprintf(fp, "%d%s%s", (int) r[resource], right ? right : "", newline == TRUE ? "\n" : "");
    }
    
    return;
}
