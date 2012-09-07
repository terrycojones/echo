/* Slider2P.h - Private definitions for Slider2 widget */

#ifndef _Slider2P_h
#define _Slider2P_h

#include <X11/CoreP.h>
#include "Slider2.h"

typedef struct {
    int			dummy;
} Slider2ClassPart;

typedef struct _Slider2ClassRec {
    CoreClassPart	core_class;
    Slider2ClassPart	slider2_class;
} Slider2ClassRec;

extern Slider2ClassRec	slider2ClassRec;

typedef struct {
    /* resources: */
    Pixel		foreground;
    XFontStruct		*font;
    Pixel		thumbcolor;
    Pixmap		thumbPixmap;
    int			shadow;
    int			thumbshadow;
    String		text;
    Dimension		minsize;
    XtCallbackList	jump_callbacks;
    XtCallbackList	drop_callbacks;
    XtCallbackList	drag_callbacks;
    XtCallbackList	button3_callbacks;
    XtCallbackList	button2_callbacks;
    /* private state: */
    float		thumb_x;
    float		thumb_y;
    float		thumb_wd;
    float		thumb_ht;
    GC			gc;
    GC			textgc;
    GC			lightshadowgc;
    GC			darkshadowgc;
    Boolean		drag_in_progress;	/* True when dragging */
    int			m_delta_x;		/* Used when dragging */
    int			m_delta_y;		/* Used when dragging */
} Slider2Part;

typedef struct _Slider2Rec {
    CorePart		core;
    Slider2Part		slider2;
} Slider2Rec;

#endif /* _Slider2P_h */
