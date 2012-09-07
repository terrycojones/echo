#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/random.c,v 1.5 1992/11/16 07:05:45 terry Exp terry $";
#endif

#include <stdio.h>

#include "types.h"
#include "globals.h"

/*
 * This is modified from the algorithm given in Numerical Recipes in C.
 * It is their algorithm ran3, which was based on Knuth's subtractive
 * method on pages 171-172 of his volume II (NOT section 3.2-3.3 as is
 * mentioned in Numerical Recipes).
 *
 * As far as I can figure out, this returns a value on [0, 1).
 *
 * I split the function into 2 functions - a seeder and a
 * generator. The generator DOES NOT check to see if the seeder has been
 * called. I chose to do this as it makes the generator faster by one if 
 * statement per call (which adds up over a million calls). Of course 
 * it also leaves a potential hole. There is no easy way to cause the 
 * generator to dump core (i.e. by initializing an array index to something
 * ridiculous) if it has not been seeded. The contents of ma will be
 * unpredictable & who knows what behavior you'll get from the generator.
 * (Later I found that Knuth's original was also not a single function.)
 *
 * There was no need to pass a pointer to the seed value - the algorithm in 
 * Numerical Recipes sets it to 1 after initialization. This was done because
 * their function was too literally translated from Knuth's Fortran, and 
 * Fortran functions must set the variable that corresponds to their name
 * to some value. In C this is of course unnecessary since our function 
 * returns void.
 *
 * I removed the definition of MZ (which was just zero).
 *
 * The variable iff is also gone, since it was used to tell if the seeder
 * had already been called - now we don't check for that.
 *
 * Calling the seeder with a value < 0 will cause it to seed itself from
 * the system clock. I hope Knuth got this right because the system clock
 * is currently returning values that are bigger than MSEED, which means
 * that mj (and hence ma[55]) will be set to a negative value in the seeder
 * if no seed is supplied. Actually the Numerical Recipes people seem to
 * have screwed up on this - Knuth specifies that the seed should be
 * in the range 0 to MBIG - 1, and sets ma[55] to that value. These people
 * do
 *
 *  mj = MSEED - seed;
 *  mj %= MBIG;
 *  ma[55] = mj;
 *
 * And as a result, if seed > MSEED, ma[55] will be negative, since the
 * modulus function will also return something < 0. This, I would say, is
 * a bug. The whole of this translation seems awkward, these people don't
 * appear to be good C programmers. I have taken Knuth's Fortran over
 * the N.R. C for this small section, and I have added a check to ensure
 * that the seed is actually < MBIG, as Knuth wants.
 *
 * The seeder returns the value of the seed used.
 *
 * The N.R. people "warm up" their generator 4 times, whilst Knuth
 * does it 3 times. I followed Knuth.
 *
 * The #define of FAC looks like it could be taken out and the return
 * statement replaced with return mj / MBIG, but in fact this would slow
 * things down - you'd do a long division every time. Instead we let the
 * compiler (hopefully) do it once and thereafter we do a long multiplication.
 *
 * Another thing that looks like it could be improved is the indexing of
 * the ma array. The zeroeth value isn't used at all. Typical Fortran
 * stuff. I was going to change all this, but the loop index (running from
 * 1 to 54) is at one point used in a calculation, so I decided not to worry.
 *
 * There is a function dump_random_state() which dumps out the state of the
 * random number generator into a FILE *. The complement function 
 * restore_random_state() will read in a state from a FILE * and set up
 * the generator's state so that a run may be resumed. This stuff will 
 * only be compiled if you #define DUMPS (this is principally to save
 * a compile of stdio.h if you are not interested in dumping/restoring).
 *
 * I took the static variables outside any function so that they could
 * be accessed by all functions in this file.
 *
 * Terry Jones (terry@santafe.edu)
 * July 3, 1992.
 *
 */

#include <sys/time.h>

#define MBIG  1000000000
#define MSEED 161803398
#define FAC   (1.0 / MBIG)

static int inext;
static int inextp;
static long ma[56];

PROBABILITY
knuth_random()
{
    long mj;
    
    if (++inext == 56){
	inext = 1;
    }
    
    if (++inextp == 56){
	inextp = 1;
    }

    mj = ma[inext] - ma[inextp];
    
    if (mj < 0){
	mj += MBIG;
    }

    ma[inext] = mj;
    return mj * FAC;
}

long
seed_random(seed)
long seed;
{
    long mj;
    long mk;
    register int i;
    register int k;

	rcsid = rcsid;
	
    if (seed < 0){
	extern int gettimeofday();

	struct timeval tp;
	if (gettimeofday(&tp, (struct timezone *)0) == -1){
	    error("Could not gettimeofday in knuth_srand().");
	}
	
	seed = tp.tv_sec;
    }
    
    if (seed >= MBIG){
	error("Seed value too big (> %d) in knuth_srand().", MBIG);
    }

    ma[55] = mj = seed;
    mk = 1;
    
    for (i = 1; i <= 54; i++){
	register int ii = (21 * i) % 55;
	ma[ii] = mk;
	mk = mj - mk;
	if (mk < 0){
	    mk += MBIG;
	}
	mj = ma[ii];
    }
    
    for (k = 0; k < 4; k++){
	for (i = 1; i <= 55; i++){
	    ma[i] -= ma[1 + (i + 30) % 55];
	    if (ma[i] < 0){
		ma[i] += MBIG;
	    }
	}
    }
    
    inext = 0;
    inextp = 31;
    
    return seed;
}

#ifdef DUMPS
#include <stdio.h>

void
dump_random_state(fp)
FILE *fp;
{
    register int i;

    if (!fp){
	error("dump_random_state() caled with NULL file pointer.");
    }

    fprintf(fp, "%d\n%d\n", inext, inextp);
    
    for (i = 0; i < 56; i++){
	fprintf(fp, "%d\n", ma[i]);
    } 
    
    return;
}

#define readline if (!(fgets(line, 1024, fp))) error("fgets fails in app_restore_state.")

void
restore_random_state(fp)
FILE *fp;
{
    extern int atoi();
    char line[1024];
    register int i;
    
    if (!fp){
	error("restore_random_state() caled with NULL file pointer.");
    }

    readline; 
    inext = atoi(line);
    readline; 
    inextp = atoi(line);
    
    for (i = 0; i < 56; i++){
	readline; 
	ma[i] = atoi(line);
    }

    return;
}
#endif /* DUMPS */


/* Functions to catch people using other random number generators. */
double
drand48()
{
    error("drand48() should not be used - use knuth_random instead!");
    return 0.0; /* To keep gcc -Wall quiet. */
}

void
srand48()
{
    error("srand48() should not be used - see the README file for help.");
}

int
rand()
{
    error("drand() should not be used - use knuth_random instead!");
    return 0; /* To keep gcc -Wall quiet. */
}

int
srand()
{
    error("srand() should not be used - see the README file for help.");
    return 0; /* To keep gcc -Wall quiet. */
}

long
random()
{
    error("random() should not be used - use uniform() instead!");
    return 0L; /* To keep gcc -Wall quiet. */
}

void
srandom()
{
    error("srandom() should not be used - see the README file for help.");
}

STRING
initstate(seed, state, n)
unsigned seed;
STRING state;
int n;
{
    error("initstate() should not be used - see the README file for help.");
    return (STRING) 0; /* To keep gcc -Wall quiet. */
}

STRING
setstate(state)
STRING state;
{
    error("setstate() should not be used - see the README file for help.");
    return (STRING) 0; /* To keep gcc -Wall quiet. */
}
