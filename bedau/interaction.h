/* $Header: /disks/disk1/Users/terry/r/echo/s.orig/RCS/interaction.h,v 1.3 1994/02/07 23:16:05 terry Exp terry $ */

#ifndef NO_COMBAT
/*
 * This is used in the combat points calculation. We figure out
 * the ratio of the lengths of the combat tags and if the second
 * agent has length 0, we use this as the denominator. A value
 * less than one discourages zero length defense tags.
 */
#define SMALLEST_LENGTH_RATIO_DENOM   0.3

extern BOOLEAN will_attack();
#endif

#ifndef NO_TRADE
extern BOOLEAN will_trade();
#endif

#ifndef NO_SEX
extern BOOLEAN will_mate();
#endif

#ifdef BUGGY_COMBAT_CALCULATION
extern RESOURCE_SIZE combat_payoff();
#else
extern double combat_payoff();
#endif

extern void interact();
