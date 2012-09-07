/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/syscalls.h,v 1.3 1992/09/18 09:21:26 terry Exp $ */
#ifndef FILE
#include <stdio.h>
#endif

extern char *Malloc();
extern FILE *Fopen();
extern void Fflush();
extern void Unlink();
extern void Fclose();
extern void Rename();
extern void Getwd();
extern void Chdir();
extern void Fseek();
extern void Stat();
extern long Ftell();
extern void Free();
