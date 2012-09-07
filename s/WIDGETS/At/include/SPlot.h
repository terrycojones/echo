/*
 *      SPlot.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  SPlot.h"
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
 * AtSPlot is  non-instanced class that contains resources and routines
 * for accessing data that is currently stored in an array of
 * structures.
 */

#ifndef _AtSPlot_h
#define _AtSPlot_h

#include "At.h"
#include "Plot.h"

/* declare specific AtSPlotWidget class and instance datatypes */

typedef struct _AtSPlotClassRec*        AtSPlotWidgetClass;
typedef struct _AtSPlotRec*             AtSPlotWidget;

/* declare the class constant */

extern WidgetClass atSPlotWidgetClass;

/*
 * The "member" routine to attach the data.  The subclasses of AtSPlot
 * export a #define for this routine.
 */

extern void AtSPlotAttachData P((AtSPlotWidget, XtPointer data,
				 AtDataType type, Cardinal stride,
				 Cardinal start, Cardinal num));

extern void AtSPlotExtendData P((AtSPlotWidget, Cardinal num));

/*
 * Get the i'th value from the AtSPlot data.  This declares the function
 * version, a macro is available below.  This is intended mainly for
 * subclasses of AtSPlot, but applications may find it useful.
 */

extern double AtSPlotGetValue P((AtSPlotWidget, Cardinal));

/*
 * A macro version of the above.  May be faster, but death to debug!
 */

/* First, get the address of the i'th element */

#define _sp_ptr(p, i) ((XtPointer)((char *)p->splot.data + \
				   p->splot.stride * (i)))

#define AtSPlotGetValue(p, i) \
     ((double) \
      ((p)->splot.type == AtDouble ? *((double *)_sp_ptr((p), (i))) : \
       (p)->splot.type == AtFloat ? (double)*((float *)_sp_ptr((p), (i))) : \
       (p)->splot.type == AtInt ? (double)*((int *)_sp_ptr((p), (i))) : 0.0))

#endif /* _AtSPlot_h */
