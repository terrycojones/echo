/*
 * Stuff for the processing of options follows.
 *
 * Each application may define its own options in a similar fashion
 * (using the app_options variable). The columns represent
 *
 *       1)   The long form of the name of the option.
 *       2)   An abbreviated name for the option (if desired).
 *       3)   The type of the option (see options.h).
 *       4)   Whether the option is mandatory.
 *       5)   Is there a lower bound on this option?
 *       6)   If so, what is the lower bound?
 *       7)   Is there an upper bound on this option?
 *       8)   If so, what is the upper bound?
 *       9)   The address of something to operate on when the option 
 *            is found (the action is determined by the option type).
 *      10)   A USEFUL help message.
 *       
 * There are some defines in options.h that make things more readable.
 * In particular, the LIMIT defines collapse columns 5 to 8 into one.
 */


static OPTION main_options[] = {

    /* OPTION NAME               ABBREV  TYPE         REQD   LIMITS */
    
    { "bucket_size",                      "bs", DOUBLE_OPT,   FALSE, LLIMIT(0.0),
       VADDR(bucket_size),

"The size of the buckets to group frequency data into. This does not\n\
have to be integral.\n\
DEFAULT VALUE : 1.0"
},

    { "check_generations",                     "cg", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
 VADDR(check_generations),

"Perform checking of input generation numbers. If the generation numbers\n\
found in the input are not in order, the program will complain.\n\
DEFAULT VALUE : on"
},

    { "data_cumulative",                     "dc", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
 VADDR(data_cumulative),

"If TRUE, assumes that the usage counts on input are cumulative. That is, they\n\
have already been incremented through time. Otherwise the usage counts are\n\
taken to be the actual usages that occurred at the current generation\n\
DEFAULT VALUE : true"
},

    { "first_generation",                      "fg", INT_OPT,   FALSE, LLIMIT(0.0),
       VADDR(first_generation),

"The number of the first generation in the input. This is not the same as\n\
the -from option, which indicates when to start analysing the data. This\n\
option merely indicates what the first generation in the input file is \n\
numbered and hence is purely syntactical. It is designed to make it simpler\n\
to process data that does not start at generation 1 (the default)."
},

    { "from",                      "", INT_OPT,   FALSE, LLIMIT(0.0),
       VADDR(start_generation),

"The number of the first generation to produce data for.\n\
DEFAULT VALUE : 1"
},

    { "generation_gap",                      "gg", INT_OPT,   FALSE, LLIMIT(0.0),
       VADDR(generation_gap),

"The number of generations between succesive lines on the input. This\n\
option allows the checking of generation numbers when the input does\n\
not consist of all the original generations. For instance, you may\n\
have already removed every second generation before the program sees\n\
the data. You can use this option to set the interval.\n\
DEFAULT VALUE : 1"
},

    { "generations",                      "g", INT_OPT,   FALSE, LLIMIT(0.0),
       VADDR(ngenerations),

"The number of generations of data that are to be output. This does\n\
not normally need to be specified, since the program will by default\n\
print information for every generation it finds on the input. It is\n\
useful for telling the program in advance how many generations there\n\
will be so that it can produce output for ssplot (see the -ssplot\n\
option for more information)."
},

    { "hash_size",                      "hs", LONG_OPT,   FALSE, LLIMIT(3.0),
       VADDR(hash_size),

"The size of the hash table that stores agent's names.\n\
DEFAULT VALUE : 97"
},

    { "help",                      "he", FUNC0_OPT,   FALSE, NO_LIMITS(),
       (VOID *)usage,

"Print a help message describing all options and their effects." 
},

    { "nbuckets",                      "", INT_OPT,   FALSE, LLIMIT(1.0),
       VADDR(nbuckets),

"The maximum number of buckets to group data into."
},

    { "options",                   "op", FUNC1_OPT,   FALSE, NO_LIMITS(),
       (VOID *)read_opts_from_file,

"Specify, with the next argument, a file containing more options.\n\
Options in files should be specified one per line in the format\n\
\"option-name : option-value\" (there may not be any value if the\n\
option does not take an argument. The option names and abbreviations\n\
are the same as the command line option names. Files of options can\n\
specify other files of options, which is very useful if you are doing\n\
a number of experiments in which many options remain the same - they\n\
can each have an experiment-specific options file that includes a file\n\
of options common to all the experiments.\n\
\n\
Options are processed left to right as they are found on the command\n\
line, and the given value for an option will reset the former value.\n\
This can be used to override options set in a default common option file.\n\
\n\
Boolean options may be given with no argument (and the colon may be\n\
omitted too), in which case their value will be toggled from TRUE to\n\
FALSE, or vice-versa, each time they are encountered. Alternately, they\n\
may be given arguments such as \"true\" or \"false\", or \"yes\" or \"no\",\n\
or \"on\" or \"off\", or even \"1\" or \"0\" - take your pick.\n\
\n\
Blank lines or lines starting with '#' in options files will be ignored,\n\
so you can comment your option files." 
},

    { "period",                      "", INT_OPT,   FALSE, LLIMIT(1.0),
       VADDR(period),

"The reporting interval. Every 'period' generations, the collected\n\
data will be averaged and reported.\n\
DEFAULT VALUE : 1"
},

    { "ssplot",                     "", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
 VADDR(ssplot),

"Make the output have a format suitable for input to the ssplot\n\
program. This means that the first three lines will look like:\n\
== DATA ==\n
M1\n
COLS ROWS\n
where COLS and ROWS are the number of buckets and generations\n\
respectively. The following data values will be separated by single\n\
TABs. This option is off by default, and can only be used if the\n\
number of generations to be printed is given with the -generations\n\
option."
},

    { "to",                      "", INT_OPT,   FALSE, LLIMIT(0.0),
       VADDR(stop_generation),

"The number of the last generation to produce data for.\n\
DEFAULT VALUE : after the last generation."
},

    { "truncate",                     "", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
 VADDR(truncate),

"Truncate when the required number of buckets exceeds the current\n\
limit. If the data would fall into a bucket that does not exist,\n\
it is discarded. If this option is disabled, the number of buckets\n\
is doubled.\n\
DEFAULT VALUE : TRUE"
},

    { "usage",                     "us", FUNC0_OPT,   FALSE, NO_LIMITS(),
 (VOID *)usage,

"Print a help message describing all options and their effects." 
},

    { "verbose",                     "", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
 VADDR(verbose),

"Show information about what's going on. This option can only be used\n\
if the code was compiled with -DDEBUG=1 in the Makefile."
},


    { "?",                          "",  FUNC0_OPT,   FALSE, NO_LIMITS(),
 (VOID *)usage,

"Print a help message describing all options and their effects." 
},

    /* END OF OPTIONS MARKER, DO NOT REMOVE! */
    { (STRING) 0,                   "",  0,           FALSE, NO_LIMITS(),        (VOID *) 0, "" }
};

/* Undefine the things in options.h - just in case... */
#undef NO_LIMITS
#undef ULIMIT
#undef LLIMIT
#undef LIMITS
