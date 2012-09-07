#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/options.c,v 1.2 1992/11/16 07:05:45 terry Exp terry $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "types.h"
#include "globals.h"
#include "options.h"
#include "syscalls.h"

typedef struct {
    STRING name;
    int namelen;
    STRING abbreviation;
    int abbrlen;
    OPTION_TYPE type;
    BOOLEAN has_lower_limit;
    double lower_limit;
    BOOLEAN has_upper_limit;
    double upper_limit;
    BOOLEAN required;
    BOOLEAN found;
    void *address;
    STRING explanation;
} REAL_OPTION;

static REAL_OPTION *options = (REAL_OPTION *) 0;
static int total_options = (int) 0;
static int *counts;
static int nlists;

int
process_options(argc, argv, main_options)
int argc;
char **argv;
OPTION *main_options;
{
    int optind;
    
    OPTION *all_options[1];
    int option_sets = 1;

	rcsid = rcsid;
    
    all_options[0] = main_options;
    check_options(main_options, FALSE);
    optind = get_options(argc, argv, all_options, option_sets);
    
#ifdef NO_GRAPHICS
    if (argv[optind] != (STRING) 0){
	sysdie = FALSE;
	error("Command line arguments were found but not expected.");
	sysdie = TRUE;
	error("The first was '%s'.", argv[optind]);
    }
#endif
    
    return optind;
}


int
get_options(argc, argv, options_list, num_option_lists)
int argc;
char **argv;
OPTION **options_list;
int num_option_lists;
{
    register int i;
    int option_index = 1;
    register int pos;
    void check_required_options();

    if (options_list == (OPTION **) 0){
	error("empty option list passed to get_options().");
    }

    if (num_option_lists < (int) 0){
	error("negative number of option lists passed to get_options().");
    }

    nlists = num_option_lists;

    if (nlists == (int) 0){
	return option_index;
    }

    counts = (int *) Malloc(nlists * sizeof(int));

    /* 
     * Count how many options we have in total. Also, into the counts
     * array, put the number in each of the option lists.
     */
    for (i = 0; i < nlists; i++){
	register int opt = (int) 0;
	while (options_list[i][opt].name != (STRING) 0){
	    opt++;
	}
	counts[i] = opt;
	total_options += opt;
    } 
    
    options = (REAL_OPTION *) Malloc(total_options * sizeof(REAL_OPTION));
    
    pos = (int) 0;

    /* Concatenate all the options into one list. And get their lengths. */
    for (i = 0; i < nlists; i++){
	register int opt = (int) 0;
	for (opt = 0; opt < counts[i]; opt++){
	    options[pos].name = options_list[i][opt].name;
	    options[pos].namelen = strlen(options_list[i][opt].name);
	    
	    if (options_list[i][opt].abbreviation != (STRING) 0){
		options[pos].abbreviation = options_list[i][opt].abbreviation;
	    }
	    else {
		options[pos].abbreviation = "";
	    }

	    options[pos].abbrlen = strlen(options[pos].abbreviation);
	    options[pos].type = options_list[i][opt].type;
	    options[pos].has_lower_limit = options_list[i][opt].has_lower_limit;
	    options[pos].lower_limit = options_list[i][opt].lower_limit;
	    options[pos].has_upper_limit = options_list[i][opt].has_upper_limit;
	    options[pos].upper_limit = options_list[i][opt].upper_limit;
	    options[pos].required = options_list[i][opt].required;
	    options[pos].found = FALSE;
	    options[pos].address = options_list[i][opt].address;
	    options[pos].explanation = options_list[i][opt].explanation;
	    pos++;
	}
    }

    /* Now process the options. */
    while (option_index < argc && argv[option_index][0] == '-'){
    
	int match;
	int find_option_match();
	int process_option();

	/* Check for "--", anything after it is a real argument. */
	if (argv[option_index][1] == '-' && argv[option_index][2] == '\0'){
	    option_index++;
	    break;
	}
	
	match = find_option_match(argv[option_index] + 1);
	
	if (match == (int) -1){
#ifndef NO_GRAPHICS
	    /* Ignore unknown options, there may be X options that we don't understand. */
	    option_index++;
	    continue;
#else
	    error("unknown option '%s', try using the -help option if you are confused.", argv[option_index]);
#endif
	}
	
	options[match].found = TRUE;
	
	option_index += process_option(&options[match], argv[option_index], argv[option_index + 1], 
				       option_index == argc - 1 ? TRUE : FALSE);
	option_index++;
    }
    
    check_required_options();

    Free(counts);
    Free(options);

    return option_index;
}

void
check_options(opts, application)
OPTION *opts;
BOOLEAN application;
{
    register int option = (int) 0;
    
    while (opts[option].name){
	if (strlen(opts[option].name) == 0){
	    error("found an option (number %d) with no name in the %s's option list.", 
		  option + 1, application == TRUE ? "application" : "main program");
	}
	option++;
    }
}


int
find_option_match(option)
STRING option;
{
    int find_exact_longname_match();
    int find_partial_longname_match();
    int find_exact_abbreviated_match();
    int find_partial_abbreviated_match();
    
    register int match = find_exact_longname_match(option);
    
    if (match == (int) -1){
	match = find_exact_abbreviated_match(option);
	
	if (match == (int) -1){
	    match = find_partial_longname_match(option);
	    
	    if (match == (int) -1){
		match = find_partial_abbreviated_match(option);
	    }
	}
    }
    
    return match;
}
    

int
find_exact_longname_match(option)
STRING option;
{
    /* There can be no ambiguity, the complete names have been checked in check_options(). */
    register int i;

    for (i = 0; i < total_options; i++){
	if (!strcmp(options[i].name, option)){
	    return i;
	}
    } 
    
    return (int) -1;
}

int
find_exact_abbreviated_match(option)
STRING option;
{
    /* There can be no ambiguity, the complete names have been checked in check_options(). */
    register int i;

    for (i = 0; i < total_options; i++){
	if (strlen(options[i].abbreviation) && !strcmp(options[i].abbreviation, option)){
	    return i;
	}
    } 
    
    return (int) -1;
}

int
find_partial_longname_match(option)
STRING option;
{
    /* Check for ambiguity. */
    register int i;
    register int location = (int) -1;
    register int matches = 0;
    register int max_width = (int) -1;

    for (i = 0; i < total_options; i++){
	if (!strncmp(options[i].name, option, strlen(option))){
	    matches++;
	    location = i;
	    if (options[i].namelen > max_width){
		max_width = options[i].namelen;
	    }
	}
    } 
    
    switch (matches){
	case 0:
	case 1:{
	    return location;
	    break;
	}

	default:{
	    fprintf(stderr, "%s: option '-%s' is ambiguous, it matches the following %d options:\n\n", myname, option, matches);
	    for (i = 0; i < total_options; i++){
		if (!strncmp(options[i].name, option, strlen(option))){
		    fprintf(stderr, "\t-%-*s", max_width + 4, options[i].name);
		    if (strlen(options[i].abbreviation)){
			fprintf(stderr, "(abbreviation: -%s)\n", options[i].abbreviation);
		    }
		    else {
			fprintf(stderr, "(abbreviation: none)\n");
		    }
		}
	    } 
	    fprintf(stderr, "\nThis program will give you more help if invoked with the -help option.\n");
	    exit(1);
	}
    }
    
    return location;
}
    


int
find_partial_abbreviated_match(option)
STRING option;
{
    /* Check for ambiguity. */
    register int i;
    register int location = (int) -1;
    register int matches = 0;
    register int max_width = (int) -1;

    for (i = 0; i < total_options; i++){
	if (strlen(options[i].abbreviation) && !strncmp(options[i].abbreviation, option, strlen(option))){
	    matches++;
	    location = i;
	    if (options[i].namelen > max_width){
		max_width = options[i].namelen;
	    }
	}
    } 
	
    switch (matches){
	case 0:
	case 1:{
	    return location;
	    break;
	}

	default:{
	    fprintf(stderr, "%s: option '-%s' is ambiguous, it matches the following %d options:\n\n", myname, option, matches);
	    for (i = 0; i < total_options; i++){
		if (strlen(options[i].abbreviation) && !strncmp(options[i].abbreviation, option, strlen(option))){
		    fprintf(stderr, "\t-%-*s(fullname: -%s)\n", max_width + 4, options[i].abbreviation, options[i].name);
		}
	    } 
	}
	fprintf(stderr, "\nThis program will give you more help if invoked with the -help option.\n");
	exit(1);
    }
    
    return location;
}
    

int
process_option(option, this_argument, next_argument, is_last_argument)
REAL_OPTION *option;
STRING this_argument;
STRING next_argument;
BOOLEAN is_last_argument;
{
    /*
     * Deal with this option. If we use up the next argument, then
     * return 1 to tell the invoker.
     */
     
    extern int tolower();
    void check_next_argument();
    void check_option_range();
    register int options_processed = (int) 0;
    
    switch (option->type){

	case BOOLEAN_OPT:{
	    if (is_last_argument == FALSE && next_argument != (STRING) 0 && strlen(next_argument) != (int) 0 && *next_argument != '-'){
		register STRING tmp = next_argument;

		/* Change to lower case to avoid testing too many possibilities. */
		while (*tmp != '\0'){
		    if (isupper(*tmp)){
			*tmp = tolower(*tmp);
		    }
		    tmp++;
		}

		if (!strcmp(next_argument, "on") || !strcmp(next_argument, "yes") || 
		    !strcmp(next_argument, "true") || !strcmp(next_argument, "1")){
		    *((BOOLEAN *) option->address) = TRUE;
		}
		else if (!strcmp(next_argument, "off") || !strcmp(next_argument, "no") 
			 || !strcmp(next_argument, "false") || !strcmp(next_argument, "0")){
		    *((BOOLEAN *) option->address) = FALSE;
		}
		else {
		    sysdie = FALSE;
		    error("unknown value '%s' given to boolean option '-%s'.", next_argument, option->name);
		    sysdie = TRUE;
		    error("use the -help option for more information on option arguments.");
		}
		
		options_processed++;
	    }
	    else {
		if (*((BOOLEAN *) option->address) == FALSE){
		    *((BOOLEAN *) option->address) = TRUE;
		}
		else {
		    *((BOOLEAN *) option->address) = FALSE;
		}
	    }
	    break;
	}

	case CHAR_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((STRING) option->address) = *next_argument;
	    check_option_range((double) (*((STRING) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case STRING_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((STRING *) option->address) = strdup(next_argument);
	    options_processed++;
	    break;
	}

	case INT_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((int *) option->address) = atoi(next_argument);
	    check_option_range((double) (*((int *) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case FLOAT_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((float *) option->address) = (float) atof(next_argument);
	    check_option_range((double) (*((float *) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case DOUBLE_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((double *) option->address) = atof(next_argument);
	    check_option_range(*((double *) option->address), option, this_argument);
	    options_processed++;
	    break;
	}

	case LONG_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((long *) option->address) = atol(next_argument);
	    check_option_range((double) (*((long *) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case FUNC0_OPT:{
	    ((void (*)()) option->address)();
	    break;
	}

	case FUNC1_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    ((void (*)()) option->address)(next_argument);
	    options_processed++;
	    break;
	}
	
	default:{
	    error("strange case found in switch in process_option().");
	}
    }
    
    return options_processed;
}

void
check_next_argument(this_argument, next_argument, is_last_argument)
STRING this_argument;
STRING next_argument;
BOOLEAN is_last_argument;
{
    if (is_last_argument == TRUE || next_argument == (STRING) 0 || *next_argument == '\0'){
	error("the '%s' option expects an argument and none was given.", this_argument);
    }
    
    if (*next_argument == '-'){
	sysdie = FALSE;
	error("WARNING: '%s' taken as argument to '%s' option!", next_argument, this_argument);
	sysdie = TRUE;
    }
    
    return;
}
	    

void
check_option_range(value, option, this_argument)
double value;
REAL_OPTION *option;
STRING this_argument;
{
    char message[4096];

    if (option->has_lower_limit == TRUE && value < option->lower_limit){
    
	switch (option->type){
	    
	    case CHAR_OPT:
	    case INT_OPT:{
		sprintf(message, "the value of %d given to %s is below the allowed limit of %d", 
			(int) value, this_argument, (int) option->lower_limit);
		break;
	    }
		
	    case LONG_OPT:{
		sprintf(message, "the value of %ld given to %s is below the allowed limit of %ld", 
			(long) value, this_argument, (long) option->lower_limit);
		break;
	    }

	    case FLOAT_OPT:
	    case DOUBLE_OPT:{
		sprintf(message, "the value of %f given to %s is below the allowed limit of %f", 
			value, this_argument, option->lower_limit);
		break;
	    }
	    
	    default:{
		error("strange case found in switch in check_option_range().");
	    }
	}
	
	error(message);
    }


    if (option->has_upper_limit == TRUE && value > option->upper_limit){
    
	switch (option->type){
	    
	    case CHAR_OPT:
	    case INT_OPT:{
		sprintf(message, "the value of %d given to %s is greater than the allowed limit of %d", 
			(int) value, this_argument, (int) option->upper_limit);
		break;
	    }
		
	    case LONG_OPT:{
		sprintf(message, "the value of %ld given to %s is greater than the allowed limit of %ld", 
			(long) value, this_argument, (long) option->upper_limit);
		break;
	    }

	    case FLOAT_OPT:
	    case DOUBLE_OPT:{
		sprintf(message, "the value of %f given to %s is greater than the allowed limit of %f", 
			value, this_argument, option->upper_limit);
		break;
	    }
	    
	    default:{
		error("strange case found in switch in check_option_range().");
	    }
	}
	
	error(message);
    }
    
    return;
}
	    

void
check_required_options()
{
    register int i;
    BOOLEAN error_found = FALSE;
    
    sysdie = FALSE;
    
    for (i = 0; i < total_options; i++){
	if (options[i].required == TRUE && options[i].found == FALSE){
	    error("the '-%s' option is required but was not given.", options[i].name);
	    error_found = TRUE;
	}
    } 
    sysdie = TRUE;
    
    if (error_found == TRUE){
	error("use the -help option for more information.");
    }
    
    return;
}
    
#define BUF_SZ 4096

void 
read_opts_from_file(file)
STRING file;
{
    FILE *fp = Fopen(file, "r");
    char line[BUF_SZ];
    STRING variable;
    STRING value;
    int line_num = 0;

    if (options == (REAL_OPTION *) 0){
	error("read_opts_from_file(%s) called before options are defined!", file);
    }

    sysdie = FALSE;
    error("reading options from file '%s'.");
    sysdie = TRUE;

    while (fgets(line, BUF_SZ, fp)){
	int match = (int) 0;
	
	line_num++;
	variable = strtok(line, " \t:\n");

	if (variable == (STRING) 0 || *variable == '\0' || *variable == '#'){
	    /* This is an empty or comment line. */
	    continue;
	}
	
	value = strtok((STRING) 0, " \t:\n");
	
	/* The value might be empty, but that is OK since we have boolean commands. */
	
	/* 
	 * Watch for an initial '-' at the start of the option name. This is not needed
	 * but some people may put it in.
	 */
	if (*variable == '-'){
	    variable++;
	}
	
	match = find_option_match(variable);
	
	if (match == (int) -1){
	    sysdie = FALSE;
	    error("unknown option '%s', found on line %d of '%s'.", variable, line_num, file);
	    sysdie = TRUE;
	    error("Try using the -help option if you are confused.");
	}
	
	options[match].found = TRUE;
	
	(void) process_option(&options[match], variable, value, FALSE);
    }
    
    Fclose(fp);
    return;
}

#undef BUF_SZ


void
usage()
{
    void print_options();

    /*
     * Give help. Print it to stdout so that the average person
     * will know how to stop it scrolling off the screen.
     */
     
    printf("                    ECHO (version %s) Usage.\n\n", echo_version);
    printf("Echo understands all X windows options, or if graphics are off,\n");
    printf("these are the options you may use (the -world option turns off graphics).\n\n");
    print_options(stdout, 0, counts[0]);
    exit(1);
}

void
print_options(fp, start, stop)
FILE *fp;
int start;
int stop;
{
    register int i;

    for (i = start; i < stop; i++){
	
	fprintf(fp, "FULL OPTION NAME    : -%s\n", options[i].name);
	
	if (strlen(options[i].abbreviation) != (int) 0){
	    fprintf(fp, "OPTION ABBREVIATION : -%s\n", options[i].abbreviation);
	}
	
	if (options[i].required == TRUE){
	    fprintf(fp, "REQUIRED OPTION     : yes\n");
	}
	
	if (options[i].has_lower_limit == TRUE){
	    
	    fprintf(fp, "LOWER LIMT          : ");
	    
	    switch (options[i].type){
		case CHAR_OPT:{
		    fprintf(fp, "%c\n", (char) options[i].lower_limit);
		    break;
		}
		
		case INT_OPT:{
		    fprintf(fp, "%d\n", (int) options[i].lower_limit);
		    break;
		}
		
		case FLOAT_OPT:{
		    fprintf(fp, "%f\n", (float) options[i].lower_limit);
		    break;
		}
		
		case DOUBLE_OPT:{
		    fprintf(fp, "%f\n", (double) options[i].lower_limit);
		    break;
		}
		
		case LONG_OPT:{
		    fprintf(fp, "%ld\n", (long) options[i].lower_limit);
		    break;
		}
		
		default:{
		    error("usage() found an option (%s) that was not supposed to have a lower bound.", 
			  options[i].name);
		}
	    }
	}
	
	if (options[i].has_upper_limit == TRUE){
	    
	    fprintf(fp, "UPPER LIMT          : ");
	    
	    switch (options[i].type){
		case CHAR_OPT:{
		    fprintf(fp, "%c\n", (char) options[i].upper_limit);
		    break;
		}
		
		case INT_OPT:{
		    fprintf(fp, "%d\n", (int) options[i].upper_limit);
		    break;
		}
		
		case FLOAT_OPT:{
		    fprintf(fp, "%f\n", (float) options[i].upper_limit);
		    break;
		}
		
		case DOUBLE_OPT:{
		    fprintf(fp, "%f\n", (double) options[i].upper_limit);
		    break;
		}
		
		case LONG_OPT:{
		    fprintf(fp, "%ld\n", (long) options[i].upper_limit);
		    break;
		}
		
		default:{
		    error("usage() found an option (%s) that was not supposed to have an upper bound.", 
			  options[i].name);
		}
	    }
	}
	
	if (options[i].explanation != (STRING) 0 && strlen(options[i].explanation) != (int) 0){
	    fprintf(fp, "HELP FOR OPTION     :\n%s\n\n", options[i].explanation);
	}
	else {
	    fprintf(fp, "\n");
	}
    }
    
    return;
}
