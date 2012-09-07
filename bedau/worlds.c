#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/worlds.c,v 1.7 1992/11/16 07:05:45 terry Exp terry $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "types.h"
#include "globals.h"
#include "syscalls.h"
#include "worlds.h"
#include "sites.h"
#include "chromosomes.h"
#include "resources.h"
#include "running.h"
#include "free.h"
#include "defaults.h"
#include "graphics.h"
#include "random.h"
#include "main.h"

WORLD *
new_world(rows, cols, resources)
COORD rows;
COORD cols;
NRESOURCES resources;
{
    /* Make a new world. */
    
    WORLD *world;
    register COORD row;
    register COORD col;
    register NRESOURCES resource;

	rcsid = rcsid;
    
    world = (WORLD *) Malloc(sizeof(WORLD));

    /* Make space for the site pointers. */
    WORLD_SITES(world) = (SITE ***) Malloc(rows * sizeof(SITE **));

    for (row = 0; row < rows; row++){
	WORLD_ROW(world, row) = (SITE **) Malloc(cols * sizeof(SITE *));
    } 
    
    for (row = 0; row < rows; row++){
	for (col = 0; col < cols; col++){
	    WORLD_SITE(world, row, col) = (SITE *) 0;
	}
    }
    
    /* Make space for the combat matrix. */
    WORLD_COMBAT(world) = (RESOURCE_SIZE **) Malloc(resources * sizeof(RESOURCE_SIZE *));
    
    for (resource = 0; resource < resources; resource++){
	WORLD_COMBAT(world)[resource] = (RESOURCE_SIZE *) Malloc(resources * sizeof(RESOURCE_SIZE));
    } 
    
    /* And now initialize all the things that have static size. */
    WORLD_NAME(world) = (char *) 0;
    WORLD_RUNNING(world) = FALSE;
    WORLD_ROWS(world) = rows;
    WORLD_COLS(world) = cols;
    WORLD_NRESOURCES(world) = resources;
    WORLD_TRADE_FRACTION(world) = DEFAULT_TRADE_FRACTION;
    WORLD_C(world) = DEFAULT_C;
    WORLD_SELFREP_FRACTION(world) = DEFAULT_SELFREP_FRACTION;
    WORLD_SELFREP_THRESHOLD(world) = DEFAULT_SELFREP_THRESHOLD;
    WORLD_P_MAINTENANCE(world) = DEFAULT_MAINTENANCE;
    WORLD_NEIGHBORHOOD(world) = DEFAULT_NEIGHBORHOOD;
    WORLD_GENERATION(world) = (GENERATION) 1;
    WORLD_HALT_GENERATION(world) = (GENERATION) 0;
#ifndef NO_MUTATION
    WORLD_MUTATIONS(world) = (COUNT)0;
#endif
    
    return world;
}

WORLD_EDIT *
new_world_edit()
{
    /* Make a new world edit. */
    
    WORLD_EDIT *world;
    
    world = (WORLD_EDIT *) Malloc(sizeof(WORLD_EDIT));

    WORLD_EDIT_NAME(world) = "";
    WORLD_EDIT_SITES(world) = "";
    WORLD_EDIT_ROWS(world) = "";
    WORLD_EDIT_COLS(world) = "";
    WORLD_EDIT_NRESOURCES(world) = "";
    WORLD_EDIT_TRADE_FRACTION(world) = "";
    WORLD_EDIT_COMBAT(world) = (char **)0;
    WORLD_EDIT_C(world) = "";
    WORLD_EDIT_SELFREP_FRACTION(world) = "";
    WORLD_EDIT_SELFREP_THRESHOLD(world) = "";
    WORLD_EDIT_P_MAINTENANCE(world) = "";
    WORLD_EDIT_NEIGHBORHOOD(world) = "";
    
    return world;
}

#define BUF_SZ 8092

WORLD *
read_world_from_file(dir, file)
char *dir;
char *file;
{
    /*
     * Read a world from a file, create a new world data structure
     * and fill in the fields.
     */

    WORLD *new;
    WORLD_EDIT *edit;
    char *row_as_str;
    COORD nrows;
    COORD row = 0;
    COORD ncols;
    NRESOURCES resource;
    char *newline;
    
#ifdef CHECK_ARGS
    if (!file){
	error("read_world_from_file() called with NULL file argument.");
    }
#endif

    /*
     * We read the file into a world edit structure as then
     * we can extract the row and column numbers and thus create a new
     * world with the correct dimensions. Also, the world edit
     * structure will the sites into a char * for us that we can
     * then go ahead and construct actual sites from.
     */

    edit = read_world_edit_from_file(dir, file);
    
    if (!edit){
	return (WORLD *) 0;
    }
    
    nrows = (COORD) atoi(WORLD_EDIT_ROWS(edit));
    ncols = (COORD) atoi(WORLD_EDIT_COLS(edit));
    nresources = (NRESOURCES) atoi(WORLD_EDIT_NRESOURCES(edit));
    
    if (nrows * ncols <= 0 || (nrows < 0 && ncols < 0)){
	popup_message("world '%s' has an invalid row/col\nspecification (%d/%d).", WORLD_EDIT_ROWS(edit), WORLD_EDIT_COLS(edit));
	free_world_edit(edit);
	return (WORLD *) 0;
    }

    new = new_world(nrows, ncols, nresources);
    
    WORLD_NAME(new) = strdup( WORLD_EDIT_NAME(edit) );
    WORLD_TRADE_FRACTION(new) = atof( WORLD_EDIT_TRADE_FRACTION(edit) );
    WORLD_C(new) = atof( WORLD_EDIT_C(edit) );
    WORLD_SELFREP_FRACTION(new) = atof( WORLD_EDIT_SELFREP_FRACTION(edit) );
    WORLD_SELFREP_THRESHOLD(new) = atof( WORLD_EDIT_SELFREP_THRESHOLD(edit) );
    WORLD_P_MAINTENANCE(new) = atof( WORLD_EDIT_P_MAINTENANCE(edit) );
    
    if (!strcmp(WORLD_EDIT_NEIGHBORHOOD(edit), "NEWS")){
	WORLD_NEIGHBORHOOD(new) = NEWS;
    }
    else if (!strcmp(WORLD_EDIT_NEIGHBORHOOD(edit), "EIGHT")){
	WORLD_NEIGHBORHOOD(new) = ALLEIGHT;
    }
    else if (!strcmp(WORLD_EDIT_NEIGHBORHOOD(edit), "NONE")){
	WORLD_NEIGHBORHOOD(new) = NONE;
    }
    else {
	popup_message("Unrecognized neighborhood type (%s) found\nin file '%s' (try NEWS, EIGHT or NONE).", 
		      WORLD_EDIT_NEIGHBORHOOD(edit), file);
	free_world_edit(edit);
	free_world(new);
	return (WORLD *) 0;
    }

    /* Get the combat matrix out of the edit structure. */
    for (resource = 0; resource < nresources; resource++){
	register int i; 
	char *resource_str = strtok(WORLD_EDIT_COMBAT(edit)[resource], " \t");

	if (!resource_str) {
	    popup_message("Could not find combat matrix entry (%d, 0)\nin WORLD_EDIT struct in read_world_from_file().", resource + 1);
	    free_world_edit(edit);
	    free_world(new);
	    return (WORLD *) 0;
	}
	
	WORLD_COMBAT_XY(new, resource, 0) = (RESOURCE_SIZE) atoi(resource_str);

	for (i = 1; i < nresources; i++){
	    resource_str = strtok(NULL, " \t");

	    if (!resource_str) {
		popup_message("Could not find combat matrix entry (%d, %d)\nin WORLD_EDIT struct in read_world_from_file().", resource + 1, i);
		free_world_edit(edit);
		free_world(new);
		return (WORLD *) 0;
	    }
	    
	    WORLD_COMBAT_XY(new, resource, i) = (RESOURCE_SIZE) atoi(resource_str);
	} 
    } 
    
    /* Get the site array from the edit structure. */
    row_as_str = WORLD_EDIT_SITES(edit);

    do {
	COORD col;
	char *site_name = row_as_str;
	
	newline = index(row_as_str, '\n');

	if (newline){
	    *newline = '\0';
	}
	
	for (col = 0; col < ncols; col++){
	    SITE *site;
	    char *tmp;
	    skip_white(site_name);
	    
	    if (!site_name){
		popup_message("Could not find %d sites for row %d in world file\n'%s'.", file, ncols, row + 1);
		free_world_edit(edit);
		free_world(new);
		return (WORLD *) 0;
	    }

	    tmp = site_name;
	    skip_to_white(tmp);
	    *tmp = '\0';
	    site = read_site_from_file("sites", site_name);
	    
	    if (!site){
		free_world_edit(edit);
		free_world(new);
		return (WORLD *) 0;
	    }

	    SITE_ROW(site) = row;
	    SITE_COL(site) = col;
	    SITE_INTERACTIONS(site) = (AGENT_POPULATION_SIZE) (WORLD_C(new) * (double) SITE_NAGENTS(site));
	    WORLD_SITE(new, row, col) = site;
	    site_name = tmp + 1;
	}
	
	row_as_str = newline + 1;
	row++;

    } while (row < nrows);
    
    free_world_edit(edit);
    return new;
}


WORLD_EDIT *
read_world_edit_from_file(dir, file)
char *dir;
char *file;
{
    /*
     * Read a world from a file, create a new world_edit data structure
     * and fill in the fields. 'dir' may be empty, in which case we
     * just open 'file'. If not, we concatenate the two and open the
     * result.
     */

    FILE *fp;
    char *name = file;
    WORLD_EDIT *new;
    char line[BUF_SZ];
    int line_num = 0;
    NRESOURCES combat_matrix_row = (NRESOURCES) 0;
    NRESOURCES resources = 0;
    struct stat sbuf;
    
#ifdef CHECK_ARGS
    if (!file){
	error("read_world_edit_from_file() called with NULL file argument.");
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
    new = new_world_edit();

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

	if (!strcmp(SITES_STR, variable)){
	    /* Read the rest of the file... */
	    long length = (long) sbuf.st_size - Ftell(fp);
	    int nread;

	    WORLD_EDIT_SITES(new) = Malloc(length + 1);
	    
	    if ((nread = fread(WORLD_EDIT_SITES(new), sizeof(char), length, fp)) != length){
		error("fread returns %d bytes (expected %d) at end of '%s' world file.", nread, length, name);
	    }
	    
	    WORLD_EDIT_SITES(new)[length] = '\0';

	    /* We may as well break as this stuff reads to the end of the file. */
	    break;
	}
	
	/* Is this the name? */
	if (!strcmp(variable, NAME_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_NAME(new) = strdup(value);
	    continue;
	}

	/* Is this the combat function? */
	if (!strcmp(variable, COMBAT_STR)){
	    
	    register char *nl;

	    if (WORLD_EDIT_COMBAT(new) == (char **)0){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	    
	    if (combat_matrix_row >= resources){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	    
	    value = line + strlen(variable) + 1;

	    while (*value == ' ' || *value == '\t' || *value == ':'){
		value++;
	    }

	    nl = strchr(value, '\n');

	    if (!nl){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	    
	    *nl = '\0';
	    
	    WORLD_EDIT_COMBAT(new)[combat_matrix_row] = strdup(value);
	    combat_matrix_row++;

	    continue;
	}

	/* Is this the trading fraction? */
	if (!strcmp(variable, TRADE_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_TRADE_FRACTION(new) = strdup(value);
	    continue;
	}
	
	/* Is this the interaction fraction (C)? */
	if (!strcmp(variable, C_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_C(new) = strdup(value);
	    continue;
	}
	
	/* Is this the self-reproduction fraction? */
	if (!strcmp(variable, SELFREP_FRACTION_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_SELFREP_FRACTION(new) = strdup(value);
	    continue;
	}
	
	/* Is this the self-reproduction threshold? */
	if (!strcmp(variable, SELFREP_THRESHOLD_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_SELFREP_THRESHOLD(new) = strdup(value);
	    continue;
	}
	
	/* Is this the maintenance probability? */
	if (!strcmp(variable, M_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_P_MAINTENANCE(new) = strdup(value);
	    continue;
	}
	
	/* Is this the neighborhood function? */
	if (!strcmp(variable, NEIGHBORHOOD_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_NEIGHBORHOOD(new) = strdup(value);
	    continue;
	}
	
	/* Is this the number of rows? */
	if (!strcmp(variable, ROWS_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_ROWS(new) = strdup(value);
	    continue;
	}
	
	/* Is this the number of columns? */
	if (!strcmp(variable, COLS_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_COLS(new) = strdup(value);
	    continue;
	}
	
	/* Is this the number of resources? */
	if (!strcmp(variable, NRESOURCES_STR)){
	    value = strtok(NULL, " :\t\n");
	    
	    if (!value){
		variable_has_no_value(variable, line_num, name, dir ? TRUE : FALSE);
		free_world_edit(new);
		return (WORLD_EDIT *) 0;
	    }
	
	    WORLD_EDIT_NRESOURCES(new) = strdup(value);
	    resources = (NRESOURCES) atoi(value);
	    
	    /* Make space for the combat matrix. */
	    WORLD_EDIT_COMBAT(new) = (char **) Malloc(resources * sizeof(char *));
	    
	    continue;
	}
	
	if (recognized == FALSE){
	    /* We don't know what it is... */
	    popup_message("Unrecognized line (%d) in file\n'%s'.", line_num, name);
	    free_world_edit(new);
	    return (WORLD_EDIT *) 0;
	}
    }
	    
    /* Check that the combat matrix was fully specified. */
    if (combat_matrix_row != resources){
	popup_message("Not enough (%d instead of %d) combat matrix lines found\nin '%s'.", combat_matrix_row, resources, name);
	free_world_edit(new);
	return (WORLD_EDIT *) 0;
    }

    if (dir){
	Free(name);
    }

    Fclose(fp);
    return new;
}

void
write_world_to_file(dir, file, site)
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
write_world_edit_to_file(dir, file, world)
char *dir;
char *file;
WORLD_EDIT *world;
{
    FILE *in_fp;
    FILE *out_fp;
    char *name = file;
    register int i; 
    register NRESOURCES resource;
    register NRESOURCES resources;
    char tmp_file[MAXPATHLEN];
    char line[BUF_SZ];
    extern char *mktemp();

#ifdef CHECK_ARGS
    if (!file){
	error("write_world_to_file() called with NULL file argument.");
    }
#endif
    
    sprintf(tmp_file, "/tmp/__echo__XXXXXX");
    mktemp(tmp_file);

    if (!tmp_file){
	error("could not make temporary file in write_world_to_file().");
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
    
    fprintf(out_fp, "%s : %s\n", NAME_STR, WORLD_EDIT_NAME(world));
    fprintf(out_fp, "%s : %s\n", TRADE_STR, WORLD_EDIT_TRADE_FRACTION(world));
    fprintf(out_fp, "%s : %s\n", C_STR, WORLD_EDIT_C(world));
    fprintf(out_fp, "%s : %s\n", SELFREP_FRACTION_STR, WORLD_EDIT_SELFREP_FRACTION(world));
    fprintf(out_fp, "%s : %s\n", SELFREP_THRESHOLD_STR, WORLD_EDIT_SELFREP_THRESHOLD(world));
    fprintf(out_fp, "%s : %s\n", M_STR, WORLD_EDIT_P_MAINTENANCE(world));
    fprintf(out_fp, "%s : %s\n", NEIGHBORHOOD_STR, WORLD_EDIT_NEIGHBORHOOD(world));
    fprintf(out_fp, "%s : %s\n", ROWS_STR, WORLD_EDIT_ROWS(world));
    fprintf(out_fp, "%s : %s\n", COLS_STR, WORLD_EDIT_COLS(world));
    fprintf(out_fp, "%s : %s\n", NRESOURCES_STR, WORLD_EDIT_NRESOURCES(world));
    
    resources = (NRESOURCES) atoi(WORLD_EDIT_NRESOURCES(world));

    for (resource = 0; resource < resources; resource++){
	fprintf(out_fp, "%s : %s\n", COMBAT_STR, WORLD_EDIT_COMBAT(world)[resource]);
    } 

    /* Zap trailing newlines, if any. */
    i = strlen(WORLD_EDIT_SITES(world)) - 1;
    
    while (i >= 0 && WORLD_EDIT_SITES(world)[i] == '\n'){
	WORLD_EDIT_SITES(world)[i] = '\0';
	i--;
    }

    fprintf(out_fp, "%s : \n%s\n", SITES_STR, WORLD_EDIT_SITES(world));

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

void
instantiate_world(dir, file, hash_size)
STRING dir;
STRING file;
int hash_size;
{
    static STRING last_dir = NULL;
    static STRING last_file = NULL;

    /* We should test to see if the same world has been selected & do nothing if so. */
    if (the_world){
	free_world(the_world);
	Fclose(site_file);
#ifndef NO_CANVAS
	destroy_canvas_widgets(WORLD_ROWS(the_world), WORLD_COLS(the_world));
#endif
    }

    if (!dir || !file){
	if (!dir && !file && last_dir && last_file){
	    dir = last_dir;
	    file = last_file;
	}
	else {
	    error("instantiate_world called with bad arguments.");
	}
    }
    else {
	if (last_dir && last_file){
	    Free(last_dir);
	    Free(last_file);
	}

	last_dir = strdup(dir);
	last_file = strdup(file);
    }

    old_seed = seed = seed_random(seed);
    seed = NO_SEED;

    the_world = read_world_from_file(dir, file);
    
    if (!the_world){
	return;
    }

    /* BEDAU: */
    open_gene_files(genes_to_dump, old_seed);

    /* BEDAU: */
    if (dump_species == TRUE){
	open_species_file();
    }


#ifndef NO_TCL
    open_site_file();
    fprintf(site_file, "%d\n%d\n", WORLD_ROWS(the_world), WORLD_COLS(the_world));
#endif

    nresources = WORLD_NRESOURCES(the_world);

#ifndef NO_GRAPHICS
    if (graphics == TRUE){
#ifndef NO_CANVAS
	create_canvas_widgets(WORLD_ROWS(the_world), WORLD_COLS(the_world));
#endif
	initialize_resource_graph();
	initialize_population_graph();
	initialize_schema_graph();
	initialize_variant_graph();
	run_world(hash_size);
    }
#endif
    
    return;
}

AGENT_POPULATION_SIZE
world_population_size(world)
WORLD *world;
{
    AGENT_POPULATION_SIZE count = (AGENT_POPULATION_SIZE) 0;
    register COORD row;
    register COORD col;
    
    for (row = 0; row < WORLD_ROWS(world); row++){
	for (col = 0; col < WORLD_COLS(world); col++){
	    count += SITE_NAGENTS( WORLD_SITE(world, row, col) );
	} 
    } 
    
    return count;
}
		
RESOURCE_LIST
world_resource_levels(levels, world)
RESOURCE_LIST levels;
WORLD *world;
{
    /*
     * This finds and returns the world wide resource levels.
     * This is slightly optimized to avoid initially setting the levels
     * to zero. We compute site (0, 0), then add the rest of the first row
     * then add all the remaining rows.
     */

    register COORD row;
    register COORD col;
    
    set_resources(levels, site_resource_levels(WORLD_SITE(world, 0, 0)));
    
    for (col = 1; col < WORLD_COLS(world); col++){
	add_resources(levels, site_resource_levels(WORLD_SITE(world, 0, col)));
    }

    for (row = 1; row < WORLD_ROWS(world); row++){
	for (col = 0; col < WORLD_COLS(world); col++){
	    add_resources(levels, site_resource_levels(WORLD_SITE(world, row, col)));
	} 
    } 
    
    return levels;
}

void
variable_has_no_value(variable, line_num, name, free_it)
STRING variable;
int line_num;
STRING name;
BOOLEAN free_it;
{
    popup_message("Variable '%s' on line %d of\n'%s' does not have a value!", variable, line_num, name);
		
    if (free_it == TRUE){
	Free(name);
    }
}

AGENT_POPULATION_SIZE 
world_count_schema(schema)
STRING schema;
{
    AGENT_POPULATION_SIZE count = 0;
    
    register COORD row;
    register COORD col;
    
    for (row = 0; row < WORLD_ROWS(the_world); row++){
	for (col = 0; col < WORLD_COLS(the_world); col++){
	    count += site_count_schema(schema, WORLD_SITE(the_world, row, col));
	} 
    } 
    
    return count;
}
