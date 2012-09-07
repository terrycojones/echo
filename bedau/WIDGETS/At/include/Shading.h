/*
 *      Shading.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  Shading.h"
 */

/*

Copyright 1991 by Burdett, Buckeridge & Young Ltd.

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
 * Specify Grayscale and stippling in a manner that is compatable
 * between printers and X
 */

#ifndef _Shading_h
#define _Shading_h

#include "At.h"

typedef enum {
     AtGRAY0,
     AtGRAY1,
     AtGRAY2,
     AtGRAY3,
     AtGRAY4,
     AtGRAY5,
     AtGRAY6,
     AtGRAY7,
     AtGRAY8,
     AtGRAY9,
     AtGRAY10
} AtShading;

#define XtNshading  "shading"
#define XtRShading  "Shading"
#define XtCShading  "Shading"

/*
 * Some routines to access the gray information
 *
 * The first two implement a cache of pixmaps, one pixmap per screen
 * per gray level.  The pixmaps are ref counted so they can be
 * released, saving server resources.
 *
 * The PostScript routine writes to the specified file a string that
 * sets the postscript graphics state so that a fill operation will
 * have the required apearance.
 *
 * The last is a routine to register a string to Shading converter.
 */

extern Pixmap AtShadingGetPixmap P((Screen *, AtShading, Pixel fg, Pixel bg));
extern void AtShadingReleasePixmap P((Pixmap));
extern char *AtShadingPS P((AtShading));
extern void AtRegisterShadingConverter P((void));

#endif /* _Shading_h */
