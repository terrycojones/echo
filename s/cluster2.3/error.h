/*
 * error.h -- error handling defines for cluster
 *
 * $Header: error.h,v 1.5 91/07/14 01:10:50 stolcke Exp Locker: stolcke $
 * $Log:	error.h,v $
 * Revision 1.5  91/07/14  01:10:50  stolcke
 * curses support added
 * graphing routines moved to separate file
 * 
 * Revision 1.4  91/04/20  16:18:10  stolcke
 * second release (beta)
 * 
 * Revision 1.3  91/04/18  18:28:24  stolcke
 * general cleanup and partial rewrite
 * 
 * Revision 1.2  91/04/18  13:29:32  stolcke
 * merged pca into cluster
 *
 */

#define Nmsg 200		/* error msg can be fairly long */
char    ERR_MSG[Nmsg];
int     ERR_FLAG;

#define MY_OK	1		/* avoid conflict with curses result code */
#define MY_ERR	0

/* error handling macros */
#define Erreturn(msg) { \
	(void)strcpy(ERR_MSG, msg); \
	return MY_ERR; \
    }

#define Erreturn1(msg, x) { \
	(void)sprintf(ERR_MSG, msg, x); \
	return MY_ERR; \
    }

#define Erreturn2(msg, x, y) { \
	(void)sprintf(ERR_MSG, msg, x, y); \
	return MY_ERR; \
    }

#define Erreturn3(msg, x, y, z) { \
	(void)sprintf(ERR_MSG, msg, x, y, z); \
	return MY_ERR; \
    }

#define IfErr(x) 	if ((x) == MY_ERR)
#define IfEOF(x)	if ((x) == EOF)

/*
 * macros handling D/C values
 */
#ifndef NO_DONTCARES
#define DC_VAL		((FLOAT)HUGE)
#define IS_DC(x)	(!finite(x))
#endif

