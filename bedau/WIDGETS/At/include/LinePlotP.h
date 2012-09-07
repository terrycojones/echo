/*
 *      LinePlotP.h
 *
 *      The AthenaTools Plotter Widget Set - Version 6.0
 *
 *      klin, Tue Jul  7 13:59:47 1992
 *      klin, Sat Aug 15 10:31:50 1992, patchlevel 4
 *                                      Changed <At/..> to <X11/At/..>.
 *
 *      SCCSid[] = "@(#) Plotter V6.0  92/08/15  LinePlotP.h"
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

#ifndef _AtLinePlotP_h
#define _AtLinePlotP_h

#include "At.h"
#include "SPlotP.h"
#include "LinePlot.h"

typedef struct {
     int empty;
} AtLinePlotClassPart;

typedef struct _AtLinePlotClassRec {
     ObjectClassPart     object_class;
     AtPlotClassPart     plot_class;
     AtSPlotClassPart    splot_class;
     AtLinePlotClassPart lineplot_class;
} AtLinePlotClassRec;

extern AtLinePlotClassRec atLinePlotClassRec;

typedef struct {
     /* No state, use the stuff in AtSPlot only */
     int                 empty;
} AtLinePlotPart;

typedef struct _AtLinePlotRec {
     ObjectPart          object;
     AtPlotPart          plot;
     AtSPlotPart         splot;
     AtLinePlotPart      lineplot;
} AtLinePlotRec;

#endif /* _AtLinePlotP_h */
