/*
 *      TextPlot.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Fri Aug  7 10:07:13 1992, Some resource names and classes
 *                                      already defined in StringDefs.h
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  TextPlot.h"
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

#ifndef _AtTextPlot_h
#define _AtTextPlot_h

#include "At.h"
#include "FontFamily.h"

#define XtNfloatingPosition   "floatingPosition"
#define XtNfloatingX          "floatingX"
#define XtNfloatingY          "floatingY"

#define XtCFloatingPosition   "FloatingPosition"
#define XtCFloatingX          "FloatingX"
#define XtCFloatingY          "FloatingY"

extern WidgetClass atTextPlotWidgetClass;

typedef struct _AtTextPlotClassRec *AtTextPlotWidgetClass;
typedef struct _AtTextPlotRec *AtTextPlotWidget;

#endif /* _AtTextPlot_h */
