/*
 *      XYLinePlot.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Mon Jul 27 14:19:43 1992, patchlevel 2
 *                                      Plot types steps and bars added
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  XYLinePlot.h"
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
 *   The more complicated sort of plot.
 *   A line connecting the X and Y values attached to the axes.
 */

#ifndef _AtXYLinePlot_h
#define _AtXYLinePlot_h

#include "At.h"
#include "XYPlot.h"

/* Declare specific AtXYLinePlotWidget class and instance datatypes */

typedef struct _AtXYLinePlotClassRec*   AtXYLinePlotWidgetClass;
typedef struct _AtXYLinePlotRec*        AtXYLinePlotWidget;

/* Declare the class constant */

extern WidgetClass atXYLinePlotWidgetClass;

/*
 *   Resources available to be set/fetched
 */

#define XtNplotLineType  "plotLineType"
#define XtNplotLineStyle "plotLineStyle"
#define XtNplotMarkType  "plotMarkType"

#define XtCPlotLineType  "PlotLineType"
#define XtCPlotLineStyle "PlotLineStyle"
#define XtCPlotMarkType  "PlotMarkType"

/*
 *   The plot line types
 */

typedef enum {
     AtPlotLINES,             /* lines, default */
     AtPlotPOINTS,            /* points */
     AtPlotIMPULSES,          /* impulses */
     AtPlotSTEPS,             /* staircase */
     AtPlotBARS,              /* bars */
     AtPlotLINEPOINTS,        /* lines with points */
     AtPlotLINEIMPULSES,      /* lines with impulses */
     AtPlotINVALID            /* invalid: use default */
} AtPlotLineType;

/*
 *   The plot line styles
 */

typedef enum {
     AtLineSOLID,             /* solid, default */
     AtLineDOTTED,            /* dotted 1 */
     AtLineDASHED,            /* dashed 2 */
     AtLineDOTDASHED,         /* dot dashed 1 */
     AtLineDOTTED2,           /* dotted 2 */
     AtLineDOTTED3,           /* dotted 3 */
     AtLineDOTTED4,           /* dotted 4 */
     AtLineDOTTED5,           /* dotted 5 */
     AtLineDASHED3,           /* dashed 3 */
     AtLineDASHED4,           /* dashed 4 */
     AtLineDASHED5,           /* dashed 5 */
     AtLineDOTDASHED2,        /* dot dashed 2 */
     AtLineINVALID            /* invalid: use default */
} AtPlotLineStyle;

/*
 *   The plot marker types
 */

typedef enum {
     AtMarkRECTANGLE,         /* rectangle, default */
     AtMarkPLUS,              /* plus sign */
     AtMarkXMARK,             /* x-mark sign */
     AtMarkSTAR,              /* star */
     AtMarkDIAMOND,           /* diamond */
     AtMarkTRIANGLE1,         /* bottom triangle */
     AtMarkTRIANGLE2,         /* top triangle */
     AtMarkTRIANGLE3,         /* left triangle */
     AtMarkTRIANGLE4,         /* right triangle */
     AtMarkINVALID            /* invalid: use default */
} AtPlotMarkType;

/*
 *   Some types for easier attaching data
 */

typedef struct {
     double x;
     double y;
} AtDoublePoint;

typedef struct {
     float x;
     float y;
} AtFloatPoint;

typedef struct {
     int x;
     int y;
} AtIntPoint;

/*
 *   The member routines - these ones are inherited from XYPlot
 */

#define AtXYLinePlotAttachData(w, xp, xt, xstride, yp, yt, ystride, start, num) \
     AtXYPlotAttachData((AtXYPlotWidget) w, xp, xt, xstride, \
					    yp, yt, ystride, start, num)
#define AtXYLinePlotExtendData(w, n) \
     AtXYPlotExtendData((AtXYPlotWidget) w, n)

/*
 *   The member routines for AtxxPoint data
 */

#define AtXYLinePlotAttachDoublePoints(w, data, start, num) \
     AtXYPlotAttachData((AtXYPlotWidget) w, \
	  (XtPointer) &(data)->x, AtDouble, sizeof(AtDoublePoint), \
	  (XtPointer) &(data)->y, AtDouble, sizeof(AtDoublePoint), \
	  start, num)

#define AtXYLinePlotAttachFloatPoints(w, data, start, num) \
     AtXYPlotAttachData((AtXYPlotWidget) w, \
	  (XtPointer) &(data)->x, AtFloat, sizeof(AtFloatPoint), \
	  (XtPointer) &(data)->y, AtFloat, sizeof(AtFloatPoint), \
	  start, num)

#define AtXYLinePlotAttachIntPoints(w, data, start, num) \
     AtXYPlotAttachData((AtXYPlotWidget) w, \
	  (XtPointer) &(data)->x, AtInt, sizeof(AtIntPoint), \
	  (XtPointer) &(data)->y, AtInt, sizeof(AtIntPoint), \
	  start, num)

/* For use with X Toolkit resources and converters */

#define XtRPlotLineType  "PlotLineType"
extern void AtRegisterPlotLineTypeConverter P((void));
#define XtRPlotLineStyle "PlotLineStyle"
extern void AtRegisterPlotLineStyleConverter P((void));
#define XtRPlotMarkType  "PlotMarkType"
extern void AtRegisterPlotMarkTypeConverter P((void));

#endif /* _AtXYLinePlot_h */
