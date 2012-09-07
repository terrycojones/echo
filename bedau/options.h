/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/options.h,v 1.2 1992/09/18 09:21:26 terry Exp $ */

#define VADDR(x) ((void *) (&(x)))

#define NO_LIMITS() FALSE, ((double)0.0), FALSE, ((double)0.0)
#define ULIMIT(x)   FALSE, ((double)0.0), TRUE,  ((double)(x))
#define LLIMIT(x)   TRUE,  ((double)(x)), FALSE, ((double)0.0)
#define LIMITS(x,y) TRUE,  ((double)(x)), TRUE,  ((double)(y))

extern int process_options();
extern int get_options();
extern void check_options();
extern void read_opts_from_file();
extern void usage();
