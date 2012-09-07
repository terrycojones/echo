/*
 *      BarPlotP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  BarPlotP.h"
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

#ifndef _AtBarPlotP_h
#define _AtBarPlotP_h

#include "At.h"
#include "SPlotP.h"
#include "BarPlot.h"

typedef struct {
    int empty;
} AtBarPlotClassPart;

typedef struct _AtBarPlotClassRec {
     ObjectClassPart    object_class;
     AtPlotClassPart    plot_class;
     AtSPlotClassPart   splot_class;
     AtBarPlotClassPart barplot_class;
} AtBarPlotClassRec;

extern AtBarPlotClassRec atBarPlotClassRec;

typedef struct {
     /* Resources */
     double             cell_offset;
     double             cell_width;
     Boolean            do_outline;
     Boolean            do_fill;
     Boolean            zero_min;
     Pixel              fill_color;
     AtShading          shading;
     Boolean            screen_shade;

     /* Internal State */
     BoundingBox        saved_bb;  /* Needed so the zeroMin resource works */
     Pixmap             shading_pixmap;
     GC                 fill_gc;
     XRectangle         *fill_rectangles;
} AtBarPlotPart;

typedef struct _AtBarPlotRec {
     ObjectPart         object;
     AtPlotPart         plot;
     AtSPlotPart        splot;
     AtBarPlotPart      barplot;
} AtBarPlotRec;


#endif /* _AtBarPlotP_h */
