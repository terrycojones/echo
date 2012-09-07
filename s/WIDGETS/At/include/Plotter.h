/*
 *      Plotter.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sun Jul 19 19:23:41 1992, patchlevel 1
 *                                      AtPlotterGetLegendWidth() added.
 *      klin, Mon Jul 27 14:17:31 1992, patchlevel 2
 *                                      Resources XtNlegendLeft and
 *                                      XtNautoRedisplay added.
 *                                      Resource XtNusePixmap and
 *                                      drawing to a pixmap added.
 *                                      Resource XtNuseCursor and
 *                                      callback cursors added.
 *                                      Resource XtNbusyCallback and
 *                                      busy callback added.
 *      klin, Sun Aug  2 18:24:12 1992, patchlevel 3
 *                                      Layout callback and some stuff for
 *                                      aligning axis positions added.
 *                                      Resource XtNtitleHeigth and
 *                                      AtPlotterGetTitleHeight() added.
 *                                      Resources XtNxxxCursor added.
 *      klin, Sat Aug 15 10:02:01 1992, patchlevel 4
 *                                      Resources XtNslideCallback and
 *                                      XtNslideCursor and needed stuff added.
 *                                      Resources XtNselectCallback and
 *                                      XtNselectCursor and needed stuff added.
 *                                      Minor changes in callbacks.
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  Plotter.h"
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

#ifndef _AtPlotter_h
#define _AtPlotter_h

#include "At.h"
#include "Plot.h"
#include "FontFamily.h"

/*
 *   Resource names
 */

#ifndef XtNtitle
#define XtNtitle              "title"
#endif
#define XtNlegendTitle        "legendTitle"

#define XtNtitleSize          "titleSize"
#define XtNlegendTitleSize    "legendTitleSize"
#define XtNlegendSize         "legendSize"

#define XtNtitleStyle         "titleStyle"
#define XtNlegendTitleStyle   "legendTitleStyle"
#define XtNlegendStyle        "legendStyle"

#define XtNtitleColor         "titleColor"
#define XtNlegendColor        "legendColor"
#define XtNplotAreaColor      "plotAreaColor"

#define XtNshowTitle          "showTitle"
#define XtNshowLegend         "showLegend"
#define XtNtitleHeight        "titleHeight"
#define XtNlegendLeft         "legendLeft"
#define XtNlegendWidth        "legendWidth"
#define XtNlegendSpacing      "legendSpacing"

#define XtNmarginWidth        "marginWidth"
#define XtNmarginHeight       "marginHeight"

#define XtNrankChildren       "rankChildren"
#define XtNautoRedisplay      "autoRedisplay"
#define XtNuseCursors         "useCursors"
#define XtNusePixmap          "usePixmap"

#define XtNplotterCursor      "plotterCursor"
#define XtNbusyCursor         "busyCursor"
#define XtNmotionCursor       "motionCursor"
#define XtNclickCursor        "clickCursor"
#define XtNdragCursor         "dragCursor"
#define XtNslideCursor        "slideCursor"
#define XtNselectCursor       "selectCursor"

#define XtNlayoutCallback     "layoutCallback"
#define XtNbusyCallback       "busyCallback"
#define XtNmotionCallback     "motionCallback"
#define XtNclickCallback      "clickCallback"
#define XtNdragCallback       "dragCallback"
#define XtNslideCallback      "slideCallback"
#define XtNselectCallback     "selectCallback"

#define XtNxAxis              "xAxis"
#define XtNyAxis              "yAxis"
#define XtNx2Axis             "x2Axis"
#define XtNy2Axis             "y2Axis"

/*
 *   Resource classes
 */

#ifndef XtCTitle
#define XtCTitle              "Title"
#endif
#define XtCLegendTitle        "LegendTitle"

#define XtCShowTitle          "ShowTitle"
#define XtCShowLegend         "ShowLegend"
#define XtCTitleHeight        "TitleHeight"
#define XtCLegendLeft         "LegendLeft"
#define XtCLegendWidth        "LegendWidth"

#define XtCRankChildren       "RankChildren"
#define XtCAutoRedisplay      "AutoRedisplay"
#define XtCUseCursors         "UseCursors"
#define XtCUsePixmap          "UsePixmap"

#define XtCXAxis              "XAxis"
#define XtCYAxis              "YAxis"
#define XtCX2Axis             "X2Axis"
#define XtCY2Axis             "Y2Axis"

/*
 *   Constraint resource names and classes
 */

#define XtNdisplayed          "displayed"
#define XtNlegendName         "legendName"
#define XtNuseX2Axis          "useX2Axis"
#define XtNuseY2Axis          "useY2Axis"
#define XtNrankOrder          "rankOrder"

#define XtCDisplayed          "Displayed"
#define XtCLegendName         "LegendName"
#define XtCUseX2Axis          "UseX2Axis"
#define XtCUseY2Axis          "UseY2Axis"
#define XtCRankOrder          "RankOrder"

/*
 *   Axis positions and set position mask values
 */

typedef struct {
     char position;                /* Position mask */
     Position xaxis;               /* The axis positions */
     Position x2axis;
     Position yaxis;
     Position y2axis;
} AtAxisPositions;

#define AtPositionNONE   0x00      /* Nothing to position */
#define AtPositionXAXES  0x01      /* Position the X axes */
#define AtPositionYAXES  0x02      /* Position the Y axes */

/*
 *   Callback data types
 */

/* Client data from busy callback */

#define AtBusyPLOTTER    1
#define AtBusyPOSTSCRIPT 2

typedef struct {
     int reason;                   /* Reason for this callback */
     Boolean busy;                 /* True if busy, False otherwise */
} AtBusyCallbackData;

/* Client data from motion and click callbacks */

#define AtPointMOTION    1
#define AtPointCLICK     2

typedef struct {
     int reason;                   /* Reason for this callback */
     Position pixelx, pixely;      /* Pixel    point coordinates */
     double x1, y1;                /* 1st axes point coordinates */
     double x2, y2;                /* 2nd axes point coordinates */
} AtPointCallbackData;

/* Client data from drag and slide callbacks */

#define AtRectangleDRAG  1
#define AtRectangleSLIDE 2

typedef struct {
     int reason;                   /* The reason for this callback */
     Position pixelx1, pixely1;    /* Pixel lower left */
     Position pixelx2, pixely2;    /* Pixel upper rigth */
     double x11, y11, x12, y12;    /* 1st axes range coordinates */
     double x21, y21, x22, y22;    /* 2nd axes range coordinates */
} AtRectangleCallbackData;

/* Client data from select callback */

#define AtSelectSELECTED      1
#define AtSelectDESELECTED    2

typedef struct {
     int reason;                   /* The reason for this callback */
     Widget widget;                /* The selected/deselected widget */
} AtSelectCallbackData;

/*
 *   Class record constants
 */

extern WidgetClass atPlotterWidgetClass;

typedef struct _AtPlotterClassRec * AtPlotterWidgetClass;
typedef struct _AtPlotterRec      * AtPlotterWidget;

void AtPlotterGeneratePostscript P((char *, AtPlotterWidget, char *,
				    int, int, int, int, int));
extern void AtPlotterDrawPS P((FILE *, AtPlotterWidget, int, int, int, int));

/*
 *   These functions are for the plot children to communicate with the parent
 */

typedef struct {
    double xmin, xmax, ymin, ymax;
} BoundingBox;

extern void AtPlotterPlotExtended P((AtPlotWidget, BoundingBox *, int, int));
extern void AtPlotterPlotDataChanged P((AtPlotWidget, BoundingBox *, int));
extern void AtPlotterRefreshRequired P((AtPlotWidget));
extern void AtPlotterRedrawRequired P((AtPlotWidget));
extern void AtPlotterLayoutRequired P((AtPlotWidget));
extern void AtPlotterRescaleRequired P((AtPlotWidget));
extern void AtPlotterRecalcThisPlot P((AtPlotWidget));

/*
 *   These may be called from applications to get
 *   the plotters legend width and title height
 */

extern int AtPlotterGetLegendWidth P((AtPlotterWidget));
extern int AtPlotterGetTitleHeight P((AtPlotterWidget));

/*
 *   These may be called from applications to get and set
 *   the plotters axis positions, i.e. for axis alignments.
 */

extern Boolean AtPlotterGetAxisPositions P((AtPlotterWidget, AtAxisPositions *));
extern void AtPlotterSetAxisPositions P((AtPlotterWidget, AtAxisPositions *));

/*
 *   These may be used to get and set selected plot widgets.
 */

extern Widget AtPlotterGetSelectedPlot P((AtPlotterWidget));
extern Boolean AtPlotterSetSelectedPlot P((AtPlotterWidget, Widget));

#endif /* _AtPlotter_h */
