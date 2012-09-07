/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/chromosomes.h,v 1.2 1992/09/06 06:30:59 terry Exp $ */

extern CHROMOSOME *create_chromosome();
extern void grow_chromosome();
#ifndef NO_MUTATION
extern void mutate_chromosome();
#endif
extern void set_chromosome();
extern void swap_chromosomes();
extern int chromosome_distance();
extern int chromosome_string_distance();

/* I leave these as lower case since it's safe to think of them as functions. */
#define copy_chromosome(chromosome)           create_chromosome((chromosome))
#define new_chromosome()                      create_chromosome((CHROMOSOME *)0)

