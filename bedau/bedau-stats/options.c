#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/s/me/bedau/RCS/options.c,v 1.1 1994/03/29 05:30:54 terry Exp terry $";
#endif


#ifndef NeXT
extern double atof();
#endif

#include "types.h"
#include "options.h"
#include "syscalls.h"

#include <string.h>
#if ! defined(mips)
#include <strings.h>
#endif

#include <ctype.h>
#include <stdlib.h>

typedef struct {
    STRING name;
    INT namelen;
    STRING abbreviation;
    INT abbrlen;
    OPTION_TYPE type;
    BOOLEAN has_lower_limit;
    DOUBLE lower_limit;
    BOOLEAN has_upper_limit;
    DOUBLE upper_limit;
    BOOLEAN required;
    BOOLEAN found;
    VOID *address;
    STRING explanation;
} REAL_OPTION;

static REAL_OPTION *options = (REAL_OPTION *) 0;
static INT total_options = (INT) 0;
static INT *counts;
static INT nlists;

INT
process_options(argc, argv, main_options)
INT argc;
CHAR **argv;
OPTION *main_options;
{
    INT optind;
    OPTION *all_options[1];
    INT option_sets = 1;
    
    all_options[0] = main_options;
    check_options(main_options, FALSE);

    optind = get_options(argc, argv, all_options, option_sets);
    
    if (argv[optind] != (CHAR *) 0){
	sysdie = FALSE;
	error("Command line arguments were found but not expected.");
	sysdie = TRUE;
	error("The first was '%s'.", argv[optind]);
    }
    
    return optind;
}


INT
get_options(argc, argv, options_list, num_option_lists)
INT argc;
CHAR **argv;
OPTION **options_list;
INT num_option_lists;
{
    register INT i;
    INT option_index = 1;
    register INT pos;
    VOID check_required_options();

    if (options_list == (OPTION **) 0){
	error("empty option list passed to get_options().");
    }

    if (num_option_lists < (INT) 0){
	error("negative number of option lists passed to get_options().");
    }

    nlists = num_option_lists;

    if (nlists == (INT) 0){
	return option_index;
    }

    counts = (INT *) Malloc(nlists * sizeof(INT));

    /* 
     * Count how many options we have in total. Also, into the counts
     * array, put the number in each of the option lists.
     */
    for (i = 0; i < nlists; i++){
	register INT opt = (INT) 0;
	while (options_list[i][opt].name != (STRING) 0){
	    opt++;
	}
	counts[i] = opt;
	total_options += opt;
    } 
    
    options = (REAL_OPTION *) Malloc(total_options * sizeof(REAL_OPTION));
    
    pos = (INT) 0;

    /* Concatenate all the options into one list. And get their lengths. */
    for (i = 0; i < nlists; i++){
	register INT opt = (INT) 0;
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
    
	INT match;
	INT find_option_match();
	INT process_option();

	/* Check for "--", anything after it is a real argument. */
	if (argv[option_index][1] == '-' && argv[option_index][2] == '\0'){
	    option_index++;
	    break;
	}
	
	match = find_option_match(argv[option_index] + 1);
	
	if (match == (INT) -1){
	    error("unknown option '%s', try using the -help option if you are confused.", argv[option_index]);
	}
	
	options[match].found = TRUE;
	
	option_index += process_option(&options[match], argv[option_index], argv[option_index + 1], 
				       option_index == argc - 1 ? TRUE : FALSE);
	option_index++;
    }
    
    check_required_options();

    free(counts);
    free(options);

    return option_index;
}

VOID
check_options(opts, application)
OPTION *opts;
BOOLEAN application;
{
    register INT option = (INT) 0;
    
    while (opts[option].name){
	if (strlen(opts[option].name) == 0){
	    error("found an option (number %d) with no name in the %s's option list.", 
		  option + 1, application == TRUE ? "application" : "main program");
	}
	option++;
    }
}


INT
find_option_match(option)
STRING option;
{
    INT find_exact_longname_match();
    INT find_partial_longname_match();
    INT find_exact_abbreviated_match();
    INT find_partial_abbreviated_match();
    
    register INT match = find_exact_longname_match(option);
    
    if (match == (INT) -1){
	match = find_exact_abbreviated_match(option);
	
	if (match == (INT) -1){
	    match = find_partial_longname_match(option);
	    
	    if (match == (INT) -1){
		match = find_partial_abbreviated_match(option);
	    }
	}
    }
    
    return match;
}
    

INT
find_exact_longname_match(option)
STRING option;
{
    /* There can be no ambiguity, the complete names have been checked in check_options(). */
    register INT i;

    for (i = 0; i < total_options; i++){
	if (!strcmp(options[i].name, option)){
	    return i;
	}
    } 
    
    return (INT) -1;
}

INT
find_exact_abbreviated_match(option)
STRING option;
{
    /* There can be no ambiguity, the complete names have been checked in check_options(). */
    register INT i;

    for (i = 0; i < total_options; i++){
	if (strlen(options[i].abbreviation) && !strcmp(options[i].abbreviation, option)){
	    return i;
	}
    } 
    
    return (INT) -1;
}

INT
find_partial_longname_match(option)
STRING option;
{
    /* Check for ambiguity. */
    register INT i;
    register INT location = (INT) -1;
    register INT matches = 0;
    register INT max_width = (INT) -1;

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
    


INT
find_partial_abbreviated_match(option)
STRING option;
{
    /* Check for ambiguity. */
    register INT i;
    register INT location = (INT) -1;
    register INT matches = 0;
    register INT max_width = (INT) -1;

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
    

INT
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
     
    VOID check_next_argument();
    VOID check_option_range();
    extern INT tolower();
    register INT options_processed = (INT) 0;
    
    switch (option->type){

	case BOOLEAN_OPT:{
	    if (is_last_argument == FALSE && next_argument != (STRING) 0 && strlen(next_argument) != (INT) 0 && *next_argument != '-'){
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
	    *((CHAR *) option->address) = *next_argument;
	    check_option_range((DOUBLE) (*((CHAR *) option->address)), option, this_argument);
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
	    *((INT *) option->address) = atoi(next_argument);
	    check_option_range((DOUBLE) (*((INT *) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case FLOAT_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((FLOAT *) option->address) = (FLOAT) atof(next_argument);
	    check_option_range((DOUBLE) (*((FLOAT *) option->address)), option, this_argument);
	    options_processed++;
	    break;
	}

	case DOUBLE_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((DOUBLE *) option->address) = atof(next_argument);
	    check_option_range(*((DOUBLE *) option->address), option, this_argument);
	    options_processed++;
	    break;
	}

	case LONG_OPT:{
	    check_next_argument(this_argument, next_argument, is_last_argument);
	    *((LONG *) option->address) = atol(next_argument);
	    check_option_range((DOUBLE) (*((LONG *) option->address)), option, this_argument);
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

VOID
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
	    

VOID
check_option_range(value, option, this_argument)
DOUBLE value;
REAL_OPTION *option;
STRING this_argument;
{
    CHAR message[4096];

    if (option->has_lower_limit == TRUE && value < option->lower_limit){
    
	switch (option->type){
	    
	    case CHAR_OPT:
	    case INT_OPT:{
		sprintf(message, "the value of %d given to %s is below the allowed limit of %d", 
			(INT) value, this_argument, (INT) option->lower_limit);
		break;
	    }
		
	    case LONG_OPT:{
		sprintf(message, "the value of %ld given to %s is below the allowed limit of %ld", 
			(LONG) value, this_argument, (LONG) option->lower_limit);
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
			(INT) value, this_argument, (INT) option->upper_limit);
		break;
	    }
		
	    case LONG_OPT:{
		sprintf(message, "the value of %ld given to %s is greater than the allowed limit of %ld", 
			(LONG) value, this_argument, (LONG) option->upper_limit);
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
	    

VOID
check_required_options()
{
    register INT i;
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
    
VOID 
read_opts_from_file(file)
STRING file;
{
    FILE *fp = Fopen(file, "r");
    CHAR line[BUF_SZ];
    INT line_num = 0;

    if (options == (REAL_OPTION *) 0){
	error("read_opts_from_file(%s) called before options are defined!", file);
    }

    sysdie = FALSE;
    error("reading options from file '%s'.", file);
    sysdie = TRUE;

    while (fgets(line, BUF_SZ, fp)){
	STRING variable;
	STRING value;
	STRING quote;
	INT match = (INT) 0;
	
	line_num++;
	quote = strchr(line, '"');

	variable = strtok(line, " \t:\n");

	if (variable == (STRING) 0 || *variable == '\0' || *variable == '#'){
	    /* This is an empty or comment line. */
	    continue;
	}
	
	if (quote){
	    value = quote + 1;
	    if (!(quote = strrchr(value, '"'))){
		error("cannot find closing quote on line %d of options file '%s'", line_num, file);
	    }
	    
	    *quote = '\0';
	}
	else {
	    value = strtok((STRING) 0, " \t:\n");
	}
	
	/* The value might be empty, but that is OK since we have boolean commands. */
	
	/* 
	 * Watch for an initial '-' at the start of the option name. This is not needed
	 * but some people may put it in.
	 */
	if (*variable == '-'){
	    variable++;
	}
	
	match = find_option_match(variable);
	
	if (match == (INT) -1){
	    sysdie = FALSE;
	    error("unknown option '%s', found on line %d of '%s'.", variable, line_num, file);
	    sysdie = TRUE;
	    error("Try using the -help option if you are confused.");
	}
	
	options[match].found = TRUE;
	
	(VOID) process_option(&options[match], variable, value, FALSE);
    }
    
    Fclose(fp);
    return;
}

VOID
usage()
{
    VOID print_options();

    /*
     * Give help. Print it to stdout so that the average person
     * will know how to stop it scrolling off the screen.
     */
     
    printf("\
\n\
The following details all the available command line options you can\n\
give to the program. Most of the option names have an alternate short\n\
abbreviation. The full option names may be specified by any prefix\n\
which distinguishes them from all other options (don't go so short as\n\
to exactly match the predefined abbreviation for another option!).\n");

    printf("\n\
Each option is shown with its full name, its abbreviation, an indicator to\n\
tell you if the option is required, information on the allowed limits (if any)\n\
on the option's argument (if any), the option's default value (if any) and a\n\
sometimes lengthy description of the option.\n\n\n");

    print_options(stdout, 0, counts[0]);
    
    exit(1);
}

VOID
print_options(fp, start, stop)
FILE *fp;
INT start;
INT stop;
{
    register INT i;

    for (i = start; i < stop; i++){
	
	fprintf(fp, "FULL OPTION NAME    : -%s\n", options[i].name);
	
	if (strlen(options[i].abbreviation) != (INT) 0){
	    fprintf(fp, "OPTION ABBREVIATION : -%s\n", options[i].abbreviation);
	}
	
	if (options[i].required == TRUE){
	    fprintf(fp, "REQUIRED OPTION     : yes\n");
	}
	
	if (options[i].has_lower_limit == TRUE){
	    
	    fprintf(fp, "LOWER LIMIT         : ");
	    
	    switch (options[i].type){
		case CHAR_OPT:{
		    fprintf(fp, "%c\n", (CHAR) options[i].lower_limit);
		    break;
		}
		
		case INT_OPT:{
		    fprintf(fp, "%d\n", (INT) options[i].lower_limit);
		    break;
		}
		
		case FLOAT_OPT:{
		    fprintf(fp, "%f\n", (FLOAT) options[i].lower_limit);
		    break;
		}
		
		case DOUBLE_OPT:{
		    fprintf(fp, "%f\n", (DOUBLE) options[i].lower_limit);
		    break;
		}
		
		case LONG_OPT:{
		    fprintf(fp, "%ld\n", (LONG) options[i].lower_limit);
		    break;
		}
		
		default:{
		    error("usage() found an option (%s) that was not supposed to have a lower bound.", 
			  options[i].name);
		}
	    }
	}
	
	if (options[i].has_upper_limit == TRUE){
	    
	    fprintf(fp, "UPPER LIMIT         : ");
	    
	    switch (options[i].type){
		case CHAR_OPT:{
		    fprintf(fp, "%c\n", (CHAR) options[i].upper_limit);
		    break;
		}
		
		case INT_OPT:{
		    fprintf(fp, "%d\n", (INT) options[i].upper_limit);
		    break;
		}
		
		case FLOAT_OPT:{
		    fprintf(fp, "%f\n", (FLOAT) options[i].upper_limit);
		    break;
		}
		
		case DOUBLE_OPT:{
		    fprintf(fp, "%f\n", (DOUBLE) options[i].upper_limit);
		    break;
		}
		
		case LONG_OPT:{
		    fprintf(fp, "%ld\n", (LONG) options[i].upper_limit);
		    break;
		}
		
		default:{
		    error("usage() found an option (%s) that was not supposed to have an upper bound.", 
			  options[i].name);
		}
	    }
	}
	
	if (options[i].explanation != (STRING) 0 && strlen(options[i].explanation) != (INT) 0){
	    fprintf(fp, "HELP FOR OPTION     :\n%s\n\n", options[i].explanation);
	}
	else {
	    fprintf(fp, "\n");
	}
    }
    
    return;
}
