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

/* This one is defined here as it's not useful to applications. */

#define VERBOSE_USAGE

static OPTION main_options[] = {

    /* OPTION NAME               ABBREV  TYPE         REQD   LIMITS */
    
    { "dump-genes",                   "dg",  STRING_OPT, FALSE, NO_LIMITS(),
 VADDR(genes_to_dump),

#if defined(VERBOSE_USAGE)
"Give the list of genes to dump as a single argument. E.g. -dg 025" 
#else
""
#endif
},

    { "dump-species",                "ds", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(dump_species),

#if defined(VERBOSE_USAGE)
"Whether or not to dump out species data."
#else
""
#endif
},
    
    { "generations",               "g",  INT_OPT,     FALSE,  LLIMIT(1.0),
	  VADDR(ngenerations),

#if defined(VERBOSE_USAGE)
"Specify, with the next argument, the number of generations to run.\n\
This is more like the maximum number of generations to run if you\n\
have asked the program to stop at some maximum fitness value." 
#else
""
#endif
},
    
    { "graphics",                "gr", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(graphics),

#if defined(VERBOSE_USAGE)
"Whether or not to display graphics."
#else
""
#endif
},
    
    { "hash_size",               "hs",  INT_OPT,     FALSE,  LLIMIT(1.0),
	  VADDR(genome_hash_size),

#if defined(VERBOSE_USAGE)
"The size of the hash table to store genomes in. Default is 199."
#else
""
#endif
},
    


    { "help",                      "he", FUNC0_OPT,   FALSE, NO_LIMITS(),
       (void *)usage,

#if defined(VERBOSE_USAGE)
"Print a help message describing all options and their effects." 
#else
""
#endif
},


    { "options",                   "op", FUNC1_OPT,   FALSE, NO_LIMITS(),
       (void *)read_opts_from_file,

#if defined(VERBOSE_USAGE)
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
#else
""
#endif
},

    { "runs",               "r",  INT_OPT,     FALSE,  LLIMIT(1.0),
	  VADDR(runs),

#if defined(VERBOSE_USAGE)
"The number of runs to do."
#else
""
#endif
},
    
    { "seed",                      "s",  LONG_OPT,    FALSE, LLIMIT(0.0),
	  VADDR(seed),

#if defined(VERBOSE_USAGE)
"Specify, with the next argument, a seed for the random number\n\
generator. This can be used to repeat a run or to repeat and extend\n\
a run for a longer time if you specify more generations." 
#else
""
#endif
},

    { "self-replication",                "", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(self_replication),

#if defined(VERBOSE_USAGE)
"Whether or not self-replication is enabled."
#else
""
#endif
},

    { "sex",                "", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(sex),

#if defined(VERBOSE_USAGE)
"Whether or not sex (via two-point crossover) is enabled."
#else
""
#endif
},

    /* BEDAU: */
    { "sum-over-genes",                "sog", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(sum_over_genes),

#if defined(VERBOSE_USAGE)
"Whether or not to consider agents individually when computing\n\
the total usage of a gene. If this is off (the default), then\n\
the output of the -dg option may contain a line like:\n\
10 20 365.abc 7 366.def 8 367.abc 15\n\
The numbers 365 to 367 are just agent numbers. The output if\n\
the option is enabled would look like:\n\
10 20 abc 12 def 8\n\
(the two agents who had abc for that gene have been coalesced)."
#else
""
#endif
},


    /* BEDAU: */
    { "update-on-testing",                "uot", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(update_on_testing),

#if defined(VERBOSE_USAGE)
"If true (the default) makes usage counts update when genes are tested."
#else
""
#endif
},

    /* BEDAU: */
    { "update-on-success",                "uos", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(update_on_success),

#if defined(VERBOSE_USAGE)
"If true (NOT the default) makes usage counts update when genes are\n\
tested and a match occurs."
#else
""
#endif
},

    /* BEDAU: */
    { "update-combat",                "uc", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(update_combat),

#if defined(VERBOSE_USAGE)
"If true (the default) makes combat usage counts update."
#else
""
#endif
},

    /* BEDAU: */
    { "update-mating",                "um", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(update_mating),

#if defined(VERBOSE_USAGE)
"If true (the default) makes mating usage counts update."
#else
""
#endif
},

    /* BEDAU: */
    { "update-trading",                "ut", BOOLEAN_OPT,   FALSE, NO_LIMITS(),
	  VADDR(update_trading),

#if defined(VERBOSE_USAGE)
"If true (the default) makes trading usage counts update."
#else
""
#endif
},

    { "usage",                     "us", FUNC0_OPT,   FALSE, NO_LIMITS(),
 (void *)usage,

#if defined(VERBOSE_USAGE)
"Print a help message describing all options and their effects." 
#else
""
#endif
},


    { "verbose",                   "v",  STRING_OPT, FALSE, NO_LIMITS(),
 VADDR(verbose),

#if defined(VERBOSE_USAGE)
"" 
#else
""
#endif
},

    
    { "version",                   "",   FUNC0_OPT,   FALSE, NO_LIMITS(),
 (void *)version,

#if defined(VERBOSE_USAGE)
"" 
#else
""
#endif
},

    
    { "world",                   "",   STRING_OPT,   FALSE, NO_LIMITS(),
	  VADDR(world_file),

#if defined(VERBOSE_USAGE)
"Name a world file to run."
#else
""
#endif
},

    
    { "?",                          "",  FUNC0_OPT,   FALSE, NO_LIMITS(),
 (void *)usage,

#if defined(VERBOSE_USAGE)
"Print a help message describing all options and their effects." 
#else
""
#endif
},

    
    /* END OF OPTIONS MARKER, DO NOT REMOVE! */
    { (STRING) 0,                   "",  BOOLEAN_OPT,  FALSE, NO_LIMITS(), (void *) 0, "" }
};

/* Undefine the things in options.h - just in case... */
#undef VADDR
#undef NO_LIMITS
#undef ULIMIT
#undef LLIMIT
#undef LIMITS

