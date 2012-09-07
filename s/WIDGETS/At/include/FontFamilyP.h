/*
 *      FontFamilyP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Typos fixed.
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  FontFamilyP.h"
 */

/*

Copyright 1990,1991 by the Massachusetts Institute of Technology

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

#ifndef _At_FontFamilyP_h
#define _At_FontFamilyP_h

#include "At.h"
#include "FontFamily.h"

/* Font families */

#define AtFontCOURIER    0
#define AtFontHELVETICA  1
#define AtFontTIMES      2
#define AtFontSCHOOLBOOK 3
#define AtFontSYMBOL     4

typedef short PostscriptMetrics[256];

typedef struct {
    char                 *xname;
    char                 *psnames[4];
    int                  flags;
    char                 slantchar;
    PostscriptMetrics    *psmetrics;
    short                psascents[4];
    short                psdescents[4];
} AtFontFamilyInfo;

extern AtFontFamilyInfo _AtFontFamilies[];

#define XNAME(ff) (_AtFontFamilies[(ff)->num].xname)
#define PSNAME(ff,style) (_AtFontFamilies[(ff)->num].psnames[style])
#define FLAGS(ff) (_AtFontFamilies[(ff)->num].flags)
#define SLANTCHAR(ff) (_AtFontFamilies[(ff)->num].slantchar)
#define PSMETRICS(ff) (_AtFontFamilies[(ff)->num].psmetrics)
#define INFO(ff) (&_AtFontFamilies[(ff)->num])

#ifndef AFMPATH
# define AFMPATH "/usr/athena/lib/ps"
#endif

#endif /* _At_FontFamilyP_h */
