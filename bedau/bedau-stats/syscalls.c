#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/s/me/bedau/RCS/syscalls.c,v 1.1 1994/03/29 05:30:57 terry Exp terry $";
#endif


#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#ifndef NeXT
#include <malloc.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

extern int fclose();
extern int fflush();
extern int unlink();
extern void error();
extern char *sys_errlist[];
extern int sys_nerr;
extern int errno;

#define WHY errno >= sys_nerr ? "Reason unknown" : sys_errlist[errno]

STRING
Malloc(n)
INT n;
{
    char *s;

    if (!(s = malloc(n))){
	error("could not malloc %d chars. (%s)", n, WHY);
    }
    
    return (STRING) s;
}

FILE *
Fopen(file, mode)
STRING file;
STRING mode;
{
    FILE *fp, *fopen();

    if (!(fp = fopen(file, mode))){
	error("could not open '%s' with mode '%s'. (%s)", file, mode, WHY);
    }

    return fp;
}

FILE *
Popen(file, mode)
STRING file;
STRING mode;
{
    FILE *fp, *popen();

    if (!(fp = popen(file, mode))){
	error("could not popen '%s' with mode '%s'. (%s)", file, mode, WHY);
    }

    return fp;
}

VOID
Fflush(fp)
FILE *fp;
{
    if (fflush(fp) == EOF){
	error("could not fflush. (%s)", WHY);
    }
    
    return;
}

VOID
Unlink(f)
STRING f;
{
    if (unlink(f) == -1){
	error("could not unlink. (%s)", WHY);
    }
    
    return;
}

VOID
Fclose(fp)
FILE *fp;
{
    if (fclose(fp) == EOF){
	error("could not fclose. (%s)", WHY);
    }
    
    return;
}

VOID
Free(ptr)
void *ptr;
{
    if (!ptr){
	return;
    }

    free(ptr);
}
