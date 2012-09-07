/* $Header: /disks/disk1/Users/terry/s/me/bedau/RCS/options.h,v 1.1 1994/03/29 05:30:55 terry Exp terry $ */
#define NO_LIMITS() FALSE, ((DOUBLE)0.0), FALSE, ((DOUBLE)0.0)
#define ULIMIT(x)   FALSE, ((DOUBLE)0.0), TRUE,  ((DOUBLE)(x))
#define LLIMIT(x)   TRUE,  ((DOUBLE)(x)), FALSE, ((DOUBLE)0.0)
#define LIMITS(x,y) TRUE,  ((DOUBLE)(x)), TRUE,  ((DOUBLE)(y))

extern INT process_options();
extern INT get_options();
extern VOID check_options();
extern VOID check_for_conflicting_options();
extern VOID read_opts_from_file();
