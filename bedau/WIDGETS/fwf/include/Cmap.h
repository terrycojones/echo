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

#ifndef CMAP__H
#define CMAP__H

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
**  The XtNchangeCallback gets back the CmapCallbackStruct, below.
**  Reasons are defined below, and are obvious.
*/

extern WidgetClass cmapWidgetClass;
typedef struct _CmapClassRec *CmapWidgetClass;
typedef struct _CmapRec *CmapWidget;

#ifndef XtIsCmap
#define XtIsCmap(w) XtIsSubclass((w), cmapWidgetClass)
#endif

#define XtNchangeCallback "changeCallback"
#define XtNselected "selected"
#define XtNmargin "margin"
#define XtNboxWidth "boxWidth"
#define XtNboxHeight "boxHeight"

#define XtCSelected "Selected"
#ifndef XtCMargin
#define XtCMargin "Margin"
#endif
#define XtCBoxWidth "BoxWidth"
#define XtCBoxHeight "BoxHeight"

/* Reasons */
#define CMAP_SELECT 1
#define CMAP_LEFT 2
#define CMAP_RIGHT 3
#define CMAP_UP 4
#define CMAP_DOWN 5
#define CMAP_VERIFY 6

typedef struct _CmapCallbackStruct
{
  int reason;
  XEvent *event;
  XColor color;
} CmapCallbackStruct, *CmapCallbackPtr;

/* External Routines */
/* If you have
** an application that plays with the colormap, you
** must verify that the label is correct for that
** color (ie, the r,g,b values are correct.
** * NOTE * The event is NULL in the callback.
*/

#if NeedFunctionPrototypes
extern void CmapCheckSelectedColor(Widget _w, Boolean _notify);
#else
extern void CmapCheckSelectedColor();
#endif

/* 
** Currently selected box in cmap widget.
** If the widget is not a cmap, then the 
** returned value is undefined.
*/

#if NeedFunctionPrototypes 
extern XColor CmapGetSelected(Widget _w);
#else
extern XColor CmapGetSelected();
#endif

#endif /* CMAP__H */
