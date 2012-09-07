/*
 *      FontFamily.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sun Aug  2 18:24:12 1992, patchlevel 3
 *                                      Define all font resources and
 *                                      classes
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  FontFamily.h"
 */

/*

Copyright 1992 by University of Paderborn
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

#ifndef _AtFontFamily_h
#define _AtFontFamily_h

#include "At.h"

/* Type faces */
#define AtFontPLAIN      0
#define AtFontBOLD       1    /* Set and clear these two bits to get */
#define AtFontITALIC     2    /*   the 4 possible typefaces */
#define AtFontBOLDITALIC 3

/* Point sizes */
#define AtFontSMALLEST   0
#define AtFontSMALL      1
#define AtFontMEDIUM     2
#define AtFontNORMAL     3
#define AtFontBIG        4
#define AtFontBIGGEST    5

typedef struct _AtFontFamily {
     Display *dpy;
     short refcount;
     int num;
     XFontStruct **cache;
     struct _AtFontFamily *next;
} AtFontFamily;

extern AtFontFamily *AtFontFamilyGet P((Display *, char *));
extern void AtFontFamilyRelease P((AtFontFamily *));
extern XFontStruct *AtFontFetch P((AtFontFamily *, int, int));
extern int AtFontPointSize P((AtFontFamily *, int));
extern int AtFontPixelSize P((AtFontFamily *, int));
extern int AtFontStringToSize P((char *));
extern void AtRegisterFontSizeConverter P((void));
extern void AtRegisterFontStyleConverter P((void));

extern char *AtFontFamilyGetName P((AtFontFamily *));
#define AtFontFamilyGetDisplay(family) ((family)->dpy)
#define AtFontBigger(size) ((size<AtFontBIGGEST) ? (size+1) : (size))
#define AtFontSmaller(size) ((size>AtFontSMALLEST) ? (size-1) : (size))
#define AtFontEmphasize(face) (face | AtFontBOLD)
#define AtFontDeemphasize(face) (face & ~AtFontBOLD)
#define AtFontItalicize(face) (face | AtFontITALIC)
#define AtFontDeitalicize(face) (face & ~AtFontITALIC)
extern void AtFontSetStyle P((AtFontFamily *, int));

extern int AtFontPSTextWidth P((AtFontFamily *, int, int, char *, int));
extern int AtFontPSAscent P((AtFontFamily *, int, int));
extern int AtFontPSDescent P((AtFontFamily *, int, int));
extern char *AtFontPSName P((AtFontFamily *, int));

/* Resource names, classes and types for use with the X Toolkit */
#define XtNfontFamily    "fontFamily"
#define XtNfontSize      "fontSize"
#define XtNfontStyle     "fontStyle"

#define XtCFontFamily    "FontFamily"
#define XtCFontSize      "FontSize"
#define XtCFontStyle     "FontStyle"

#define XtRFontSize      "FontSize"
#define XtRFontStyle     "FontStyle"

#endif /* _AtFontFamily_h */
