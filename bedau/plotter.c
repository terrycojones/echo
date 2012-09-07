#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/r/echo/s/RCS/plotter.c,v 1.2 1994/04/08 06:31:38 terry Exp terry $";
#endif


#ifndef NO_GRAPHICS

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "WIDGETS/Athena3d/include/Form.h"

/* Athena plotter includes. */
#include "WIDGETS/At/include/Plotter.h"
#include "WIDGETS/At/include/XYAxis.h"
#include "WIDGETS/At/include/XYLinePlot.h"

#include "types.h"
#include "globals.h"
#include "plotter.h"
#include "syscalls.h"
#include "misc_graphics.h"

/*
 * Space for this many lines is created in a plotter by default.
 */
#define DEF_PLOTTER_LINES      5

/*
 * If more space is needed for lines, it is created this many at a time.
 */
#define DEF_PLOTTER_LINES_INC  1

PLOTTER *
create_plotter(title, xlabel, ylabel, widget_name, max_points, history_length, xmin, xmax, ymin, ymax, parent, nlines)
char *title;
char *xlabel;
char *ylabel;
char *widget_name;
int max_points;
int history_length;
double xmin;
double xmax;
double ymin;
double ymax;
Widget parent;
int nlines;
{
    PLOTTER *new;
    Widget button;
    int i;
    Arg args[15];
    /* double xtic = 10.0; */
    /* double ytic = 50.0; */

	rcsid = rcsid;

    if (history_length < 0){
	error("history_length has illegal value (%d) in create_plotter()", history_length);
    }

    if (history_length >= max_points){
	error("history_length (%s) is >= max_points (%d) in create_plotter()", history_length, max_points);
    }

    new = (PLOTTER *) Malloc(sizeof(PLOTTER));

    if (nlines <= 0){
	nlines = DEF_PLOTTER_LINES;
    }

    new->max_points = max_points;
    new->npoints = 0;
    new->history_length = history_length;
    new->xmin = xmin;
    new->original_xmin = xmin;
    new->xmax = xmax;
    new->original_xmax = xmax;
    new->ymin = ymin;
    new->original_ymin = ymin;
    new->ymax = ymax;
    new->original_ymax = ymax;
    new->lines = (PLOTTER_LINE *) Malloc(nlines * sizeof(PLOTTER_LINE));
    new->max_lines = nlines;
    new->nlines = 0;

    /* Create a popup and a form to contain the buttons and the plotter. */
    new->popup = XtCreatePopupShell(widget_name, topLevelShellWidgetClass, parent, NULL, 0);
    new->form = XtCreateManagedWidget("form", formWidgetClass, new->popup, NULL, 0);

    /* Create the exit button. */
    button = create_command_button("Exit", "Exit", NULL, new->form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) new->popup);

    /* Create the plotter widget. */
    i = 0;
    XtSetArg(args[i], XtNtitle, title);            i++;
    XtSetArg(args[i], XtNfromVert,  button);       i++;
    XtSetArg(args[i], XtNshowLegend, True);        i++;
    XtSetArg(args[i], XtNwidth,  600);             i++;
    XtSetArg(args[i], XtNheight, 600);             i++;
    XtSetArg(args[i], XtNborderWidth, 0);          i++;
    XtSetArg(args[i], XtNtop, XawChainTop);        i++;
    
    new->plotter = XtCreateManagedWidget("plotter", atPlotterWidgetClass, new->form, args, i);

    /* Create the X axis. */
    i = 0;
    XtSetArg(args[i], XtNlabel, xlabel);           i++;
    XtSetArg(args[i], XtNaxisWidth, 2);            i++;
    XtSetArg(args[i], XtNticsInside, True);        i++;
    XtSetArg(args[i], XtNticsOutside, False);      i++;
    XtSetArg(args[i], XtNlinTicFormat, "%g");      i++;
    /* XtSetArg(args[i], XtNticInterval, &xtic);      i++; */
    /* XtSetArg(args[i], XtNautoTics, False);         i++; */
    XtSetArg(args[i], XtNautoScale, False);        i++;
    XtSetArg(args[i], XtNmin, &xmin);              i++;
    XtSetArg(args[i], XtNmax, &xmax);              i++;

    new->xaxis = XtCreateWidget("xaxis", atXYAxisWidgetClass, new->plotter, args, i);

    /* Create the Y axis. */
    i = 0;
    XtSetArg(args[i], XtNlabel, ylabel);           i++;
    XtSetArg(args[i], XtNaxisWidth, 2);            i++;
    XtSetArg(args[i], XtNticsInside, True);        i++;
    XtSetArg(args[i], XtNticsOutside, False);      i++;
    XtSetArg(args[i], XtNvertical, True);          i++;
    XtSetArg(args[i], XtNlinTicFormat, "%g");      i++;
    /* XtSetArg(args[i], XtNticInterval, &ytic);      i++; */
    /* XtSetArg(args[i], XtNautoTics, False);         i++; */
    XtSetArg(args[i], XtNautoScale, False);        i++;
    XtSetArg(args[i], XtNmin, &ymin);              i++;
    XtSetArg(args[i], XtNmax, &ymax);              i++;    

    new->yaxis = XtCreateWidget("yaxis", atXYAxisWidgetClass, new->plotter, args, i);
    
    /* Attach the axes */
    i = 0;
    XtSetArg(args[i], XtNxAxis, new->xaxis);           i++;
    XtSetArg(args[i], XtNyAxis, new->yaxis);           i++;
    XtSetValues(new->plotter, args, i);
    
    return new;
}

void
clear_plotter(plotter)
PLOTTER *plotter;
{
    Arg args[2];
    register int i;

    XtSetArg(args[0], XtNmin, &plotter->original_xmin);
    XtSetArg(args[1], XtNmax, &plotter->original_xmax);
    XtSetValues(plotter->xaxis, args, 2);

    XtSetArg(args[0], XtNmin, &plotter->original_ymin);
    XtSetArg(args[1], XtNmax, &plotter->original_ymax);
    XtSetValues(plotter->yaxis, args, 2);

    plotter->npoints = 0;
    plotter->xmin = plotter->original_xmin;
    plotter->xmax = plotter->original_xmax;
    plotter->ymin = plotter->original_ymin;
    plotter->ymax = plotter->original_ymax;

    for (i = 0; i < plotter->nlines; i++){
	clear_plotter_line(&plotter->lines[i]);

	/* Re-enable printing on any lines that have been turned off. */
	plotter->lines[i].printing = TRUE;
    }

    return;
}

void
clear_plotter_line(line)
PLOTTER_LINE *line;
{
    AtXYLinePlotAttachIntPoints(line->line, line->data,  0, 0);

    return;
}

void
add_line_to_plotter(plotter, widget_name, title, delete_if_y_zero)
PLOTTER *plotter;
char *widget_name;
char *title;
BOOLEAN delete_if_y_zero;
{
    PLOTTER_LINE *line;
    register int i;
    Arg args[10];

    if (plotter->nlines == plotter->max_lines){
	PLOTTER_LINE *old = plotter->lines;
	
	plotter->lines = (PLOTTER_LINE *)
	    realloc(plotter->lines, (plotter->nlines + DEF_PLOTTER_LINES_INC) * sizeof(PLOTTER_LINE));

	if (old == plotter->lines){
	    /* The realloc failed. */
	    sysdie = FALSE;
	    error("Could not realloc to get a new plotter line in add_line_to_plotter().");
	    sysdie = TRUE;
	}

	plotter->max_lines += DEF_PLOTTER_LINES_INC;
    }

    line = plotter->lines + plotter->nlines;
    plotter->nlines++;

    line->delete_if_y_zero = delete_if_y_zero;
    line->printing = TRUE;

    /* Allocate space for the data. */
    line->data = (AtIntPoint *) Malloc(plotter->max_points * sizeof(AtIntPoint));

    /* Create the widget for the line. */
    i = 0;

    if (title){
	XtSetArg(args[i], XtNlegendName, title);          i++;
    }
    XtSetArg(args[i], XtNplotLineType, AtPlotLINEPOINTS);    i++;
    XtSetArg(args[i], XtNplotLineStyle, AtLineDOTTED);       i++;
    XtSetArg(args[i], XtNplotMarkType, AtMarkSTAR);          i++;

    line->line = XtCreateWidget(widget_name, atXYLinePlotWidgetClass, plotter->plotter, args, i);

    clear_plotter_line(line);

    return;
}

void
remove_all_lines_from_plotter(plotter)
PLOTTER *plotter;
{
    while (plotter->nlines){
	remove_line_from_plotter(plotter, plotter->nlines - 1);
    }

    return;
}

void
remove_line_from_plotter(plotter, line)
PLOTTER *plotter;
int line;
{
    register int i;
    register int nlines = plotter->nlines;

    XtDestroyWidget(plotter->lines[line].line);
    Free(plotter->lines[line].data);

    nlines--;
    
    for (i = line; i < nlines; i++){
	plotter->lines[i] = plotter->lines[i + 1];
    }

    plotter->nlines--;

    return;
}

void
extend_plotter_line(plotter, which_line, x, y)
PLOTTER *plotter;
int which_line;
int x;
int y;
{
    PLOTTER_LINE *line = plotter->lines + which_line;
    register int npoints = plotter->npoints;

    if (line->printing == FALSE){
	return;
    }

    line->data[npoints].x = x;
    line->data[npoints].y = y;

    if (plotter->ymax < y){
	Arg arg[1];

	plotter->ymax = 1.5 * (double) y;

	XtSetArg(arg[0], XtNmax, &plotter->ymax);
	XtSetValues(plotter->yaxis, arg, 1);
    }

    if (plotter->ymin > y){
	Arg arg[1];

	plotter->ymin = (double) y;

	XtSetArg(arg[0], XtNmin, &plotter->ymin);
	XtSetValues(plotter->yaxis, arg, 1);
    }

    
    if (y == 0 && line->delete_if_y_zero == TRUE){
	line->printing = FALSE;
    } else {
	AtXYLinePlotExtendData(line->line, npoints + 1);
    }
    
    return;
}

void
update_plotter(plotter)
PLOTTER *plotter;
{
    Arg args[2];
    PLOTTER_LINE *lines;
    int history_length;
    register int i;

    plotter->npoints++;

    if (plotter->npoints != plotter->max_points){
	return;
    }
    
    lines = plotter->lines;
    history_length = plotter->history_length;
    plotter->npoints = history_length;
    
    for (i = 0; i < plotter->nlines; i++){
	if (lines[i].printing == TRUE){
	    memcpy(lines[i].data, lines[i].data + plotter->max_points - history_length, history_length * sizeof(AtIntPoint));
	    AtXYLinePlotAttachIntPoints(lines[i].line, lines[i].data,  0, history_length);
	}
	else {
	    /* memcpy(lines[i].data, lines[i].data + plotter->max_points - history_length, history_length * sizeof(AtIntPoint)); */
	    AtXYLinePlotAttachIntPoints(lines[i].line, lines[i].data,  0, 0);
	}
    }
	
    for (i = 0; i < plotter->nlines; i++){
	if (lines[i].printing == TRUE){
	    break;
	}
    }

    if (i == plotter->nlines){
	/* no lines are still active. */
	return;
    }

    plotter->xmin = (double) lines[i].data[0].x;
    plotter->xmax = (double) (plotter->xmin + plotter->max_points);

    XtSetArg(args[0], XtNmin, &plotter->xmin);
    XtSetArg(args[1], XtNmax, &plotter->xmax);
    XtSetValues(plotter->xaxis, args, 2);

    return;
}


void
name_plotter_line(plotter, which_line, name)
PLOTTER *plotter;
int which_line;
char *name;
{
    PLOTTER_LINE *line = plotter->lines + which_line;
    Arg arg[1];

    XtSetArg(arg[0], XtNlegendName, name);
    XtSetValues(line->line, arg, 1);

    return;
}

#endif /* NO_GRAPHICS */
