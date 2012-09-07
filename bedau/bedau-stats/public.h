/* $Header: /disks/disk1/Users/terry/s/me/bedau/RCS/public.h,v 1.1 1994/03/29 05:30:56 terry Exp terry $ */
#if defined(ultrix) || defined(NeXT)
extern char *strdup();
#else
extern int _flsbuf();
extern int fflush();
extern int printf();
extern int fprintf();
#ifdef mips
extern int sprintf();
#endif
#endif

#if defined(sparc) || defined(mc68020)
extern int printf();
extern int fprintf();
#endif

extern VOID error();
extern VOID usage();

#define VADDR(x) ((VOID *) (&(x)))
#define BUF_SZ             8192

