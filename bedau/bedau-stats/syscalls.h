/* $Header: /disks/disk1/Users/terry/s/me/bedau/RCS/syscalls.h,v 1.1 1994/03/29 05:30:58 terry Exp terry $ */
#include <stdio.h>

extern FILE *Fopen();
extern FILE *Popen();
extern VOID Fclose();
extern VOID Fflush();
extern VOID Unlink();
extern STRING Malloc();
extern INT Stat();
extern INT Access();
extern VOID Free();

