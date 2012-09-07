/*
 *      AxisCoreP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Mon Jul 27 14:16:10 1992, patchlevel 2
 *                                      Added new resource XtNnumberWidth
 *                                      and function AtAxisGetNumberWidth()
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *                                      Minor bug fix (G. Neumann)
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  AxisCoreP.h"
 */

/*

Copyright 1992 by University of Paderborn
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
 *   The core axis resources.
 *   This class is the superclass of all axis objects.
 *
 */

#ifndef _AxisCoreP_h
#define _AxisCoreP_h

#include "At.h"
#include "AxisCore.h"
#include "PlotP.h"

typedef void (*AtAxisRangeProc) P((AtAxisCoreWidget, double *, double *,
				   double *, int *));
typedef void (*AtAxisCalcProc) P((AtAxisCoreWidget));

typedef struct {
     AtAxisRangeProc    range_proc;
     AtAxisCalcProc     calc_proc;
} AtAxisCoreClassPart;

#define XtInheritRangeProc ((AtAxisRangeProc)_XtInherit)
#define XtInheritCalcProc  ((AtAxisCalcProc)_XtInherit)

typedef struct _AtAxisCoreClassRec {
     ObjectClassPart     object_class;
     AtPlotClassPart     plot_class;
     AtAxisCoreClassPart axiscore_class;
} AtAxisCoreClassRec;

extern AtAxisCoreClassRec atAxisCoreClassRec;

typedef struct {
     /* Resources */
     double         min;
     double         max;
     double         tic_interval;
     Boolean        vertical;
     Boolean        mirror;
     /* The label */
     String         font_family;
     String         label;
     int            label_size;
     int            label_style;
     Pixel          label_color;
     /* The Numbers */
     Dimension      default_number_width;
     int            number_size;
     int            number_style;
     Pixel          number_color;
     /* The callbacks */
     XtCallbackList range_callback;
     /* What-to-do resources */
     Boolean        tics_inside;
     Boolean        tics_outside;
     Dimension      tic_length;
     Dimension      subtic_length;
     Boolean        draw_numbers;
     Boolean        numbers_outside;
     Boolean        draw_grid;
     Boolean        draw_subgrid;
     Boolean        draw_origin;
     Boolean        draw_frame;
     Dimension      axis_linewidth;
     Pixel          axis_color;

     /* Private state */
     AtScale        *scale;
     Dimension      x1, y1;
     Dimension      x2, y2;
     Dimension      grid_length;
     int            max_num_width;      /* As decided by callbacks etc */
     int            actual_num_width;   /* Computed as text generated */
     int            axis_width;         /* includes tics, label & numbers */
     AtFontFamily   *ff;
     GC             label_gc;
     AtText         *label_text;
     Dimension      label_line;         /* Start drawing the label at this pos */
     GC             axis_gc;
     XSegment       axis_segment;
     XSegment       origin_segment;
     XSegment       frame_segment;
     GC             tic_gc;
     XSegment       *tic_segments;      /* Is num_tics + num_subtics */
     GC             number_gc;
     double         *tic_values;        /* Axis coords of major tics */
     String         *tic_label_string;  /* Label to go with each */
     Cardinal       num_tics;           /* How many tics & labels */
     double         *subtic_values;     /* Axis coord of subtics */
     Cardinal       num_subtics;        /* How many IN TOTAL */
     Cardinal       num_ticsegments;
     Dimension      *tic_pos;           /* The position of each label */
     Dimension      *subtic_pos;        /* The position of subtic */
     Dimension      tic_label_line;     /* Is on this line somewhere */
     AtText         **tic_label_text;   /* And num_tics of these */
     GC             grid_gc;
     XSegment       *grid_segments;     /* Is num_tics of these */
     GC             subgrid_gc;
     XSegment       *subgrid_segments;  /* Is num_subtics of these */
     Boolean        numbers_changed;
     Boolean        position_changed;
} AtAxisCorePart;

typedef struct _AtAxisCoreRec {
     ObjectPart     object;
     AtPlotPart     plot;
     AtAxisCorePart axiscore;
} AtAxisCoreRec;

#endif /* _AxisCoreP_h */
