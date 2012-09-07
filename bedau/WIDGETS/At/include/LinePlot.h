/*
 *      LinePlot.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  LinePlot.h"
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
 * The timplest sort of ployt possible.  A line connecting the Y
 * values.  X values are integers without holes.
 */

#ifndef _AtLinePlot_h
#define _AtLinePlot_h

#include "At.h"
#include "SPlot.h"

/* declare specific AtLinePlotWidget class and instance datatypes */

typedef struct _AtLinePlotClassRec*     AtLinePlotWidgetClass;
typedef struct _AtLinePlotRec*          AtLinePlotWidget;

/* declare the class constant */

extern WidgetClass atLinePlotWidgetClass;


/*
 * The "member" routines - these ones are inherited from SPlot
 */
#define AtLinePlotAttachData(w,p,t,stride,start,num) \
     AtSPlotAttachData((AtSPlotWidget)w, p, t, stride, start, num)
#define AtLinePlotExtendData(w, n) AtSPlotExtendData((AtSPlotWidget)w, n)

#endif /* _AtLinePlot_h */
