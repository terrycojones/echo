/* Slider Widget public include file */

#ifndef _Slider2_h
#define _Slider2_h

#include <X11/Core.h>

/*
 *  New resources
 *
 *  The shadow is used for the outser frame. negative means that the
 *  widget will look sunken into the background. The thumbshadow is
 *  normally positive. The foreground color is used for the text,which
 *  is placed in the middle of the thumb.
 */

/*		Symbolic name	Resource name	   Type		Default	    */
/*		--------------	--------------	   -----------	----------- */
/*		XtNforeground	"foreground"	   Pixel	black	    */
/*		XtNfont		"font"		   XFontStruct*	NULL	    */
#define		XtNthumbcolor	"thumbcolor"	/* Pixel	white	    */
#define		XtNthumbPixmap	"thumbPixmap"	/* Pixmap	None	    */
#define		XtNshadow	"shadow"	/* int		-2	    */
#define		XtNthumbshadow	"thumbshadow"	/* int		2	    */
#define		XtNtext		"text"		/* String	NULL	    */
#define		XtNminsize	"minsize"	/* Dimension	20	    */
#define		XtNjump		"jump"		/* Callback	NULL	    */
#define		XtNdrop		"drop"		/* Callback	NULL	    */
#define		XtNdrag		"drag"		/* Callback	NULL	    */
#define		XtNbutton3	"button3"	/* Callback	NULL	    */
#define		XtNbutton2	"button2"	/* Callback	NULL	    */

/*		Symbolic name	Class name	   Type		Default     */
/*		--------------	-------------	   ------------ ----------- */
/*		XtCForeground	"Foreground"	   Pixel	black	    */
/*		XtCFont		"Font"		   XFontStruct*	NULL	    */
#define		XtCThumbcolor	"Thumbcolor"	/* Pixel	white	    */
#define		XtCThumbPixmap	"ThumbPixmap"	/* Pixmap	None	    */
#define 	XtCShadow	"Shadow"	/* int		-2          */
#define 	XtCThumbshadow	"Thumbshadow"	/* int		2           */
#define 	XtCText		"Text"		/* String	NULL        */
#define		XtCMinsize	"Minsize"	/* Dimension	20	    */
#define		XtCJump		"Jump"		/* Callback	NULL	    */
#define		XtCDrop		"Drop"		/* Callback	NULL	    */
#define		XtCDrag		"Drag"		/* Callback	NULL	    */
#define		XtCButton3	"Button3"	/* Callback	NULL	    */
#define		XtCButton2	"Button2"	/* Callback	NULL	    */

/*
 *  The button3 callback is called when the 3rd mouse button is
 *  clicked.
 *
 *  The jump callback is called when the user clicks outside the
 *  thumb. The callback gets one of the following constants as
 *  argument:
 */
#define THUMB_UP		1
#define THUMB_RIGHT		2
#define THUMB_DOWN		4
#define THUMB_LEFT		8
#define THUMB_UP_RIGHT		(THUMB_UP | THUMB_RIGHT)
#define THUMB_UP_LEFT		(THUMB_UP | THUMB_LEFT)
#define THUMB_DOWN_RIGHT	(THUMB_DOWN | THUMB_RIGHT)
#define THUMB_DOWN_LEFT		(THUMB_DOWN | THUMB_LEFT)
/*
 *  The drag callback is called for every move of the mouse, while
 *  dragging the thumb. Processing should be quick, otherwise move
 *  events will be stacked up and the programwill get behind.
 *
 *  The drop callback is called when the mouse is released after
 *  dragging the thumb. If moving along with the drag events is too
 *  slow, a program could install a callbeck for drop and anly receive
 *  the final position.
 *
 *  The position is passed along to the callback as a pointer to the
 *  following struct:
 */
typedef struct {
    float top;		/* thumb at top = 0 <= top <= 1 = at bottom */
    float left;		/* thumb left = 0 <= left <= 1 = right */
    float width;	/* 0 <= width <= 1 = max. width */
    float height;	/* 0 <= height <= 1 = max. height */
} SliderInfo;


extern WidgetClass slider2WidgetClass;

typedef struct _Slider2ClassRec *Slider2WidgetClass;
typedef struct _Slider2Rec *Slider2Widget;

/*
 *  Public functions
 *
 *  moveThumb and resizeThumb are called with the desired relative
 *  position and size of the thumb (numbers between 0 and 1).
 */

void moveThumb(
#if NeedFunctionPrototypes
	       Widget widget,
	       double thumb_x,			/* 0 <= thumb_x <= 1 */
	       double thumb_y			/* 0 <= thumb_y <= 1 */
#endif
);

void resizeThumb(
#if NeedFunctionPrototypes
	       Widget widget,
	       double thumb_wd,			/* 0 <= thumb_wd <= 1 */
	       double thumb_ht			/* 0 <= thumb_ht <= 1 */
#endif
);

void getThumb(
#if NeedFunctionPrototypes
	       Widget widget,
	       SliderInfo *info
#endif
);

#endif /* _Slider2_h */
