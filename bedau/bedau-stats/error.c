#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/s/me/bedau/RCS/error.c,v 1.1 1994/03/29 05:30:48 terry Exp terry $";
#endif


#include <stdio.h>
#include "types.h"

/*
 * error()
 *
 * Error message printer. The argument a should be a format string that
 * can be printed using fprintf(). The message is printed preceded by the
 * name we were invoked with and succeeded by a newline.
 *
 */
/* VARARGS2 */
VOID
error(u, v, w, x, y, z)
char *u, *v, *w, *x, *y, *z;
{
    extern void perror();

    fprintf(stderr, "%s: ", myname);
    fprintf(stderr, u, v, w, x, y, z);
    fprintf(stderr, "\n");
    if (fflush(stderr) == EOF){
        perror("fflush");
    }
    if (sysdie == TRUE){
	exit(1);
    }
    return;
}

