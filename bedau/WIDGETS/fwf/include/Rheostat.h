/*************************************************************
 *
 * Rheostat.h.
 * Public header file for Rheostat widget.
 *
 * Author: Joe English, joe@trystero.art.com
 *
 *************************************************************
 */

#ifndef  RHEOSTAT_H
#define  RHEOSTAT_H

extern WidgetClass rheostatWidgetClass;
typedef struct _RheostatClassRec *RheostatWidgetClass;
typedef struct _RheostatRec *RheostatWidget;

/*
 * Resources:
 * Angles are specified in degrees, going clockwise from straight down.
 * 0 = down, 90 = left, 180 = up, 270 = right, 360 = down again.
 *
 * The Dial value is an integer, with minimumValue <= value <= maximumValue
 *
 * Setting minimumAngle=minimumValue=0 and maximumAngle=maximumValue=360 
 * will configure the Rheostat for specifying angles.
 *
 * If resizeArrow is True, then the arrow will be resized proportionally
 * when the Rheostat is resized.
 *
 * If tickGravity is True, then clicking near a Rheostat tick will snap
 * the indicator to that tick.
 *
 */ 

#define XtNmaximumAngle		"maximumAngle"
#define XtNminimumAngle		"minimumAngle"
#define XtNmaximumValue		"maximumValue"
#define XtNminimumValue		"minimumValue"
#define XtNnumberIntervals	"numberIntervals"
#define XtNvalue		"value"
#define XtNresizeArrow		"resizeArrow"
#define XtNtickGravity		"tickGravity"

/* 
 * Geometry-specifying resources (outside to inside):
 */

#define XtNouterMargin		"outerMargin"
#define XtNtickLength		"tickLength"
#define XtNdialThickness	"dialThickness"
#define XtNinnerMargin		"innerMargin"
#define XtNradius		"radius"

/*
 * Appearance resources:
 */
#define XtNdialColor		"dialColor"
#define XtNtickColor		"tickColor"
#define XtNarrowColor		"arrowColor"
#define XtNtickThickness	"tickThickness"

/*
 * Arrow appearance:
 */
#define XtNarrowWidth		"arrowWidth"
#define XtNinnerArrowLength	"innerArrowLength"
#define XtNouterArrowLength	"outerArrowLength"
#define XtNfillArrow		"fillArrow"
#define XtNarrowThickness	"arrowThickness"

/* if useShadowColors is True, then the Motif * topShadowColor and 
 * bottomShadowcolor resources will be used for dialColor and arrowColor.
 * Only valid in Motif version of Rheostat.
 */
#define XtNuseShadowColors	"useShadowColors"


/*
 * Callbacks:
 */
#define XtNnotifyCallback	"notify"
#define XtNsetCallback		"set"

#define XtCNumberIntervals	"NumberIntervals"
#define XtCGravity		"Gravity"
#define XtCMinimum		"Minimum"
#define XtCMaximum		"Maximum"

/*
 * Rheostat callback structure:
 */
typedef struct {
    int 	reason;		/* for Motif compatibility only */
    XEvent 	*event;		/* Ditto */
    int		value;		/* current dial value */
} RheostatCallbackStruct;

/*
 * Convenience callback function:
 * 'closure' must be an 'int *', into which is stored the current dial value.
 */

extern void RheostatSetIntCallback( /* Widget,int *,RheostatCallbackStruct */);

#endif	/* RHEOSTAT_H */
