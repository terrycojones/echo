#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/error.c,v 1.3 1992/09/15 13:35:53 terry Exp $";
#endif


#include <stdio.h>
#include "types.h"
#include "globals.h"

/*
 * error()
 *
 * Error message printer. The argument a should be a format string that
 * can be printed using fprintf(). The message is printed preceded by the
 * name we were invoked with and succeeded by a newline.
 *
 */
/* VARARGS2 */
void
error(u, v, w, x, y, z)
char *u, *v, *w, *x, *y, *z;
{
    extern void perror();
    extern int fflush();

    rcsid = rcsid;

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

