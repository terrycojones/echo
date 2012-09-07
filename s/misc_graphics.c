#ifndef NO_GRAPHICS

#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "WIDGETS/Athena3d/include/Label.h"        
#include "WIDGETS/Athena3d/include/Dialog.h"        
#include "WIDGETS/Athena3d/include/Command.h"    
#include "WIDGETS/Athena3d/include/AsciiText.h"

/* FWF includes. */
#include "WIDGETS/fwf/include/FileSel.h"

/* At plotter includes. */
#include "WIDGETS/At/include/AxisCore.h"

#include "misc_graphics.h"
#include "types.h"
#include "agents.h"
#include "sites.h"
#include "worlds.h"
#include "schema.h"
#include "running.h"
#include "resources.h"
#include "chromosomes.h"
#include "globals.h"
#include "graphics.h"
#include "syscalls.h"
#include "free.h"

void
quit(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
#ifndef NO_TCL
    if (site_file){
	Fclose(site_file);
    }
#endif
    XtDestroyApplicationContext(XtWidgetToApplicationContext(w));
    exit(0);
}


void 
popdown_widget(w, who_to_popdown, dummy2)
Widget w;
XtPointer who_to_popdown;
XtPointer dummy2;
{
    XtPopdown((Widget)who_to_popdown);
    return;
}

void
popup_widget(w, who_to_popup, dummy2)
Widget w;
XtPointer who_to_popup;
XtPointer dummy2;
{
    XtPopup((Widget)who_to_popup, XtGrabNone);
    return;
}

void
clear_text_widget(w, who_to_clear, dummy2)
Widget w;
XtPointer who_to_clear;
XtPointer dummy2;
{
    set_text_widget_text(*((Widget *) who_to_clear), "");
    return;
}

void
clear_dialog_widget(w, who_to_clear, dummy2)
Widget w;
XtPointer who_to_clear;
XtPointer dummy2;
{
    set_dialog_value(*((Widget *) who_to_clear), "");
    return;
}


void
left_justify_dialog_label(w)
Widget w;
{
    Arg arg;
    Widget label = XtNameToWidget(w, "label");

    if (!label){
	error("XtNameToWidget fails to find label subwidget.");
    }
    
    XtSetArg(arg, XtNjustify, XtJustifyLeft);
    XtSetValues(label, &arg, 1);
    
    return;
}
    
void
set_dialog_value(w, value)
Widget w;
String value;
{
    Arg arg;

    XtSetArg(arg, "value", value);
    XtSetValues(w, &arg, 1);
    
    return;
}


void
call_instantiate_world(w, down, ret)
Widget w;
XtPointer down;
FileSelectorOkButtonReturnStruct *ret;
{
    XtPopdown((Widget) down);
    instantiate_world(ret->path, ret->file, genome_hash_size);
    return;
}
    


void 
call_pause_world(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    pause_world();
    return;
}

void 
call_continue_world(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    continue_world();
    return;
}

void 
call_run_for_x_generations(w, n, dummy2)
Widget w;
XtPointer n;
XtPointer dummy2;
{
    run_for_x_generations((int) n);
    return;
}

void
call_run_world_indefinitely(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    run_world_indefinitely();
    return;
}

void
set_x_axis(w, dummy1, range)
Widget w;
XtPointer dummy1;
XtPointer range;
{
    /*
    AtAxisRangeArgs *axis_range = (AtAxisRangeArgs *) range;

    axis_range->minp = 0.0;
    axis_range->maxp = 100.0;
    axis_range->tic_intervalp = 10.0;
    */
    
    return;
}


void
toggle_verbose()
{
    /* verbose = (verbose == TRUE) ? FALSE : TRUE; */
    /* printf("Verbose mode %s.\n", (verbose == TRUE) ? "enabled" : "disabled"); */
    
    return;
}

String
remove_underscores(s)
String s;
{
    static String ret = NULL;
    int pos = 0;

    if (!ret){
	ret = XtMalloc(1024);
    }

    while (*s){
	if (*s == '_'){
	    ret[pos] = ' ';
	}
	else {
	    ret[pos] = *s;
	}
	
	s++;
	pos++;
    }
    
    ret[pos] = '\0';

    return ret;
}

Widget
create_label_and_text(text_w_name, label_w_value, above, parent)
String text_w_name;
String label_w_value;
Widget above;
Widget parent;
{
    Arg args[10];
    register int i;
    Widget label;
    
    i = 0;
    XtSetArg(args[i], XtNlabel, label_w_value);        i++;
    XtSetArg(args[i], "right", XawChainLeft);          i++;
    XtSetArg(args[i], "left", XawChainLeft);           i++;
    XtSetArg(args[i], XtNfromVert, above);             i++;
    XtSetArg(args[i], XtNjustify, XtJustifyLeft);      i++;
    label = XtCreateManagedWidget("label", labelWidgetClass, parent, args, i);
    
    i = 0;
    XtSetArg(args[i], "editType", XawtextEdit);        i++;
    XtSetArg(args[i], "resizable", True);              i++;
    XtSetArg(args[i], "resize", XawtextResizeWidth);   i++;
    XtSetArg(args[i], "right", XawChainRight);         i++;
    XtSetArg(args[i], "left", XawChainLeft);           i++;
    XtSetArg(args[i], XtNfromVert, above);             i++;
    XtSetArg(args[i], XtNfromHoriz, label);            i++;
    
    return XtCreateManagedWidget(text_w_name, asciiTextWidgetClass, parent, args, i);
}
    
void
set_text_widget_text(w, text)
Widget w;
String text;
{
    Arg args[1];

    XtSetArg(args[0], XtNstring, text);
    XtSetValues(w, args, 1);
    
    return;
}
    
String
get_text_widget_text(w)
Widget w;
{
    String text;
    Arg args[1];
    
    XtSetArg(args[0], XtNstring, &text);
    XtGetValues(w, args, 1);

    return text;
}

Widget
create_command_button(w_name, w_label, horiz, parent)
String w_name;
String w_label;
Widget horiz;
Widget parent;
{
    Arg args[10];
    register int i = 0;
    
    XtSetArg(args[i], XtNlabel, w_label);                             i++;
    XtSetArg(args[i], "right", XawChainLeft);                         i++;
    XtSetArg(args[i], "left", XawChainLeft);                          i++;
    XtSetArg(args[i], "top", XawChainTop);                            i++;
    XtSetArg(args[i], "bottom", XawChainTop);                         i++;
    if (horiz){
	XtSetArg(args[i], XtNfromHoriz, horiz);                       i++;
    }
    
    return XtCreateManagedWidget(w_name, commandWidgetClass, parent, args, i);
}

int
genome_difference(entry, genome, fp)
void *entry;
void *genome;
void *fp;
{
    STRING g1 = strdup(((GENOME_HASH_DATA *)entry)->genome);
    STRING g2 = strdup((char *)genome);
    STRING free1;
    STRING free2;
    register int distance = 0;
    char *u1;
    char *u2;
    register int i;
    
    free1 = g1;
    free2 = g2;
    
    for (i = 0; i < NCHROMOSOMES + 1; i++){
	u1 = strchr(g1, '_');
	u2 = strchr(g2, '_');
	
	if (!u1){
	    error("could not locate underscore in genome '%s'.", ((GENOME_HASH_DATA *)entry)->genome);
	}
	
	if (!u2){
	    error("could not locate underscore in genome '%s'.", (char *)genome);
	}
	
	*u1 = *u2 = '\0';
	
	distance += chromosome_string_distance(g1, g2);

	g1 = u1 + 1;
	g2 = u2 + 1;
    } 
	
    distance += chromosome_string_distance(g1, g2);
    
    fprintf((FILE *)fp, "%d ", distance);

    Free(free1);
    Free(free2);
    return 1;
}

int
living_genome_difference(entry, genome, fp)
void *entry;
void *genome;
void *fp;
{
    STRING g1;
    STRING g2;
    STRING free1;
    STRING free2;
    register int distance;
    char *u1;
    char *u2;
    register int i;
    
    if ((BOOLEAN) ((GENOME_HASH_DATA *)entry)->alive == FALSE){
	return 1;
    }

    g1 = strdup(((GENOME_HASH_DATA *)entry)->genome);
    g2 = strdup((char *)genome);
    free1 = g1;
    free2 = g2;
    distance = 0;
    
    for (i = 0; i < NCHROMOSOMES + 1; i++){
	u1 = strchr(g1, '_');
	u2 = strchr(g2, '_');
	
	if (!u1){
	    error("could not locate underscore in genome '%s'.", ((GENOME_HASH_DATA *)entry)->genome);
	}
	
	if (!u2){
	    error("could not locate underscore in genome '%s'.", (char *)genome);
	}
	
	*u1 = *u2 = '\0';
	
	distance += chromosome_string_distance(g1, g2);

	g1 = u1 + 1;
	g2 = u2 + 1;
    } 
	
    distance += chromosome_string_distance(g1, g2);
    
    fprintf((FILE *)fp, "%d ", distance);

    Free(free1);
    Free(free2);
    return 1;
}

int
cluster_for_agent(entry, fp, dummy)
void *entry;
void *fp;
void *dummy;
{
    hash_traverse(genome_hash, genome_difference, ((GENOME_HASH_DATA *)entry)->genome, fp);
    fprintf((FILE *)fp, "%s%03d \n", ((GENOME_HASH_DATA *)entry)->name, ((GENOME_HASH_DATA *)entry)->instance);
    return 1;
}

int
cluster_for_living_agent(entry, fp, dummy)
void *entry;
void *fp;
void *dummy;
{
    if (((GENOME_HASH_DATA *)entry)->alive == TRUE){
	hash_traverse(genome_hash, living_genome_difference, ((GENOME_HASH_DATA *)entry)->genome, fp);
	fprintf((FILE *)fp, "%s%03d \n", ((GENOME_HASH_DATA *)entry)->name, ((GENOME_HASH_DATA *)entry)->instance);
    }
    return 1;
}

void
cluster(w, living, dummy)
Widget w;
XtPointer living;
XtPointer dummy;
{
    extern int pclose();
    FILE *fp;
    
    fp = popen("cluster -t -", "w");

    if (!fp){
	popup_message("Could not popen cluster pipeline.\nClustering is not available"); 
	return;
    }
    
    if ((BOOLEAN)living == TRUE){
	hash_traverse(genome_hash, cluster_for_living_agent, fp, 0);
    }
    else {
	hash_traverse(genome_hash, cluster_for_agent, fp, 0);
    }
    
    pclose(fp);
    
    return;
}

void
call_pop_viewer()
{
    char cmd[1024];

    sprintf(cmd, "cd .. && ./sv OBJECTS/%s &", site_file_name);
    system(cmd);

    return;
}

#endif /* NO_GRAPHICS */

