/*
 * Copyright 1992 John L. Cwikla
 * 
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of John L. Cwikla or
 * University of Illinois not be used in advertising or publicity
 * pertaining to distribution of the software without specific, written
 * prior permission.  John L. Cwikla and University of Illinois make no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * John L. Cwikla and University of Illinois disclaim all warranties with
 * regard to this software, including all implied warranties of
 * merchantability and fitness, in no event shall John L. Cwikla or
 * University of Illinois be liable for any special, indirect or
 * consequential damages or any damages whatsoever resulting from loss of
 * use, data or profits, whether in an action of contract, negligence or
 * other tortious action, arising out of or in connection with the use or
 * performance of this software.
 *
 * Author:
 * 	John L. Cwikla
 * 	Materials Research Laboratory Center for Computation
 * 	University Of Illinois at Urbana-Champaign
 *	104 S. Goodwin
 * 	Urbana, IL 61801
 * 
 * 	cwikla@uimrl7.mrl.uiuc.edu
 */

#ifndef CMAPP__H
#define CMAPP__H

/*
** Cmap Widget
*/

/*
** Resources:           Type:      Default:         Comment:
**
**  XtNmargin         : integer  :  5             : minimum margin around widget picture and label
**  XtNselected       : integer  :  0             : current box for selection and values
**  XtNchangeCallback : callback : NULL           : callback when any new box is chosen
**  XtNfont           : XFontStruct   : XtDefaultFont  : fontname for label
**  XtNforeground     : pixel    : XtDefaultForeground : pixel value for selected box and label
**  XtNboxWidth       : integer  : 0              : maximum box width (0 for calculate/resize)
**  XtNboxHeight      : integer  : 0              : maximum box height (0 for calculate/resize)
**
*/


#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include "Cmap.h"

typedef struct _CmapPart
{
  XFontStruct *font;
  /* "R:65535 G:65535 B:65535 (255 of 255)" */
  char label[50];	/* Label for rgb string */
  Boolean useMargin;
  Pixel foreground;
  XColor selectedColor;
  int margin;
  int availColors;
  int selected;
  int numX;
  int numY;
  XPoint labelPos;
  XPoint drawPos;
  Dimension labelHeight;
  Dimension labelWidth;
  Dimension lineWidth;
  int boxWidth;
  int boxHeight;
  int realBoxWidth;
  int realBoxHeight;
  GC gc;
  GC foregroundGC;
  GC eraseGC;
  XtCallbackList changeCallback;
} CmapPart, *CmapPartPtr;

typedef struct _CmapRec
{
  CorePart core;
  CmapPart cmap;
} CmapRec, *CmapPtr;

typedef struct _CmapClassPart
{
  int empty;
} CmapClassPart;

typedef struct _CmapClassRec
{
  CoreClassPart core_class;	
  CmapClassPart cmap_class;
} CmapClassRec, *CmapClassPtr;

extern CmapClassRec cmapClassRec;

#endif /* CMAPP__H */
