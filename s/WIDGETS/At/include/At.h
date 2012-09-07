/*
 *      At.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  At.h"
 */

/*

Copyright 1992 by University of Paderborn

All rights reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of the firms, institutes
or employers of the authors not be used in advertising or publicity
pertaining to distribution of the software without specific, written
prior permission.

THE AUTHORS AND THEIR FIRMS, INSTITUTES OR EMPLOYERS DISCLAIM ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE AUTHORS AND THEIR FIRMS,
INSTITUTES OR EMPLOYERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/

/*
 *   Common include file for all At Plotter files
 *
 */

#ifndef _At_h
#define _At_h

#include <stdio.h>

#ifdef __STDC__
# include <stdlib.h>
#endif

#include <math.h>
#include <assert.h>

#if  defined(SYSV) || defined(SVR4) || defined(VMS)
# include <string.h>
#else
# include <strings.h>
#endif

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>

#ifndef XtPointer
# define XtPointer caddr_t
#endif

#ifndef P
# ifdef __STDC__
#  define P(args) args
# else
#  define P(args) ()
# endif
#endif

#ifndef Min
# define Min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef Max
# define Max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef HUGE_VAL
# define HUGE_VAL        3.40282346638528860e+38
#endif

/* Default fonts */

#ifndef DFLT_FONTFAMILY
# define DFLT_FONTFAMILY "new century schoolbook"
#endif

#ifdef USE_SMALLER_FONTS
# define DFLT_FONTBIG    AtFontNORMAL
# define DFLT_FONTNORMAL AtFontSMALL
# define DFLT_FONTSMALL  AtFontSMALLEST
#else
# define DFLT_FONTBIG    AtFontBIG
# define DFLT_FONTNORMAL AtFontNORMAL
# define DFLT_FONTSMALL  AtFontSMALL
#endif

#define LIN_MIN 0.0
#define LIN_MAX 1.0
#define LOG_MIN 0.1
#define LOG_MAX 1.0
#define TIC_INT 1.0

extern int strcasecmp  P((const char *, const char *));
extern int strncasecmp P((const char *, const char *, int));

#endif /* _At_h */
