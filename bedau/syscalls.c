#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/syscalls.c,v 1.4 1992/09/18 09:21:26 terry Exp $";
#endif


#include <stdio.h>
#include <stdlib.h>
#ifndef NeXT
#include <malloc.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "types.h"
#include "globals.h"

extern int fclose();
extern int fflush();
extern int unlink();
extern int rename();
extern int chdir();
extern int getwd();
extern int fseek();
extern int stat();
extern long ftell();
extern void error();
extern char *sys_errlist[];
extern int sys_nerr;
extern int errno;

#define WHY errno >= sys_nerr ? "Reason unknown" : sys_errlist[errno]

char *
Malloc(n)
int n;
{
    char *s;

	rcsid = rcsid;

    if (!(s = malloc(n))){
	error("could not malloc %d chars (%s).", n, WHY);
    }

    /* total_allocated += n; */

    return s;
}

FILE *
Fopen(file, mode)
char *file;
char *mode;
{
    FILE *fp, *fopen();

    if (!(fp = fopen(file, mode))){
	error("could not fopen '%s' with mode '%s' (%s).", file, mode, WHY);
    }

    return fp;
}

FILE *
Popen(file, mode)
char *file;
char *mode;
{
    FILE *fp, *popen();

    if (!(fp = popen(file, mode))){
	error("could not popen '%s' with mode '%s' (%s).", file, mode, WHY);
    }

    return fp;
}

void
Fflush(fp)
FILE *fp;
{
    if (fflush(fp) == EOF){
	error("could not fflush (%s).", WHY);
    }
    
    return;
}

void
Unlink(f)
char *f;
{
    if (unlink(f) == -1){
	error("could not unlink (%s).", WHY);
    }
    
    return;
}

void
Fclose(fp)
FILE *fp;
{
    if (fclose(fp) == EOF){
	error("could not fclose (%s).", WHY);
    }
    
    return;
}

void
Pclose(fp)
FILE *fp;
{
    if (pclose(fp) == EOF){
	error("could not pclose (%s).", WHY);
    }
    
    return;
}

void
Rename(file1, file2)
char *file1;
char *file2;
{
    if (rename(file1, file2) == -1){
	error("could not rename '%s' to '%s' (%s).", file1, file2, WHY);
    }

    return;
}


void
Getwd(dir)
char *dir;
{
    if (!getwd(dir)){
	error("could not get working directory (getwd says: '%s') (%s).", dir, WHY);
    }
    
    return;
}
	
void
Chdir(dir)
char *dir;
{
    if (chdir(dir) == -1){
	error("could not change directory to '%s' (%s).\n", dir, WHY);
    }

    return;
}

void
Fseek(fp, offset, ptr)
FILE *fp;
long offset;
int ptr;
{
    if (fseek(fp, offset, ptr) == -1){
	error("could not fseek (%s).\n", WHY);
    }

    return;
}

void
Stat(path, buf)
char *path;
struct stat *buf;
{
    if (stat(path, buf) == -1){
	error("could not stat '%s' (%s).\n", path, WHY);
    }
    
    return;
}

long
Ftell(fp)
FILE *fp;
{
    long ret = ftell(fp);

    if (ret == -1L){
	error("could not ftell (%s).\n", WHY);
    }

    return ret;
}

void
Free(ptr)
void *ptr;
{
    if (!ptr){
	return;
    }

    free(ptr);
}
