/* $Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/random.h,v 1.4 1992/10/22 04:14:17 terry Exp $ */

#define NO_SEED (-1L)

/*
 * Some macros to access the random number generator in a friendly
 * fashion.
 */

extern PROBABILITY knuth_random();
extern long seed_random();

/* 
 * Return [0, n - 1]
 */
#define uniform(n)             ((int) (knuth_random() * (double)(n)))

/* 
 * Returns true if a random double is less than the given probability. 
 */
#define biased_coin_flip(prob) (knuth_random() < (prob))

/*
 * Return TRUE or FALSE with equal probability.
 */
#define random_boolean() (uniform(2) ? TRUE : FALSE)

/*
 * This is to catch other random number generators that may be called.
 * The definitions of these are in random.c and all cause an error.
 */
extern double drand48();
extern void srand48();
extern int rand();
extern int srand();
extern long random();
extern void srandom();
extern char *initstate();
extern char *setstate();
