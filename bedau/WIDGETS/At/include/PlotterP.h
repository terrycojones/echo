/*
 *      PlotterP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sun Jul 19 19:23:41 1992, patchlevel 1
 *                                      AtPlotterGetLegendWidth() added.
 *      klin, Mon Jul 27 14:17:43 1992, patchlevel 2
 *                                      Resources XtNlegendLeft and
 *                                      XtNautoRedisplay added.
 *                                      Resource XtNusePixmap and
 *                                      drawing to a pixmap added.
 *                                      Resource XtNuseCursor and
 *                                      callback cursors added.
 *                                      Resource XtNbusyCallback and
 *                                      busy callback added.
 *      klin, Sun Aug  2 18:24:27 1992, patchlevel 3
 *                                      Layout callback and some stuff for
 *                                      aligning axis positions added.
 *                                      Resource XtNtitleHeight and
 *                                      AtPlotterGetTitleHeight() added.
 *                                      Resources XtNxxxCursor added.
 *      klin, Sat Aug 15 10:25:48 1992, patchlevel 4
 *                                      Resources XtNslideCallback and
 *                                      XtNslideCursor and needed stuff added.
 *                                      Resources XtNselectCallback and
 *                                      XtNselectCursor and needed stuff added.
 *                                      Typos fixed.
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  PlotterP.h"
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

#ifndef _PlotterP_h
#define _PlotterP_h

#include "At.h"
#include "Plotter.h"
#include "Text.h"
#include "AxisCore.h"

#include <X11/cursorfont.h>
#include <X11/ConstrainP.h>

typedef struct ranking{       /* A private data structure */
     AtPlotWidget   child;
     int            rank_order;
     struct ranking *prev;
     struct ranking *next;
} Rank;

typedef struct {
     double    xmin,  xmax;
     double    x2min, x2max;
     double    ymin,  ymax;
     double    y2min, y2max;
} AtPlotterBoundingBox;

/*
 *   All these coords are with 0,0 in top left.  This is the opposite
 *   to the intuitive graph numbering, so some care is definitly required....
 *
 *   The Y axis is along the LH edge, the Y2 axis the RH edge, the X
 *   axis is bottom, X2 axis is the top.  Point (x1,y1) is top LH
 *   corner, (x2,y2) is bottom RH corner.
 *
 */

typedef struct {
     Position       x1, y1, x2, y2;               /* the plotting area */
     Dimension      width, height;                /* the size of same */
     Position       title_x, title_y;             /* where the title goes */
     Position       legend_x, legend_y;           /* where the legend goes */
     Dimension      legend_width, legend_height;  /* Size of legend */
} AtPlotterLayout;

typedef struct _AtPlotterPart {
     /* Resources */
     String              font_family;
     String              title;
     String              legend_title;
     int                 title_size;
     int                 legend_title_size;
     int                 legend_size;
     int                 title_style;
     int                 legend_title_style;
     int                 legend_style;
     Pixel               title_color;
     Pixel               legend_color;
     Pixel               plotarea_color;
     Boolean             show_title;
     Boolean             show_legend;
     Dimension           default_title_height;
     Boolean             legend_left;
     Dimension           default_legend_width;
     Dimension           legend_spacing;
     Dimension           margin_width;
     Dimension           margin_height;
     Boolean             rank_children;
     Boolean             auto_redisplay;
     Boolean             use_cursors;
     Boolean             use_pixmap;
     /* Cursors */
     Cursor              plotter_cursor;
     Cursor              busy_cursor;
     Cursor              click_cursor;
     Cursor              motion_cursor;
     Cursor              drag_cursor;
     Cursor              slide_cursor;
     Cursor              select_cursor;
     /* Callbacks */
     XtCallbackList      layout_callback;
     XtCallbackList      busy_callback;
     XtCallbackList      motion_callback;
     XtCallbackList      click_callback;
     XtCallbackList      drag_callback;
     XtCallbackList      slide_callback;
     XtCallbackList      select_callback;
     /* Private widgets. All are also children of this widget */
     AtAxisCoreWidget    xaxis;
     AtAxisCoreWidget    yaxis;
     AtAxisCoreWidget    x2axis;
     AtAxisCoreWidget    y2axis;

     /* Private state */
     Rank                *ordered_children;  /* rank ordered list of all children */
     AtFontFamily        *ff;
     GC                  title_gc;
     GC                  legend_gc;
     GC                  select_gc;
     GC                  drag_gc;
     GC                  plotarea_gc;
     AtText              *title_text;
     AtText              *legend_title_text;
     AtPlotterLayout     layout;
     AtAxisPositions     positions;          /* Current axis positions */
     AtPlotterBoundingBox bounding_box;      /* As accepted by the axes */
     AtPlotterBoundingBox raw_bounding_box;  /* Raw from plot data */
     /* States for callbacks */
     Boolean             in_select;
     Boolean             in_click;
     Boolean             in_drag;
     Boolean             in_slide;
     Boolean             can_slide;
     Boolean             in_legend;
     /* Positions for callbacks */
     int                 drag_x;
     int                 drag_y;
     int                 drag_width;
     int                 drag_height;
     double              slide_x1;
     double              slide_y1;
     double              slide_x2;
     double              slide_y2;
     int                 selected_item;      /* Selected legend item */
     int                 selected_item_y;
     int                 legend_item;        /* Item in selection */
     int                 legend_item_y;
     /* The state for the redraw processing */
     Boolean             expose_requested;
     Boolean             redraw_required;
     Boolean             rescale_required;
     Boolean             layout_required;
     Boolean             redisplay_required;
     Boolean             position_required;  /* Set axis positions */
     Boolean             in_layout_mode;     /* Help stop infinite loops! */
     /* The current and callback cursors */
     Cursor              current_cursor;
     Cursor              top_left_cursor;
     Cursor              top_right_cursor;
     Cursor              bottom_left_cursor;
     Cursor              bottom_right_cursor;
     short               cursor_mask;
     /* The pixmap and pixmap variables */
     Pixmap              pixmap;
     GC                  pixmap_gc;
     Dimension           pixmap_width;
     Dimension           pixmap_height;
     Boolean             pixmap_required;
} AtPlotterPart;

typedef struct _AtPlotterRec {
     CorePart            core;
     CompositePart       composite;
     ConstraintPart      constraint;
     AtPlotterPart       plotter;
} AtPlotterRec;

typedef struct _AtPlotterClassPart {
     int                 peek_a_boo;
} AtPlotterClassPart;

typedef struct _AtPlotterClassRec {
     CoreClassPart       core_class;
     CompositeClassPart  composite_class;
     ConstraintClassPart constraint_class;
     AtPlotterClassPart  plotter_class;
} AtPlotterClassRec;

extern AtPlotterClassRec atPlotterClassRec;

/*
 *   A linked list of from,to pairs for the extended list
 */

typedef struct _ExtendedList {
     struct _ExtendedList *next;
     int from, to;
} ExtendedList;

typedef struct _AtPlotterConstraintsPart {
     /* Resources */
     Boolean        displayed;
     String         legend_name;
     Boolean        use_x2_axis;
     Boolean        use_y2_axis;
     int            rank_order;

     /* Private  state */
     AtText         *legend_text;
     BoundingBox    bounding_box;
     /* This is the state kept for the redraw routine */
     Boolean        needs_refresh;
     ExtendedList   *extended_list;
} AtPlotterConstraintsPart;

typedef struct {
     AtPlotterConstraintsPart plotter;
} AtPlotterConstraintsRec, *AtPlotterConstraints;

#endif /* _PlotterP_h */
