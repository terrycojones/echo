/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/defs.h,v 1.2 1992/09/15 13:35:53 terry Exp $ */
extern int printf();
extern int fprintf();
extern int fread();
extern int _flsbuf();

#if defined(ultrix) || defined(NeXT)
extern char *strdup();
#endif
