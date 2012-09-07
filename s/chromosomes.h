/* $Header: /disks/disk1/Users/terry/r/echo/s.orig/RCS/chromosomes.h,v 1.4 1994/02/07 23:16:01 terry Exp terry $ */

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

