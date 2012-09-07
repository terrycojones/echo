/*
 *      TextPlotP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  TextPlotP.h"
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

#ifndef _AtTextPlotP_h
#define _AtTextPlotP_h

#include "At.h"
#include "PlotP.h"
#include "TextPlot.h"
#include "FontFamily.h"
#include "Text.h"

typedef struct _AtTextPlotClassPart {
     int higgledy_piggledy;
} AtTextPlotClassPart;

typedef struct _AtTextPlotClassRec {
     ObjectClassPart     object_class;
     AtPlotClassPart     plot_class;
     AtTextPlotClassPart textplot_class;
} AtTextPlotClassRec;


typedef struct _AtTextPlotPart {
     /* Resources */
     String         label;
     String         font_family;
     int            font_size;
     int            font_style;
     int            justify;
     int            x;
     int            y;
     Boolean        floatingPosition;
     double         floating_x;
     double         floating_y;

     /* Private state */
     AtText         *label_text;
     AtFontFamily   *ff;
     /* Percentage of x & y, set at create time */
     float          xpercent;
     float          ypercent;
     /* There it actually is after justifying it */
     int            xpos, ypos;
} AtTextPlotPart;

typedef struct _AtTextPlotRec {
     ObjectPart     object;
     AtPlotPart     plot;
     AtTextPlotPart textplot;
} AtTextPlotRec;

#endif /* _AtTextPlotP_h */
