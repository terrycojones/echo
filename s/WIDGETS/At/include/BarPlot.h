/*
 *      BarPlot.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  BarPlot.h"
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
 * Simple bar chart, Y vs an integer X axis
 */

#ifndef _AtBarPlot_h
#define _AtBarPlot_h

#include "At.h"
#include "SPlot.h"
#include "Shading.h"

/* declare specific AtBarPlotWidget class and instance datatypes */

typedef struct _AtBarPlotClassRec*      AtBarPlotWidgetClass;
typedef struct _AtBarPlotRec*           AtBarPlotWidget;

/* declare the class constant */

extern WidgetClass atBarPlotWidgetClass;

/* The resources for this widget */
#define XtNcellOffset    "cellOffset"
#define XtNcellWidth     "cellWidth"
#define XtNdoOutline     "doOutline"
#define XtNdoFill        "doFill"
#define XtNzeroMin       "zeroMin"
#define XtNfillColor     "fillColor"
#define XtNfillPixmap    "fillPixmap"
#define XtNscreenShade   "screenShade"

#define XtCCellOffset    "CellOffset"
#define XtCCellWidth     "CellWidth"
#define XtCDoOutline     "DoOutline"
#define XtCDoFill        "DoFill"
#define XtCZeroMin       "ZeroMin"
#define XtCScreenShade   "ScreenShade"

#ifndef XtRDouble
# define XtRDouble       "Double"
#endif

/*
 * The "member" routines - these ones are exported from SPlot
 */
#define AtBarPlotAttachData(w, p, t, stride, start, num) \
     AtSPlotAttachData((AtSPlotWidget)w, p, t, stride, start, num)
#define AtBarPlotExtendData(w, n) AtSPlotExtendData((AtSPlotWidget)w, n)

#endif /* _AtBarPlot_h */
