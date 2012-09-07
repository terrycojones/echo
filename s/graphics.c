#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/s/RCS/graphics.c,v 1.10 1993/05/27 15:46:00 terry Exp terry $";
#endif

#ifndef NO_GRAPHICS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include "WIDGETS/Athena3d/include/Box.h"
#include "WIDGETS/Athena3d/include/Label.h"
#include "WIDGETS/Athena3d/include/Dialog.h"
#include "WIDGETS/Athena3d/include/Form.h"
#include "WIDGETS/Athena3d/include/Viewport.h"
#include "WIDGETS/Athena3d/include/Command.h"
#include "WIDGETS/Athena3d/include/MenuButton.h"
#include "WIDGETS/Athena3d/include/SimpleMenu.h"
#include "WIDGETS/Athena3d/include/Sme.h"
#include "WIDGETS/Athena3d/include/SmeBSB.h"
#include "WIDGETS/Athena3d/include/SmeLine.h"
#include "WIDGETS/Athena3d/include/Cardinals.h"
#include "WIDGETS/Athena3d/include/AsciiText.h"

/* FWF includes. */
#ifndef NO_CANVAS
#include "WIDGETS/fwf/include/Canvas.h"
#endif
#include "WIDGETS/fwf/include/FileSel.h"

/* Athena plotter includes. */
#include "WIDGETS/At/include/Plotter.h"
#include "WIDGETS/At/include/XYAxis.h"
#include "WIDGETS/At/include/XYLinePlot.h"

#include "types.h"
#include "plotter.h"
#include "agents.h"
#include "sites.h"
#include "worlds.h"
#include "schema.h"
#include "running.h"
#include "resources.h"
#include "globals.h"
#include "graphics.h"
#include "syscalls.h"
#include "free.h"
#include "misc_graphics.h"
#include "random.h"

/* Callbacks. */
static void save_agent_edit();
static void save_site_edit();
static void save_world_edit();
static void read_and_display_agent_edit();
static void read_and_display_site_edit();
static void read_and_display_world_edit();
static void create_agent_edit_widgets();
static void create_site_edit_widgets();
static void create_world_edit_widgets();
static void call_set_halt_generation();
static void set_verbosity();
static void popup_generation_selector();
static void popup_verbose_selector();
static void popup_seed_selector();
static void popup_stack();
static void popdown_stack();
static void set_seed();
static void resume_from_stack();
static void set_new_schema();
static void popup_stack_selector();
static void select_stack_for_editing();
static void show_seed();
static void rerun_world();

/* Other functions. */
static void create_control_menu_widgets();
static void create_edit_menu_widgets();
static void create_running_menu_widgets();
static void create_graph_widgets();
static void create_schema_widgets();
static void create_message_widgets();

extern String XawDialogGetValueString();

String fallback_resources[] = { 
    "*editMenuButton.label:                 Edit",
    "*controlMenuButton.label:              Control",
    "*runningMenuButton.label:              Running",
    "*graphsMenuButton.label:               Graphs",
    "*schemaMenuButton.label:               Examine",
    NULL,
};

struct {
    AGENT_EDIT *agent;
    Widget agent_file_selector_popup;
    Widget fs;
    Widget popup;
    Widget form;
    Widget filename_text;
    Widget name_text;
    Widget resource_text;
    Widget reservoir_text;
    Widget chromosome_text[NCHROMOSOMES];
    Widget uptake_text;
} agent_edit;

struct {
    SITE_EDIT *site;
    Widget site_file_selector_popup;
    Widget fs;
    Widget popup;
    Widget agents_popup;
    Widget agents_form;
    Widget agents_text;
    Widget form;
    Widget filename_text;
    Widget name_text;
    Widget mutation_text;
    Widget crossover_text;
    Widget death_text;
    Widget production_text;
    Widget maximums_text;
    Widget maintenance_text;
    Widget resources_text;
} site_edit;

struct {
    WORLD_EDIT *world;
    Widget world_file_selector_popup;
    Widget fs;
    Widget popup;
    Widget sites_form;
    Widget sites_popup;
    Widget sites_text;
    Widget combat_form;
    Widget combat_popup;
    Widget combat_text;
    Widget form;
    Widget filename_text;
    Widget name_text;
    Widget nresources_text;
    Widget rows_text;
    Widget cols_text;
    Widget trade_text;
    Widget c_text;
    Widget p_maintenance_text;
    Widget selfrep_fraction_text;
    Widget selfrep_threshold_text;
    Widget neighborhood_text;
} world_edit;

struct {
    Widget menu_button;
    Widget menu;
    Widget entry;
    Widget verbose_selector_popup;
    Widget verbose_form;
    Widget verbose_dialog;
} control_menu_widgets;

struct {
    Widget menu_button;
    Widget menu;
    Widget entry;
    Widget stack_popup;
    Widget stack_help_popup;
    Widget stack_viewport;
    Widget stack_form;
    Widget stack_help_form;
    Widget stack_text;
    Widget stack_header;
    Widget stack_selector_popup;
    Widget stack_selector_form;
    Widget stack_selector_dialog;
    String stack_file;
    FILE *stack_fp;
    SITE *stack_site;
} edit_menu_widgets;

struct {
    Widget menu_button;
    Widget menu;
    Widget entry;
    Widget world_file_selector_popup;
    Widget fs;
    Widget generation_selector_popup;
    Widget generation_form;
    Widget generation_dialog;
    Widget seed_popup;
    Widget seed_form;
    Widget seed_dialog;
} running_menu_widgets;

struct {
    Widget menu_button;
    Widget menu;
} graph_menu_widgets;

struct {
    Widget menu_button;
    Widget menu;
    Widget popup;
    Widget form;
    Widget schema_dialog;
    String schema;
} schema_menu_widgets;

PLOTTER *population_graph;
PLOTTER *schema_graph;
PLOTTER *variant_graph;
PLOTTER *resources_graph;
PLOTTER *population_graph;
PLOTTER *species_graph;

struct {
    Widget popup;
    Widget dialog;
} message_widgets;

XtAppContext context;

#ifndef NO_CANVAS
Widget canvas_widgets_parent;
Widget **canvas_widgets;
#endif

void
initialize_graphics(argc, argv, world_file, object_location)
int argc;
char **argv;
char *world_file;
char *object_location;
{
    Widget toplevel;
    Widget form;
    Arg args[10];
    register int i;
    FileSelectorOkButtonReturnStruct def_file;
    String file;

    rcsid = rcsid;
    
    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                              i++;
    toplevel = XtAppInitialize(&context, "Echo", (XrmOptionDescList) NULL, 0, &argc, argv, fallback_resources, args, i);

    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);          i++;
    form = XtCreateManagedWidget("form", formWidgetClass, toplevel, args, i);
#ifndef NO_CANVAS
    canvas_widgets_parent = form;
#endif

    create_agent_edit_widgets(toplevel, object_location);
    create_site_edit_widgets(toplevel, object_location);
    create_world_edit_widgets(toplevel, object_location);
    create_control_menu_widgets(form, toplevel);
    create_edit_menu_widgets(form);
    create_running_menu_widgets(form, object_location);
    create_graph_widgets(form);
    create_schema_widgets(form);
    create_message_widgets(form);
    
    XtRealizeWidget(toplevel);

    if (world_file)
    {
	def_file.path = "./worlds";
	def_file.file = world_file;
	read_and_display_world_edit(0, 0, &def_file);

	read_and_display_site_edit(0, 0, 0);
	read_and_display_agent_edit(0, 0, 0);
    }
    else
    {
	if ((file = getenv("ECHO_WORLD"))){
	    def_file.path = "./worlds";
	    def_file.file = file;
	    read_and_display_world_edit(0, 0, &def_file);
	}
	else {
	    read_and_display_world_edit(0, 0, 0);
	}
	
	if ((file = getenv("ECHO_SITE"))){
	    def_file.path = "./sites";
	    def_file.file = file;
	    read_and_display_site_edit(0, 0, &def_file);
	}
	else {
	    read_and_display_site_edit(0, 0, 0);
	}
	
	if ((file = getenv("ECHO_AGENT"))){
	    def_file.path = "./agents";
	    def_file.file = file;
	    read_and_display_agent_edit(0, 0, &def_file);
	}
	else {
	    read_and_display_agent_edit(0, 0, 0);
	}
    }
    
    /* We start with just these two graphs showing. */
    popup_widget(0, variant_graph->popup, 0);
    popup_widget(0, species_graph->popup, 0);

    if (world_file) {
	instantiate_world("./worlds", world_file, genome_hash_size);
    }
    
    XtAppMainLoop(context);
}


static void
read_and_display_agent_edit(w, down, ret)
Widget w;
XtPointer down;
FileSelectorOkButtonReturnStruct *ret;
{
    /* char filename[MAXPATHLEN]; */
    register int i;

    if (ret){
	if (down){
	    /* Pop down the selector that was used to choose this file. */
	    XtPopdown((Widget) down);
	}
    
	XawFormDoLayout(agent_edit.form, FALSE);
	agent_edit.agent = read_agent_edit_from_file(ret->path, ret->file);
	
	if (!agent_edit.agent){
	    return;
	}
    }
    else {
	/* They want to edit an agent from scratch. */
	agent_edit.agent = new_agent_edit();
    }

    set_text_widget_text(agent_edit.name_text, AGENT_EDIT_NAME(agent_edit.agent));
    
    if (ret){
	/* 
	 * This would give the full path name.
	 * sprintf(filename, "%s%s", ret->path, ret->file);
	 * set_text_widget_text(agent_edit.filename_text, filename);
	 */
    
	set_text_widget_text(agent_edit.filename_text, ret->file);
    }
    else {
	set_text_widget_text(agent_edit.filename_text, "");
    }
    set_text_widget_text(agent_edit.uptake_text, AGENT_EDIT_UPTAKE_MASK(agent_edit.agent));
    set_text_widget_text(agent_edit.resource_text, AGENT_EDIT_TRADING_RESOURCE(agent_edit.agent));
    set_text_widget_text(agent_edit.reservoir_text, AGENT_EDIT_RESERVE_LIST(agent_edit.agent));
    
    for (i = 0; i < NCHROMOSOMES; i++){
	set_text_widget_text(agent_edit.chromosome_text[i], AGENT_EDIT_CHROMOSOMES(agent_edit.agent)[i]);
    } 
    
    XawFormDoLayout(agent_edit.form, TRUE);
    XtPopup(agent_edit.popup, XtGrabNone);
    return;
}

static void
read_and_display_site_edit(w, down, ret)
Widget w;
XtPointer down;
FileSelectorOkButtonReturnStruct *ret;
{
    Arg args[1];

    if (ret){
	if (down){
	    XtPopdown((Widget) down);
	}
	XawFormDoLayout(site_edit.form, FALSE);
    
	site_edit.site = read_site_edit_from_file(ret->path, ret->file);
	
	if (!site_edit.site){
	    return;
	}
    }
    else {
	/* They want to edit an agent from scratch. */
	site_edit.site = new_site_edit();
    }

    set_text_widget_text(site_edit.name_text, SITE_EDIT_NAME(site_edit.site));
    if (ret){
	set_text_widget_text(site_edit.filename_text, ret->file);
    }
    else {
	set_text_widget_text(site_edit.filename_text, "");
    }
    set_text_widget_text(site_edit.mutation_text, SITE_EDIT_P_MUTATION(site_edit.site));
    set_text_widget_text(site_edit.crossover_text, SITE_EDIT_P_CROSSOVER(site_edit.site));
    set_text_widget_text(site_edit.death_text, SITE_EDIT_P_RANDOM_DEATH(site_edit.site));
    set_text_widget_text(site_edit.production_text, SITE_EDIT_PRODUCE_LIST(site_edit.site));
    set_text_widget_text(site_edit.maximums_text, SITE_EDIT_MAXIMUM_LIST(site_edit.site));
    set_text_widget_text(site_edit.maintenance_text, SITE_EDIT_MAINTENANCE_LIST(site_edit.site));
    set_text_widget_text(site_edit.resources_text, SITE_EDIT_RESOURCE_LIST(site_edit.site));
    
    XtSetArg(args[0], XtNstring, SITE_EDIT_AGENTS(site_edit.site));
    XtSetValues(site_edit.agents_text, args, 1);
    
    XawFormDoLayout(site_edit.form, TRUE);
    XtPopup(site_edit.popup, XtGrabNone);
    return;
}

static void
read_and_display_world_edit(w, down, ret)
Widget w;
XtPointer down;
FileSelectorOkButtonReturnStruct *ret;
{
    Arg args[1];
    char *combat_matrix_as_str = NULL;

    if (ret){
	if (down) {
	    XtPopdown((Widget) down);
	}
	XawFormDoLayout(world_edit.form, FALSE);
	
	world_edit.world = read_world_edit_from_file(ret->path, ret->file);
	
	if (!world_edit.world){
	    return;
	}
    }
    else {
	/* They want to edit a world from scratch. */
	world_edit.world = new_world_edit();
    }

    set_text_widget_text(world_edit.name_text, WORLD_EDIT_NAME(world_edit.world));
    if (ret){
	set_text_widget_text(world_edit.filename_text, ret->file);
    }
    else {
	set_text_widget_text(world_edit.filename_text, "");
    }
    set_text_widget_text(world_edit.nresources_text, WORLD_EDIT_NRESOURCES(world_edit.world));
    set_text_widget_text(world_edit.rows_text, WORLD_EDIT_ROWS(world_edit.world));
    set_text_widget_text(world_edit.cols_text, WORLD_EDIT_COLS(world_edit.world));
    set_text_widget_text(world_edit.trade_text, WORLD_EDIT_TRADE_FRACTION(world_edit.world));
    set_text_widget_text(world_edit.c_text, WORLD_EDIT_C(world_edit.world));
    set_text_widget_text(world_edit.selfrep_fraction_text, WORLD_EDIT_SELFREP_FRACTION(world_edit.world));
    set_text_widget_text(world_edit.selfrep_threshold_text, WORLD_EDIT_SELFREP_THRESHOLD(world_edit.world));
    set_text_widget_text(world_edit.p_maintenance_text, WORLD_EDIT_P_MAINTENANCE(world_edit.world));
    set_text_widget_text(world_edit.neighborhood_text, WORLD_EDIT_NEIGHBORHOOD(world_edit.world));
    
    if (ret){
	register int combat_matrix_as_str_len = 0;
	register NRESOURCES resource;
	register NRESOURCES resources = (NRESOURCES) atoi(WORLD_EDIT_NRESOURCES(world_edit.world));

	for (resource = 0; resource < resources; resource++){
	    /* The + 1 is for a newline we'll insert. */
	    combat_matrix_as_str_len += strlen( WORLD_EDIT_COMBAT(world_edit.world)[resource] ) + 1;
	} 
	
	combat_matrix_as_str = Malloc(combat_matrix_as_str_len);
	
	sprintf(combat_matrix_as_str, "%s", WORLD_EDIT_COMBAT(world_edit.world)[0]);
	
	for (resource = 1; resource < resources; resource++){
	    sprintf(combat_matrix_as_str, "%s\n%s", combat_matrix_as_str, WORLD_EDIT_COMBAT(world_edit.world)[resource]);
	} 
    }
    
    XtSetArg(args[0], XtNstring, ret ? combat_matrix_as_str : "");
    XtSetValues(world_edit.combat_text, args, 1);
    
    Free(combat_matrix_as_str);
    
    XtSetArg(args[0], XtNstring, WORLD_EDIT_SITES(world_edit.world));
    XtSetValues(world_edit.sites_text, args, 1);
    
    XawFormDoLayout(world_edit.form, TRUE);
    XtPopup(world_edit.popup, XtGrabNone);
    return;
}


static void
create_agent_edit_widgets(top, object_location)
Widget top;
char *object_location;
{
    Arg args[10];
    register int i;
    Widget button;

    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                       i++;
    agent_edit.popup = XtCreatePopupShell("agentEditor", topLevelShellWidgetClass, top, args, i);
    
    agent_edit.form = XtCreateManagedWidget("agentForm", formWidgetClass, agent_edit.popup, NULL, 0);
    
    button = create_command_button("exit", "Exit", NULL, agent_edit.form);
    XtAddCallback(button, XtNcallback, save_agent_edit, (XtPointer) (FALSE));
    
    button = create_command_button("save", "Save", button, agent_edit.form);
    XtAddCallback(button, XtNcallback, save_agent_edit, (XtPointer) (TRUE));
    
    agent_edit.name_text = create_label_and_text("name", remove_underscores(NAME_STR), button, agent_edit.form);
    agent_edit.filename_text = create_label_and_text("filename", remove_underscores(FILENAME_STR), agent_edit.name_text, agent_edit.form);
    agent_edit.resource_text = create_label_and_text("resource", remove_underscores(TRADING_RESOURCE_STR), agent_edit.filename_text, agent_edit.form);
    agent_edit.reservoir_text = create_label_and_text("reservoir", remove_underscores(RESERVOIR_STR), agent_edit.resource_text, agent_edit.form);
    agent_edit.uptake_text = create_label_and_text("uptake", remove_underscores(UPTAKE_MASK_STR), agent_edit.reservoir_text, agent_edit.form);
    agent_edit.chromosome_text[0] = create_label_and_text("chromosome", remove_underscores(chromosome_names[0]), agent_edit.uptake_text, agent_edit.form);
    
    for (i = 1; i < NCHROMOSOMES; i++){
	agent_edit.chromosome_text[i] = create_label_and_text("chromosome", remove_underscores(chromosome_names[i]), 
			      agent_edit.chromosome_text[i - 1], agent_edit.form);
    } 
    
    agent_edit.agent_file_selector_popup = XtCreatePopupShell("agentSelector", topLevelShellWidgetClass, top, (ArgList) NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNcurrentDirectory, object_location ? "agents" : ".");                   i++;
    agent_edit.fs = XtCreateManagedWidget("agentFileSelector", fileSelectorWidgetClass, agent_edit.agent_file_selector_popup, args, i);
    XtAddCallback(agent_edit.fs, XtNcancelButtonCallback, popdown_widget, (XtPointer) agent_edit.agent_file_selector_popup);
    XtAddCallback(agent_edit.fs, XtNokButtonCallback, read_and_display_agent_edit, (XtPointer) agent_edit.agent_file_selector_popup);
    
    return;
}

static void
create_site_edit_widgets(top, object_location)
Widget top;
char *object_location;
{
    Arg args[10];
    register int i;
    Widget button;

    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                       i++;
    site_edit.agents_popup = XtCreatePopupShell("siteAgentEditor", topLevelShellWidgetClass, top, args, i);
    
    site_edit.agents_form = XtCreateManagedWidget("siteAgentForm", formWidgetClass, site_edit.agents_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, site_edit.agents_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) site_edit.agents_popup);
    
    button = create_command_button("clear", "Clear", button, site_edit.agents_form);
    XtAddCallback(button, XtNcallback, clear_text_widget, (XtPointer) &site_edit.agents_text);
    
    i = 0;
    XtSetArg(args[i], XtNtop, XawChainTop);                             i++;
    XtSetArg(args[i], "editType", XawtextEdit);                         i++;
    XtSetArg(args[i], "resize", XawtextResizeBoth);                     i++;
    XtSetArg(args[i], XtNresizable, TRUE);                              i++;
    XtSetArg(args[i], XtNfromVert, button);      i++;
    site_edit.agents_text = XtCreateManagedWidget("siteAgents", asciiTextWidgetClass, site_edit.agents_form, args, i);
    
    site_edit.popup = XtCreatePopupShell("siteEditor", topLevelShellWidgetClass, top, NULL, 0);
    site_edit.form = XtCreateManagedWidget("siteForm", formWidgetClass, site_edit.popup, NULL, 0);
    
    button = create_command_button("exit", "Exit", NULL, site_edit.form);
    XtAddCallback(button, XtNcallback, save_site_edit, (XtPointer) (FALSE));
    
    button = create_command_button("save", "Save", button, site_edit.form);
    XtAddCallback(button, XtNcallback, save_site_edit, (XtPointer) (TRUE));
    
    button = create_command_button("agents", "Agents", button, site_edit.form);
    XtAddCallback(button, XtNcallback, popup_widget, (XtPointer) site_edit.agents_popup);
    
    site_edit.name_text = create_label_and_text("name", remove_underscores(NAME_STR), button, site_edit.form);
    site_edit.filename_text = create_label_and_text("filename", remove_underscores(FILENAME_STR), agent_edit.name_text, site_edit.form);
    site_edit.mutation_text = create_label_and_text("mutation", remove_underscores(MUTATION_PROB_STR), agent_edit.filename_text, site_edit.form);
    site_edit.crossover_text = create_label_and_text("crossover", remove_underscores(CROSSOVER_PROB_STR), site_edit.mutation_text, site_edit.form);
    site_edit.death_text = create_label_and_text("death", remove_underscores(RANDOM_DEATH_PROB_STR), site_edit.crossover_text, site_edit.form);
    site_edit.production_text = create_label_and_text("production", remove_underscores(PRODUCTION_FUNCTION_STR), site_edit.death_text, site_edit.form);
    site_edit.resources_text = create_label_and_text("resources", remove_underscores(SITE_RESOURCES_STR), site_edit.production_text, site_edit.form);
    site_edit.maximums_text = create_label_and_text("maximums", remove_underscores(MAXIMUMS_STR), site_edit.resources_text, site_edit.form);
    site_edit.maintenance_text = create_label_and_text("maintenance", remove_underscores(MAINTENANCE_STR), site_edit.maximums_text, site_edit.form);
    
    site_edit.site_file_selector_popup = XtCreatePopupShell("siteSelector", topLevelShellWidgetClass, top, (ArgList) NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNcurrentDirectory, object_location ? "sites" : ".");                   i++;
    site_edit.fs = XtCreateManagedWidget("siteFileSelector", fileSelectorWidgetClass, site_edit.site_file_selector_popup, args, i);
    XtAddCallback(site_edit.fs, XtNcancelButtonCallback, popdown_widget, (XtPointer) site_edit.site_file_selector_popup);
    XtAddCallback(site_edit.fs, XtNokButtonCallback, read_and_display_site_edit, (XtPointer) site_edit.site_file_selector_popup);
    
    return;
}

static void
create_world_edit_widgets(top, object_location)
Widget top;
char *object_location;
{
    Arg args[10];
    register int i;
    Widget button;
    
    /* WORLD EDITOR POPUP WIDGETS. */
    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                              i++;
    world_edit.sites_popup = XtCreatePopupShell("worldSiteEditor", topLevelShellWidgetClass, top, args, i);
    
    world_edit.sites_form = XtCreateManagedWidget("worldSiteForm", formWidgetClass, world_edit.sites_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, world_edit.sites_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) world_edit.sites_popup);
    
    button = create_command_button("clear", "Clear", button, world_edit.sites_form);
    XtAddCallback(button, XtNcallback, clear_text_widget, (XtPointer) &world_edit.sites_text);
    
    i = 0;
    XtSetArg(args[i], XtNtop, XawChainTop);                             i++;
    XtSetArg(args[i], "editType", XawtextEdit);                         i++;
    XtSetArg(args[i], "resize", XawtextResizeBoth);                     i++;
    XtSetArg(args[i], XtNresizable, TRUE);                              i++;
    XtSetArg(args[i], XtNfromVert, button);      i++;
    world_edit.sites_text = XtCreateManagedWidget("siteAgents", asciiTextWidgetClass, world_edit.sites_form, args, i);
    
    /* COMBAT MATRIX POPUP WIDGETS. */
    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                              i++;
    world_edit.combat_popup = XtCreatePopupShell("worldCombatEditor", topLevelShellWidgetClass, top, args, i);
    
    world_edit.combat_form = XtCreateManagedWidget("worldCombatForm", formWidgetClass, world_edit.combat_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, world_edit.combat_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) world_edit.combat_popup);
    
    button = create_command_button("clear", "Clear", button, world_edit.combat_form);
    XtAddCallback(button, XtNcallback, clear_text_widget, (XtPointer) &world_edit.combat_text);
    
    i = 0;
    XtSetArg(args[i], XtNtop, XawChainTop);                             i++;
    XtSetArg(args[i], "editType", XawtextEdit);                         i++;
    XtSetArg(args[i], "resize", XawtextResizeBoth);                     i++;
    XtSetArg(args[i], XtNresizable, TRUE);                              i++;
    XtSetArg(args[i], XtNfromVert, button);      i++;
    world_edit.combat_text = XtCreateManagedWidget("worldCombatMatrix", asciiTextWidgetClass, world_edit.combat_form, args, i);
    
    /* WORLD EDITOR WIDGETS. */
    world_edit.popup = XtCreatePopupShell("worldEditor", topLevelShellWidgetClass, top, NULL, 0);
    world_edit.form = XtCreateManagedWidget("worldForm", formWidgetClass, world_edit.popup, NULL, 0);
    
    button = create_command_button("exit", "Exit", NULL, world_edit.form);
    XtAddCallback(button, XtNcallback, save_world_edit, (XtPointer) (FALSE));
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) world_edit.sites_popup);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) world_edit.combat_popup);
    
    button = create_command_button("save", "Save", button, world_edit.form);
    XtAddCallback(button, XtNcallback, save_world_edit, (XtPointer) (TRUE));
    
    button = create_command_button("sites", "Sites", button, world_edit.form);
    XtAddCallback(button, XtNcallback, popup_widget, (XtPointer) world_edit.sites_popup);
    
    button = create_command_button("combatMatrix", "Combat Matrix", button, world_edit.form);
    XtAddCallback(button, XtNcallback, popup_widget, (XtPointer) world_edit.combat_popup);
    
    world_edit.name_text = create_label_and_text("name", remove_underscores(NAME_STR), button, world_edit.form);
    world_edit.filename_text = create_label_and_text("filename", remove_underscores(FILENAME_STR), world_edit.name_text, world_edit.form);
    world_edit.nresources_text = create_label_and_text("nresources", remove_underscores(NRESOURCES_STR), world_edit.filename_text, world_edit.form);
    world_edit.rows_text = create_label_and_text("rows", remove_underscores(ROWS_STR), world_edit.nresources_text, world_edit.form);
    world_edit.cols_text = create_label_and_text("cols", remove_underscores(COLS_STR), world_edit.rows_text, world_edit.form);
    world_edit.trade_text = create_label_and_text("trade", remove_underscores(TRADE_STR), world_edit.cols_text, world_edit.form);
    world_edit.c_text = create_label_and_text("c", remove_underscores(C_STR), world_edit.trade_text, world_edit.form);
    world_edit.selfrep_fraction_text = create_label_and_text("selfrepFraction", remove_underscores(SELFREP_FRACTION_STR), world_edit.c_text, world_edit.form);
    world_edit.selfrep_threshold_text = create_label_and_text("selfrepThreshold", remove_underscores(SELFREP_THRESHOLD_STR), world_edit.selfrep_fraction_text, world_edit.form);
    world_edit.p_maintenance_text = create_label_and_text("maintenance", remove_underscores(M_STR), world_edit.selfrep_threshold_text, world_edit.form);
    world_edit.neighborhood_text = create_label_and_text("neighborhood", remove_underscores(NEIGHBORHOOD_STR), world_edit.p_maintenance_text, world_edit.form);
    
    world_edit.world_file_selector_popup = XtCreatePopupShell("worldSelector", topLevelShellWidgetClass, top, (ArgList) NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNcurrentDirectory, object_location ? "worlds" : ".");                   i++;
    world_edit.fs = XtCreateManagedWidget("worldFileSelector", fileSelectorWidgetClass, world_edit.world_file_selector_popup, args, i);
    XtAddCallback(world_edit.fs, XtNcancelButtonCallback, popdown_widget, (XtPointer) world_edit.world_file_selector_popup);
    XtAddCallback(world_edit.fs, XtNokButtonCallback, read_and_display_world_edit, (XtPointer) world_edit.world_file_selector_popup);
    
    return;
}

static void
create_control_menu_widgets(top, real_top)
Widget top;
Widget real_top;
{
    register int i;
    Arg args[10];
    Widget button;

    control_menu_widgets.verbose_selector_popup = XtCreatePopupShell("verboseSelector", topLevelShellWidgetClass, top, NULL, 0);
    control_menu_widgets.verbose_form = XtCreateManagedWidget("form", formWidgetClass, 
								 control_menu_widgets.verbose_selector_popup, NULL, 0);
    button = create_command_button("ok", "Ok", NULL, control_menu_widgets.verbose_form);
    XtAddCallback(button, XtNcallback, set_verbosity, (XtPointer) control_menu_widgets.verbose_selector_popup);
    
    button = create_command_button("cancel", "Cancel", button, control_menu_widgets.verbose_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) control_menu_widgets.verbose_selector_popup);
    
    button = create_command_button("clear", "Clear", button, control_menu_widgets.verbose_form);
    XtAddCallback(button, XtNcallback, clear_dialog_widget, (XtPointer) &control_menu_widgets.verbose_dialog);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Verbose Options:\n\n\
b Show Bankruptcy\n\
c Show who will Combat\n\
C Show Combat details\n\
d Show agents that are Dead\n\
D Show Deaths\n\
g Show the Generation count\n\
i Show who will Interact\n\
I Show Interactions & locations\n\
k Show kills\n\
l List the agent stack\n\
m Show Migration\n\
M Show who will Mate\n\
p Show Population summary\n\
r Show who will Replicate\n\
R Show details of replication\n\
s Show Species\n\
S Show details of sites\n\
t Show details of trade\n\
u Show mUtations\n\
x Show taXation\n\
X Show crossover details\n\
z Show Zaps");                                  i++;
    XtSetArg(args[i], XtNfromVert, button);          i++;
    control_menu_widgets.verbose_dialog = XtCreateManagedWidget("verbose", dialogWidgetClass, control_menu_widgets.verbose_form, args, i);
    left_justify_dialog_label(control_menu_widgets.verbose_dialog);
    
    i = 0;
    XtSetArg(args[i], XtNleft, XawChainLeft);       i++;
    XtSetArg(args[i], XtNright, XawChainLeft);      i++;
    XtSetArg(args[i], XtNtop, XawChainTop);         i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);      i++;
    XtSetArg(args[i], XtNmenuName, "controlMenu");  i++;
    
    control_menu_widgets.menu_button = XtCreateManagedWidget("controlMenuButton", menuButtonWidgetClass, top, args, i);
    control_menu_widgets.menu = XtCreatePopupShell("controlMenu", simpleMenuWidgetClass, control_menu_widgets.menu_button, NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Set Verbose Level...");                         i++;
    control_menu_widgets.entry = XtCreateManagedWidget("Verbose", smeBSBObjectClass, control_menu_widgets.menu, args, i);
    XtAddCallback(control_menu_widgets.entry, XtNcallback, popup_verbose_selector, (XtPointer) 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Show Seed");                         i++;
    control_menu_widgets.entry = XtCreateManagedWidget("Seed", smeBSBObjectClass, control_menu_widgets.menu, args, i);
    XtAddCallback(control_menu_widgets.entry, XtNcallback, show_seed, (XtPointer) 0);
    
    control_menu_widgets.entry = XtCreateManagedWidget("Exit", smeBSBObjectClass, control_menu_widgets.menu, NULL, 0);
    XtAddCallback(control_menu_widgets.entry, XtNcallback, quit, NULL);
    
    return;
}

static void
show_seed()
{
    printf("The seed is %ld\n", old_seed);
    return;
}


static void
create_edit_menu_widgets(top)
Widget top;
{
    register int i;
    Arg args[20];
    Widget button;
    static String template;
    extern String mktemp();
    char chrom_description[4096];
    
#define TEMPLATE_NAME "/tmp/echo_stack_XXXXXX"
    template = Malloc(strlen(TEMPLATE_NAME) + 1);
    template[0] = '\0';
    strcat(template, TEMPLATE_NAME);
#undef TEMPLATE_NAME

    edit_menu_widgets.stack_file = mktemp(template);
    
    if (!strlen(template)){
	error("mktemp() cannot allocate temporary filename in create_edit_menu_widgets().");
    }

    edit_menu_widgets.stack_selector_popup = XtCreatePopupShell("stackSelector", topLevelShellWidgetClass, top, NULL, 0);
    edit_menu_widgets.stack_selector_form = XtCreateManagedWidget("form", formWidgetClass, 
								 edit_menu_widgets.stack_selector_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, edit_menu_widgets.stack_selector_form);
    XtAddCallback(button, XtNcallback, select_stack_for_editing, (XtPointer) edit_menu_widgets.stack_selector_popup);
    
    button = create_command_button("cancel", "Cancel", button, edit_menu_widgets.stack_selector_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) edit_menu_widgets.stack_selector_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Edit the stack at which site?");            i++;
    XtSetArg(args[i], XtNfromVert, button);                                  i++;
    edit_menu_widgets.stack_selector_dialog = XtCreateManagedWidget("stackSelectorDialog", dialogWidgetClass, 
								   edit_menu_widgets.stack_selector_form, args, i);
    left_justify_dialog_label(edit_menu_widgets.stack_selector_dialog);


    edit_menu_widgets.stack_help_popup = XtCreatePopupShell("stackDumpHelp", topLevelShellWidgetClass, top, NULL, 0);
    edit_menu_widgets.stack_help_form = XtCreateManagedWidget("stackDumpHelpForm", formWidgetClass, edit_menu_widgets.stack_help_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, edit_menu_widgets.stack_help_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) edit_menu_widgets.stack_help_popup);
    
    sprintf(chrom_description, "The genome contains:\n\n\
    - The %d tags and conditions.\n\
    - The trading resource.\n\
    - The uptake mask.\n\n\
These fields are separated by underscores.\n\nThe tags and conditions (in order) are:\n\n", NCHROMOSOMES);

    for (i = 0; i < NCHROMOSOMES; i++){
	sprintf(chrom_description, "%s    %s\n", chrom_description, remove_underscores(chromosome_names[i]));
    } 

    i = 0;
    XtSetArg(args[i], XtNlabel, chrom_description);    i++;
    XtSetArg(args[i], XtNtop, XawChainTop);            i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);         i++;
    XtSetArg(args[i], "right", XawChainLeft);          i++;
    XtSetArg(args[i], "left", XawChainLeft);           i++;
    XtSetArg(args[i], XtNfromVert, button);            i++;
    XtSetArg(args[i], XtNjustify, XtJustifyLeft);      i++;
    button = XtCreateManagedWidget("label", labelWidgetClass, edit_menu_widgets.stack_help_form, args, i);
    
    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                       i++;
    edit_menu_widgets.stack_popup = XtCreatePopupShell("stackDump", topLevelShellWidgetClass, top, args, i);
    edit_menu_widgets.stack_form = XtCreateManagedWidget("stackDumpForm", formWidgetClass, edit_menu_widgets.stack_popup, NULL, 0);
    
    
    button = create_command_button("cancel", "Cancel", NULL, edit_menu_widgets.stack_form);
    XtAddCallback(button, XtNcallback, popdown_stack, (XtPointer) edit_menu_widgets.stack_popup);
    
    button = create_command_button("amen", "Amen", button, edit_menu_widgets.stack_form);
    XtAddCallback(button, XtNcallback, resume_from_stack, (XtPointer) edit_menu_widgets.stack_popup);
    
    button = create_command_button("help", "Help", button, edit_menu_widgets.stack_form);
    XtAddCallback(button, XtNcallback, popup_widget, (XtPointer) edit_menu_widgets.stack_help_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Agent    Genome               Makeup     Reserve");        i++;
    XtSetArg(args[i], XtNtop, XawChainTop);            i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);         i++;
    XtSetArg(args[i], "right", XawChainLeft);          i++;
    XtSetArg(args[i], "left", XawChainLeft);           i++;
    XtSetArg(args[i], XtNfromVert, button);            i++;
    XtSetArg(args[i], XtNjustify, XtJustifyLeft);      i++;
    button = XtCreateManagedWidget("label", labelWidgetClass, edit_menu_widgets.stack_form, args, i);
    
    i = 0;
    XtSetArg(args[i], XtNtop, XawChainTop);                      i++;
    XtSetArg(args[i], XtNbottom, XawChainBottom);                i++;
    XtSetArg(args[i], XtNleft, XawChainLeft);                    i++;
    XtSetArg(args[i], XtNright, XawChainRight);                  i++;
    XtSetArg(args[i], "allowHoriz", TRUE);                       i++;
    XtSetArg(args[i], "allowVert", TRUE);                        i++;
    XtSetArg(args[i], "useBottom", TRUE);                        i++;
    XtSetArg(args[i], "useRight", TRUE);                         i++;
    /* XtSetArg(args[i], "forceBars", TRUE);                        i++; */
    XtSetArg(args[i], XtNfromVert, button);                      i++;
    XtSetArg(args[i], XtNresizable, TRUE);                       i++;
    edit_menu_widgets.stack_viewport = XtCreateManagedWidget("viewport", viewportWidgetClass, edit_menu_widgets.stack_form, args, i);
    
    i = 0;
    XtSetArg(args[i], "editType", XawtextEdit);                         i++;
    XtSetArg(args[i], "type", XawAsciiFile);                            i++;
    edit_menu_widgets.stack_text = XtCreateManagedWidget("stackEditor", asciiTextWidgetClass, edit_menu_widgets.stack_viewport, args, i);
    

    i = 0;
    XtSetArg(args[i], XtNleft, XawChainLeft);                            i++;
    XtSetArg(args[i], XtNright, XawChainLeft);                           i++;
    XtSetArg(args[i], XtNtop, XawChainTop);                              i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);                           i++;
    XtSetArg(args[i], XtNfromHoriz, control_menu_widgets.menu_button);   i++;
    XtSetArg(args[i], XtNmenuName, "editMenu");                          i++;
    
    edit_menu_widgets.menu_button = XtCreateManagedWidget("editMenuButton", menuButtonWidgetClass, top, args, i);
    edit_menu_widgets.menu = XtCreatePopupShell("editMenu", simpleMenuWidgetClass, edit_menu_widgets.menu_button, NULL, 0);
    
    edit_menu_widgets.entry = XtCreateManagedWidget("Worlds", smeBSBObjectClass, edit_menu_widgets.menu, NULL, 0);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, popup_widget, (XtPointer) world_edit.world_file_selector_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "New World");                         i++;
    edit_menu_widgets.entry = XtCreateManagedWidget("newWorld", smeBSBObjectClass, edit_menu_widgets.menu, args, i);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, read_and_display_world_edit, (XtPointer) 0);
	
    edit_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, edit_menu_widgets.menu, NULL, ZERO);
    
    edit_menu_widgets.entry = XtCreateManagedWidget("Sites", smeBSBObjectClass, edit_menu_widgets.menu, NULL, 0);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, popup_widget, (XtPointer) site_edit.site_file_selector_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "New Site");                         i++;
    edit_menu_widgets.entry = XtCreateManagedWidget("newSite", smeBSBObjectClass, edit_menu_widgets.menu, args, i);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, read_and_display_site_edit, (XtPointer) 0);
	
    edit_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, edit_menu_widgets.menu, NULL, ZERO);
    
    edit_menu_widgets.entry = XtCreateManagedWidget("Agents", smeBSBObjectClass, edit_menu_widgets.menu, NULL, 0);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, popup_widget, (XtPointer) agent_edit.agent_file_selector_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "New Agent");                         i++;
    edit_menu_widgets.entry = XtCreateManagedWidget("newAgent", smeBSBObjectClass, edit_menu_widgets.menu, args, i);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, read_and_display_agent_edit, (XtPointer) 0);
	
    edit_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, edit_menu_widgets.menu, NULL, ZERO);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Stack");                         i++;
    edit_menu_widgets.entry = XtCreateManagedWidget("DumpStack", smeBSBObjectClass, edit_menu_widgets.menu, args, i);
    XtAddCallback(edit_menu_widgets.entry, XtNcallback, popup_stack_selector, (XtPointer) 0);
    
    return;
}

static void
create_running_menu_widgets(top, object_location)
Widget top;
char *object_location;
{
    register int i;
    Arg args[10];
    Widget button;
    
    running_menu_widgets.world_file_selector_popup = XtCreatePopupShell("worldSelector", topLevelShellWidgetClass, top, NULL, 0);
    running_menu_widgets.generation_selector_popup = XtCreatePopupShell("generationSelector", topLevelShellWidgetClass, top, NULL, 0);
    running_menu_widgets.generation_form = XtCreateManagedWidget("form", formWidgetClass, 
								 running_menu_widgets.generation_selector_popup, NULL, 0);
    running_menu_widgets.seed_popup = XtCreatePopupShell("seedSelector", topLevelShellWidgetClass, top, NULL, 0);
    running_menu_widgets.seed_form = XtCreateManagedWidget("form", formWidgetClass, running_menu_widgets.seed_popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, running_menu_widgets.generation_form);
    XtAddCallback(button, XtNcallback, call_set_halt_generation, (XtPointer) running_menu_widgets.generation_selector_popup);
    
    button = create_command_button("cancel", "Cancel", button, running_menu_widgets.generation_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) running_menu_widgets.generation_selector_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Generation to stop after?");            i++;
    XtSetArg(args[i], XtNfromVert, button);          i++;
    running_menu_widgets.generation_dialog = XtCreateManagedWidget("generation", dialogWidgetClass, 
								   running_menu_widgets.generation_form, args, i);
    left_justify_dialog_label(running_menu_widgets.generation_dialog);
    
    button = create_command_button("ok", "Ok", NULL, running_menu_widgets.seed_form);
    XtAddCallback(button, XtNcallback, set_seed, (XtPointer) running_menu_widgets.seed_popup);
    
    button = create_command_button("cancel", "Cancel", button, running_menu_widgets.seed_form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) running_menu_widgets.seed_popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Random seed value?");                   i++;
    XtSetArg(args[i], XtNfromVert, button);          i++;
    running_menu_widgets.seed_dialog = XtCreateManagedWidget("seed", dialogWidgetClass, running_menu_widgets.seed_form, args, i);
    left_justify_dialog_label(running_menu_widgets.seed_dialog);
    
    i = 0;
    XtSetArg(args[i], XtNcurrentDirectory, object_location ? "worlds" : ".");                   i++;
    running_menu_widgets.fs = XtCreateManagedWidget("WorldSelector", fileSelectorWidgetClass, 
						    running_menu_widgets.world_file_selector_popup, args, i);
    XtAddCallback(running_menu_widgets.fs, XtNcancelButtonCallback, popdown_widget, 
		  (XtPointer) running_menu_widgets.world_file_selector_popup);
    XtAddCallback(running_menu_widgets.fs, XtNokButtonCallback, call_instantiate_world, 
		  (XtPointer) running_menu_widgets.world_file_selector_popup);

    i = 0;
    XtSetArg(args[i], XtNleft, XawChainLeft);                            i++;
    XtSetArg(args[i], XtNright, XawChainLeft);                           i++;
    XtSetArg(args[i], XtNtop, XawChainTop);                              i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);                           i++;
    XtSetArg(args[i], XtNfromHoriz, edit_menu_widgets.menu_button);      i++;
    XtSetArg(args[i], XtNmenuName, "runningMenu");                       i++;
    
    running_menu_widgets.menu_button = XtCreateManagedWidget("runningMenuButton", menuButtonWidgetClass, top, args, i);
    running_menu_widgets.menu = XtCreatePopupShell("runningMenu", simpleMenuWidgetClass, running_menu_widgets.menu_button, NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Choose World");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Choose_world", smeBSBObjectClass, running_menu_widgets.menu, args, i);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, popup_widget, (XtPointer) running_menu_widgets.world_file_selector_popup);
	
    running_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, running_menu_widgets.menu, NULL, ZERO);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Run Indefinitely");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Run_Indefinitely", smeBSBObjectClass, running_menu_widgets.menu, args, i);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, call_run_world_indefinitely, (XtPointer) 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Run 1 Generation");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Run_1", smeBSBObjectClass, running_menu_widgets.menu, args, i);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, call_run_for_x_generations, (XtPointer) 1);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Run 10 Generations");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Run_10", smeBSBObjectClass, running_menu_widgets.menu, args, i);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, call_run_for_x_generations, (XtPointer) 10);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Run Until...");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Until", smeBSBObjectClass, running_menu_widgets.menu, args, i);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, popup_generation_selector, (XtPointer) 0);
    
    running_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, running_menu_widgets.menu, NULL, ZERO);
    
    running_menu_widgets.entry = XtCreateManagedWidget("Pause", smeBSBObjectClass, running_menu_widgets.menu, NULL, 0);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, call_pause_world, (XtPointer) 0);
    
    running_menu_widgets.entry = XtCreateManagedWidget("Continue", smeBSBObjectClass, running_menu_widgets.menu, NULL, 0);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, call_continue_world, (XtPointer) 0);
    
    running_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, running_menu_widgets.menu, NULL, ZERO);
	
    i = 0;
    XtSetArg(args[i], XtNlabel, "Replay Run");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Replay", smeBSBObjectClass, running_menu_widgets.menu, NULL, 0);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, rerun_world, (XtPointer) 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Set Seed...");                         i++;
    running_menu_widgets.entry = XtCreateManagedWidget("Seed", smeBSBObjectClass, running_menu_widgets.menu, NULL, 0);
    XtAddCallback(running_menu_widgets.entry, XtNcallback, popup_seed_selector, (XtPointer) 0);
    
    return;
}

static void
create_graph_widgets(top)
Widget top;
{
    Widget entry;
    register int i;
    Arg args[10];
    
    /* Make the popup and the form to hold buttons and the plotter. */

    population_graph = create_plotter("Worldwide Population Level", "Time", "Level", "populationGraph",
				      400, 100, 0.0, 400.0, 0.0, 200.0, top, 0);
    species_graph =    create_plotter("Species Level", "Time", "Level", "speciesGraph",
				      400, 100, 0.0, 400.0, 0.0, 200.0, top, 0);
    resources_graph =  create_plotter("Resource Levels", "Time", "Level", "resourcesGraph",
				      400, 100, 0.0, 400.0, 0.0, 400.0, top, 0);
    schema_graph =     create_plotter("Schema Level", "Time", "Level", "schemaGraph",
				      400, 100, 0.0, 400.0, 0.0, 100.0, top, 0);
    variant_graph =    create_plotter("Variant Levels", "Time", "Level", "variantGraph",
				      400, 100, 0.0, 400.0, 0.0, 100.0, top, 0);
    
    /* Create the lines for the graphs (those that we already know about). */
    
    add_line_to_plotter(population_graph, "line", "Population", FALSE);
    add_line_to_plotter(schema_graph, "line", NULL, FALSE);
    add_line_to_plotter(variant_graph, "alive_line", "Living Variants", FALSE);
    add_line_to_plotter(variant_graph, "total_line", "Total Variants", FALSE);

    /* Now make the menu for the graphs. */
    i = 0;
    XtSetArg(args[i], XtNleft, XawChainLeft);                            i++;
    XtSetArg(args[i], XtNright, XawChainLeft);                           i++;
    XtSetArg(args[i], XtNtop, XawChainTop);                              i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);                           i++;
    XtSetArg(args[i], XtNfromHoriz, running_menu_widgets.menu_button);   i++;
    XtSetArg(args[i], XtNmenuName, "graphMenu");                         i++;
    
    graph_menu_widgets.menu_button = XtCreateManagedWidget("graphsMenuButton", menuButtonWidgetClass, top, args, i);
    graph_menu_widgets.menu = XtCreatePopupShell("graphMenu", simpleMenuWidgetClass, graph_menu_widgets.menu_button, NULL, 0);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "World Population Level");                         i++;
    entry = XtCreateManagedWidget("populationLevel", smeBSBObjectClass, graph_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) population_graph->popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Species Levels");                                 i++;
    entry = XtCreateManagedWidget("speciesLevels", smeBSBObjectClass, graph_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) species_graph->popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "World Resource Levels");                         i++;
    entry = XtCreateManagedWidget("resourceLevels", smeBSBObjectClass, graph_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) resources_graph->popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Schema Level");                                  i++;
    entry = XtCreateManagedWidget("schemaLevel", smeBSBObjectClass, graph_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) schema_graph->popup);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Variant Levels");                                  i++;
    entry = XtCreateManagedWidget("variantLevel", smeBSBObjectClass, graph_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) variant_graph->popup);
    
    return;
}

static void
create_schema_widgets(top)
Widget top;
{
    Widget entry;
    Widget button;
    Arg args[10];
    int i;
    
    schema_menu_widgets.schema = NULL;
    
    schema_menu_widgets.popup = XtCreatePopupShell("schemaSelector", topLevelShellWidgetClass, top, NULL, 0);
    schema_menu_widgets.form = XtCreateManagedWidget("form", formWidgetClass, schema_menu_widgets.popup, NULL, 0);
    
    button = create_command_button("ok", "Ok", NULL, schema_menu_widgets.form);
    XtAddCallback(button, XtNcallback, set_new_schema, (XtPointer) schema_menu_widgets.popup);
    
    button = create_command_button("cancel", "Cancel", button, schema_menu_widgets.form);
    XtAddCallback(button, XtNcallback, popdown_widget, (XtPointer) schema_menu_widgets.popup);
    
    button = create_command_button("clear", "Clear", button, schema_menu_widgets.form);
    XtAddCallback(button, XtNcallback, clear_dialog_widget, (XtPointer) &schema_menu_widgets.schema_dialog);
    
    i = 0;
    XtSetArg(args[i], XtNlabel, "Schema to trace?");            i++;
    XtSetArg(args[i], XtNfromVert, button);                     i++;
    schema_menu_widgets.schema_dialog = XtCreateManagedWidget("generation", dialogWidgetClass, schema_menu_widgets.form, args, i);
    left_justify_dialog_label(schema_menu_widgets.schema_dialog);
    set_dialog_value(schema_menu_widgets.schema_dialog, "");
    
    
    /* Make the menu for the schema things. */
    i = 0;
    XtSetArg(args[i], XtNleft, XawChainLeft);                            i++;
    XtSetArg(args[i], XtNright, XawChainLeft);                           i++;
    XtSetArg(args[i], XtNtop, XawChainTop);                              i++;
    XtSetArg(args[i], XtNbottom, XawChainTop);                           i++;
    XtSetArg(args[i], XtNfromHoriz, graph_menu_widgets.menu_button);     i++;
    XtSetArg(args[i], XtNmenuName, "graphMenu");                         i++;
    
    schema_menu_widgets.menu_button = XtCreateManagedWidget("schemaMenuButton", menuButtonWidgetClass, top, args, i);
    schema_menu_widgets.menu = XtCreatePopupShell("graphMenu", simpleMenuWidgetClass, schema_menu_widgets.menu_button, NULL, 0);

#ifndef NO_TCL
    i = 0;
    XtSetArg(args[i], XtNlabel, "Population Viewer");               i++;
    entry = XtCreateManagedWidget("populationViewer", smeBSBObjectClass, schema_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, call_pop_viewer, (XtPointer) NULL);
#endif

    i = 0;
    XtSetArg(args[i], XtNlabel, "Choose Schema to Graph...");               i++;
    entry = XtCreateManagedWidget("schemaLevel", smeBSBObjectClass, schema_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, popup_widget, (XtPointer) schema_menu_widgets.popup);

    i = 0;
    XtSetArg(args[i], XtNlabel, "Cluster living population.");               i++;
    entry = XtCreateManagedWidget("cluster", smeBSBObjectClass, schema_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, cluster, (XtPointer) TRUE);

    i = 0;
    XtSetArg(args[i], XtNlabel, "Cluster all individuals.");               i++;
    entry = XtCreateManagedWidget("cluster", smeBSBObjectClass, schema_menu_widgets.menu, args, i);
    XtAddCallback(entry, XtNcallback, cluster, (XtPointer) FALSE);

    return;
}

#ifndef NO_CANVAS
void
create_canvas_widgets(nrows, ncols)
int nrows;
int ncols;
{
    register int i;
    register int row;
    register int col;
    Arg args[10];

#ifdef CHECK_ARGS
    if (ncols * nrows <= 0 || (ncols < 0 && nrows < 0)){
	error("create_canvas_widgets() called with bad row and column numbers (%d, %d)!", nrows, ncols);
    }
#endif /* CHECK_ARGS */

    /* The next line knows that canvas_widgets_parent is really a form... */
    XawFormDoLayout(canvas_widgets_parent, FALSE);
    
    canvas_widgets = (Widget **) XtMalloc(nrows * sizeof(Widget *));

    for (i = 0; i < nrows; i++){
	canvas_widgets[i] = (Widget *) XtMalloc(ncols * sizeof(Widget));
    } 

    i = 0;
    XtSetArg(args[i], XtNfromVert, control_menu_widgets.menu_button);     i++;
    canvas_widgets[0][0] = XtCreateManagedWidget("canvas", canvasWidgetClass, canvas_widgets_parent, args, i);
    
    XtSetArg(args[0], XtNfromVert, control_menu_widgets.menu_button);
    
    for (col = 1; col < ncols; col++){
	XtSetArg(args[1], XtNfromHoriz, canvas_widgets[0][col - 1]);
	canvas_widgets[0][col] = XtCreateManagedWidget("canvas", canvasWidgetClass, canvas_widgets_parent, args, 2);
    } 
    
    for (row = 1; row < nrows; row++){
	XtSetArg(args[0], XtNfromVert, canvas_widgets[row - 1][0]);
	canvas_widgets[row][0] = XtCreateManagedWidget("canvas", canvasWidgetClass, canvas_widgets_parent, args, 1);
	
	for (col = 1; col < ncols; col++){
	    XtSetArg(args[1], XtNfromHoriz, canvas_widgets[row][col - 1]);
	    canvas_widgets[row][col] = XtCreateManagedWidget("canvas", canvasWidgetClass, canvas_widgets_parent, args, 2);
	} 
    } 
    
    /* The next line knows that canvas_widgets_parent is really a form... */
    XawFormDoLayout(canvas_widgets_parent, TRUE);
    return;
}

void
destroy_canvas_widgets(nrows, ncols)
int nrows;
int ncols;
{
    register int row;
    register int col;

#ifdef CHECK_ARGS
    if (ncols * nrows <= 0 || (ncols < 0 && nrows < 0)){
	error("create_canvas_widgets() called with bad row and column numbers (%d, %d)!", nrows, ncols);
    }
#endif /* CHECK_ARGS */

    /* The next line knows that canvas_widgets_parent is really a form... */
    XawFormDoLayout(canvas_widgets_parent, FALSE);
    
    for (row = 0; row < nrows; row++){
	for (col = 0; col < ncols; col++){
	    XtDestroyWidget(canvas_widgets[row][col]);
	}
    }

    /* The next line knows that canvas_widgets_parent is really a form... */
    XawFormDoLayout(canvas_widgets_parent, TRUE);
    return;
}
#endif


static void
create_message_widgets(top)
Widget top;
{
    register int i;
    Arg args[10];
    
    i = 0;
    XtSetArg(args[i], XtNallowShellResize, TRUE);                              i++;
    
    message_widgets.popup = XtCreatePopupShell("messagePopup", topLevelShellWidgetClass, top, args, i);
    message_widgets.dialog = XtCreateManagedWidget("message", dialogWidgetClass, message_widgets.popup, NULL, 0);
    left_justify_dialog_label(message_widgets.dialog);
    
    XawDialogAddButton(message_widgets.dialog, "OK", popdown_widget, (XtPointer) message_widgets.popup);
    
    return;
}

void
popup_message(u, v, w, x, y, z)
char *u, *v, *w, *x, *y, *z;
{
    Arg args[1];
    char message[8092];

    sprintf(message, u, v, w, x, y, z);
    
    XtSetArg(args[0], XtNlabel,  message);
    XtSetValues(message_widgets.dialog, args, 1);
    
    XBell(XtDisplay(message_widgets.dialog), 0);
    
    XtPopup(message_widgets.popup, XtGrabNonexclusive);
    return;
}

    
static void 
save_agent_edit(w, save, dummy)
Widget w;
XtPointer save;
XtPointer dummy;
{
    /*
     * Get all the values from the dialog and so on widgets that we used
     * to let the user edit an agent. If they are different from the original
     * values, free the old ones and insert the new ones. Then write the
     * new edit structure out to a file whose name is determined by the contents
     * of part of the structure we are retrieving.
     */
    
    /* this function should call free_agent_edit before it returns (if it does a popdown). */

    Arg args[10];
    String value;
    register int i;
    register char *tmp;

    if ((BOOLEAN)save == FALSE){
	if (agent_edit.agent){
	    free_agent_edit(agent_edit.agent);
	}
	XtPopdown(agent_edit.popup);
	return;
    }
    
    /*
     * Set the agent's characteristics from the widgets on the screen
     * and then write it back to the file.
     */
     
    XtSetArg(args[0], XtNstring, &value);
    XtGetValues(agent_edit.name_text, args, 1);
    
    if (!strlen(value)){
	popup_message("You must give your agent a name before it can be saved.");
	return;
    }

    if (strcmp(value, AGENT_EDIT_NAME(agent_edit.agent))){
	Free(AGENT_EDIT_NAME(agent_edit.agent));
	AGENT_EDIT_NAME(agent_edit.agent) = strdup(value);
    }
    
    value = get_text_widget_text(agent_edit.resource_text);
    
    if (!IS_VALID_RESOURCE(*value)){
	popup_message("The character '%c' is not a valid trading resource!", *value);
	return;
    }
    
    if (strlen(value) != 1){
	popup_message("The trading resource must be exactly one character!");
	return;
    }

    if (strcmp(AGENT_EDIT_TRADING_RESOURCE(agent_edit.agent), value)){
	Free(AGENT_EDIT_TRADING_RESOURCE(agent_edit.agent));
	AGENT_EDIT_TRADING_RESOURCE(agent_edit.agent) = strdup(value);
    }
    
    value = get_text_widget_text(agent_edit.uptake_text);
    
    if (strlen(value) > MAX_RESOURCES){
	popup_message("The uptake mask must be <= %d chars long!\n", MAX_RESOURCES);
	return;
    }

    if (strcmp(AGENT_EDIT_UPTAKE_MASK(agent_edit.agent), value)){
	tmp = value;
    
	while (*tmp){
	    if (*tmp != '0' && *tmp != '1'){
		popup_message("The resource uptake mask must contain only zeroes and ones.");
		return;
	    }
	    tmp++;
	}
    
	Free(AGENT_EDIT_UPTAKE_MASK(agent_edit.agent));
	AGENT_EDIT_UPTAKE_MASK(agent_edit.agent) = strdup(value);
    } 

    value = get_text_widget_text(agent_edit.reservoir_text);
    
    if (strcmp(AGENT_EDIT_RESERVE_LIST(agent_edit.agent), value)){
	Free(AGENT_EDIT_RESERVE_LIST(agent_edit.agent));
	AGENT_EDIT_RESERVE_LIST(agent_edit.agent) = strdup(value);
    } 

    for (i = 0; i < NCHROMOSOMES; i++){
	char *tmp;
	value = get_text_widget_text(agent_edit.chromosome_text[i]);
	tmp = value;

	while (*tmp){
	    if (!IS_VALID_RESOURCE(*tmp)){
		popup_message("Character '%c' in the %s chromsome\nis outside the legal (a-%c) range!", 
			      *tmp, chromosome_names[i], 'a' + nresources - 1);
		return;
	    }
	    tmp++;
	}
	
	if (strcmp(AGENT_EDIT_CHROMOSOMES(agent_edit.agent)[i], value)){
	    Free(AGENT_EDIT_CHROMOSOMES(agent_edit.agent)[i]);
	    AGENT_EDIT_CHROMOSOMES(agent_edit.agent)[i] = strdup(value);
	}
    }
    
    value = get_text_widget_text(agent_edit.filename_text);

    if (!strlen(value)){
	popup_message("You specify a file for your agent\nbefore it can be saved.", *value);
	return;
    }

    write_agent_edit_to_file("agents", value, agent_edit.agent);
    /* XtPopdown(agent_edit.popup); */
    return;
}

static void 
save_site_edit(w, save, dummy)
Widget w;
XtPointer save;
XtPointer dummy;
{
    /*
     * Get all the values from the dialog and so on widgets that we used
     * to let the user edit a site. If they are different from the original
     * values, free the old ones and insert the new ones. Then write the
     * new edit structure out to a file whose name is determined by the contents
     * of part of the structure we are retrieving.
     */

    /* this function should call free_site_edit before it returns (if it does a popdown). */
    
    String value;
    Arg args[1];

    if ((BOOLEAN)save == FALSE){
	if (site_edit.site){
	    free_site_edit(site_edit.site);
	}
	XtPopdown(site_edit.agents_popup);
	XtPopdown(site_edit.popup);
	return;
    }

    value = get_text_widget_text(site_edit.name_text);
    
    if (!strlen(value)){
	popup_message("You must give your site a name before it can be saved.");
	return;
    }

    if (strcmp(SITE_EDIT_NAME(site_edit.site), value)){
	Free(SITE_EDIT_NAME(site_edit.site));
	SITE_EDIT_NAME(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.mutation_text);
    if (strcmp(SITE_EDIT_P_MUTATION(site_edit.site), value)){
	Free(SITE_EDIT_P_MUTATION(site_edit.site));
	SITE_EDIT_P_MUTATION(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.crossover_text);
    if (strcmp(SITE_EDIT_P_CROSSOVER(site_edit.site), value)){
	Free(SITE_EDIT_P_CROSSOVER(site_edit.site));
	SITE_EDIT_P_CROSSOVER(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.death_text);
    if (strcmp(SITE_EDIT_P_RANDOM_DEATH(site_edit.site), value)){
	Free(SITE_EDIT_P_RANDOM_DEATH(site_edit.site));
	SITE_EDIT_P_RANDOM_DEATH(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.production_text);
    if (strcmp(SITE_EDIT_PRODUCE_LIST(site_edit.site), value)){
	Free(SITE_EDIT_PRODUCE_LIST(site_edit.site));
	SITE_EDIT_PRODUCE_LIST(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.resources_text);
    if (strcmp(SITE_RESOURCE_LIST(site_edit.site), value)){
	Free(SITE_RESOURCE_LIST(site_edit.site));
	SITE_RESOURCE_LIST(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.maximums_text);
    if (strcmp(SITE_EDIT_MAXIMUM_LIST(site_edit.site), value)){
	Free(SITE_EDIT_MAXIMUM_LIST(site_edit.site));
	SITE_EDIT_MAXIMUM_LIST(site_edit.site) = strdup(value);
    }

    value = get_text_widget_text(site_edit.maintenance_text);
    if (strcmp(SITE_EDIT_MAINTENANCE_LIST(site_edit.site), value)){
	Free(SITE_EDIT_MAINTENANCE_LIST(site_edit.site));
	SITE_EDIT_MAINTENANCE_LIST(site_edit.site) = strdup(value);
    }

    XtSetArg(args[0], XtNstring, &value);
    XtGetValues(site_edit.agents_text, args, 1);
    SITE_EDIT_AGENTS(site_edit.site) = strdup(value);

    value = get_text_widget_text(site_edit.filename_text);

    if (!strlen(value)){
	popup_message("You specify a file for your site before it can be saved.", *value);
	return;
    }

    write_site_edit_to_file("sites", value, site_edit.site);
    
    /* XtPopdown(site_edit.agents_popup); */
    /* XtPopdown(site_edit.popup); */
    return;
}

static void 
save_world_edit(w, save, dummy)
Widget w;
XtPointer save;
XtPointer dummy;
{
    /*
     * Get all the values from the dialog and so on widgets that we used
     * to let the user edit a world. If they are different from the original
     * values, free the old ones and insert the new ones. Then write the
     * new edit structure out to a file whose name is determined by the contents
     * of part of the structure we are retrieving.
     */

    /* this function should call free_world_edit before it returns (if it does a popdown). */

    Arg args[1];
    String value;
    register NRESOURCES row;
    register NRESOURCES resources;
    char *combat_matrix;
    char *combat_matrix_row_as_str;

    if ((BOOLEAN)save == FALSE){
	if (world_edit.world){
	    free_world_edit(world_edit.world);
	}
	XtPopdown(world_edit.sites_popup);
	XtPopdown(world_edit.popup);
	return;
    }

    value = get_text_widget_text(world_edit.name_text);
    
    if (!strlen(value)){
	popup_message("You must give your world a name before it can be saved.");
	return;
    }

    if (strcmp(WORLD_EDIT_NAME(world_edit.world), value)){
	Free(WORLD_EDIT_NAME(world_edit.world));
	WORLD_EDIT_NAME(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.nresources_text);
    if (strcmp(WORLD_EDIT_NRESOURCES(world_edit.world), value)){
	Free(WORLD_EDIT_NRESOURCES(world_edit.world));
	WORLD_EDIT_NRESOURCES(world_edit.world) = strdup(value);
    }
    
    resources = (NRESOURCES) atoi(value);

    value = get_text_widget_text(world_edit.rows_text);
    if (strcmp(WORLD_EDIT_ROWS(world_edit.world), value)){
	Free(WORLD_EDIT_ROWS(world_edit.world));
	WORLD_EDIT_ROWS(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.cols_text);
    if (strcmp(WORLD_EDIT_COLS(world_edit.world), value)){
	Free(WORLD_EDIT_COLS(world_edit.world));
	WORLD_EDIT_COLS(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.trade_text);
    if (strcmp(WORLD_EDIT_TRADE_FRACTION(world_edit.world), value)){
	Free(WORLD_EDIT_TRADE_FRACTION(world_edit.world));
	WORLD_EDIT_TRADE_FRACTION(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.c_text);
    if (strcmp(WORLD_EDIT_C(world_edit.world), value)){
	Free(WORLD_EDIT_C(world_edit.world));
	WORLD_EDIT_C(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.selfrep_fraction_text);
    if (strcmp(WORLD_EDIT_SELFREP_FRACTION(world_edit.world), value)){
	Free(WORLD_EDIT_SELFREP_FRACTION(world_edit.world));
	WORLD_EDIT_SELFREP_FRACTION(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.selfrep_threshold_text);
    if (strcmp(WORLD_EDIT_SELFREP_THRESHOLD(world_edit.world), value)){
	Free(WORLD_EDIT_SELFREP_THRESHOLD(world_edit.world));
	WORLD_EDIT_SELFREP_THRESHOLD(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.p_maintenance_text);
    if (strcmp(WORLD_EDIT_P_MAINTENANCE(world_edit.world), value)){
	Free(WORLD_EDIT_P_MAINTENANCE(world_edit.world));
	WORLD_EDIT_P_MAINTENANCE(world_edit.world) = strdup(value);
    }

    value = get_text_widget_text(world_edit.neighborhood_text);
    if (strcmp(WORLD_EDIT_NEIGHBORHOOD(world_edit.world), value)){
	Free(WORLD_EDIT_NEIGHBORHOOD(world_edit.world));
	WORLD_EDIT_NEIGHBORHOOD(world_edit.world) = strdup(value);
    }

    if (WORLD_EDIT_COMBAT(world_edit.world)){
	/*
	 * This is a dirty hack (not to mention a waste of a small amount
	 * of space). We set this to 0 so that the combat matrix rows will
	 * be reallocated. We don't free the old rows since we don't know
	 * how many we had. I could try to get it from above, and notice
	 * if the number of resources had changed etc, but it's too
	 * involved. I can't be bothered since the plan is to throw this code
	 * away when I move to tcl. Bloody lazy horrible. This is just to
	 * fix a crashing problem. I hate this shit. Dec 6, 1993.
	 *
	 */
	
	WORLD_EDIT_COMBAT(world_edit.world) = 0;
    }

    /* 
     * Create the combat matrix pointers if need be. These will not
     * have been created if the user decided to edit a new world
     * since new_world_edit() didn't know how many to make.
     * We only find out now...
     */
     
    if (!WORLD_EDIT_COMBAT(world_edit.world)){
	WORLD_EDIT_COMBAT(world_edit.world) = (char **)Malloc(resources * sizeof(char *));

	for (row = 0; row < resources; row++){
	    WORLD_EDIT_COMBAT(world_edit.world)[row] = (char *)0;
	} 
    }
     
    /* Now get the combat matrix. */
    XtSetArg(args[0], XtNstring, &value);
    XtGetValues(world_edit.combat_text, args, 1);
    
    combat_matrix = strdup(value);
    combat_matrix_row_as_str = strtok(combat_matrix, "\n");
    row = 0;

    do {
	if (!combat_matrix_row_as_str) {
	    popup_message("Could not find row %d of your combat matrix.\nUse the Combat Matrix button to check your combat matrix.", 
			  row + 1);
	    return;
	}
	
	if (WORLD_EDIT_COMBAT(world_edit.world)[row] != (char *)0){
	    if (strcmp(WORLD_EDIT_COMBAT(world_edit.world)[row], combat_matrix_row_as_str)){
		Free(WORLD_EDIT_COMBAT(world_edit.world)[row]);
		WORLD_EDIT_COMBAT(world_edit.world)[row] = strdup(combat_matrix_row_as_str);
	    }
	}
	else {
	    WORLD_EDIT_COMBAT(world_edit.world)[row] = strdup(combat_matrix_row_as_str);
	}
	
	combat_matrix_row_as_str = strtok(NULL, "\n");
	row++;
    
    } while (row < resources);
    
    Free(combat_matrix);
    
    /* Get the sites array. */
    XtSetArg(args[0], XtNstring, &value);
    XtGetValues(world_edit.sites_text, args, 1);
    WORLD_EDIT_SITES(world_edit.world) = strdup(value);

    /* Now write the thing out. */
    
    value = get_text_widget_text(world_edit.filename_text);

    if (!strlen(value)){
	popup_message("You specify a file for your world before it can be saved.", *value);
	return;
    }

    write_world_edit_to_file("worlds", value, world_edit.world);
    /* XtPopdown(world_edit.sites_popup); */
    /* XtPopdown(world_edit.popup); */
    return;
}

static void
call_set_halt_generation(w, down, dummy2)
Widget w;
XtPointer down;
XtPointer dummy2;
{
    String value;
    
    XtPopdown((Widget) down);
    
    value = XawDialogGetValueString(running_menu_widgets.generation_dialog);
    
    set_halt_generation((GENERATION) atol(value));
    
    return;
}

static void
popup_stack_selector(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    if (!the_world){
	popup_message("Please select a world for running before you try this!");
	return;
    }
    
    if (WORLD_ROWS(the_world) == 1 && WORLD_COLS(the_world) == 1){
	popup_stack(0, 0);
    }
    else {
	set_dialog_value(edit_menu_widgets.stack_selector_dialog, "");
	popup_widget(0, edit_menu_widgets.stack_selector_popup, 0);
    }

    return;
}

static void
select_stack_for_editing(w, down, dummy2)
Widget w;
XtPointer down;
XtPointer dummy2;
{
    String value;
    String row_str;
    String col_str;
    int row;
    int col;
    
    XtPopdown((Widget) down);
    
    value = strdup(XawDialogGetValueString(edit_menu_widgets.stack_selector_dialog));

    row_str = strtok(value, " \t\n,:-");
    col_str = strtok(NULL, " \t\n,:-");

    if (!row_str || !col_str){
	popup_message("Invalid row/column specification. Please give two numbers.");
	Free(value);
	return;
    }

    row = atoi(row_str);
    col = atoi(col_str);

    if (row < 0 || row >= WORLD_ROWS(the_world)){
	popup_message("The row must be in the range 0 to %d.", WORLD_ROWS(the_world));
    }
    
    if (col < 0 || col >= WORLD_COLS(the_world)){
	popup_message("The row must be in the range 0 to %d.", WORLD_ROWS(the_world));
    }

    popup_stack(row, col);
    
    return;
}

static void
set_seed(w, down, dummy2)
Widget w;
XtPointer down;
XtPointer dummy2;
{
    String value;
    String tmp;
    
    tmp = value = XawDialogGetValueString(running_menu_widgets.seed_dialog);

    while (*tmp){
	if (!isdigit(*tmp)){
	    popup_message("The random seed can only contain digits!");
	    return;
	}
	tmp++;
    }
    
    XtPopdown((Widget) down);
    
    old_seed = seed = atol(value);
    
    return;
}

static void
set_verbosity(w, down, dummy2)
Widget w;
XtPointer down;
XtPointer dummy2;
{
    String value;
    
    XtPopdown((Widget) down);
    
    value = XawDialogGetValueString(control_menu_widgets.verbose_dialog);

    Free(verbose);
    verbose = strdup(value);
    
    return;
}


static void
popup_generation_selector(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    set_dialog_value(running_menu_widgets.generation_dialog, "");
    XtPopup(running_menu_widgets.generation_selector_popup, XtGrabNone);
    
    return;
}

static void
popup_seed_selector(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    set_dialog_value(running_menu_widgets.seed_dialog, "");
    XtPopup(running_menu_widgets.seed_popup, XtGrabNone);
    
    return;
}

static void
popup_verbose_selector(w, dummy1, dummy2)
Widget w;
XtPointer dummy1;
XtPointer dummy2;
{
    set_dialog_value(control_menu_widgets.verbose_dialog, verbose);
    XtPopup(control_menu_widgets.verbose_selector_popup, XtGrabNone);

    return;
}

void
plot_population_level(pop)
AGENT_POPULATION_SIZE pop;
{
    extend_plotter_line(population_graph, 0, (int) WORLD_GENERATION(the_world), (int) pop);
    return;
}

void
plot_variant_levels(alive, ever)
AGENT_POPULATION_SIZE alive;
AGENT_POPULATION_SIZE ever;
{
    extend_plotter_line(variant_graph, 0, (int) WORLD_GENERATION(the_world), (int) alive);
    extend_plotter_line(variant_graph, 1, (int) WORLD_GENERATION(the_world), (int) ever);

    return;
}

void
plot_schema_level()
{
    if (!schema_menu_widgets.schema){
	extend_plotter_line(schema_graph, 0, (int) WORLD_GENERATION(the_world), 0);
    }
    else {
	extend_plotter_line(schema_graph, 0, (int) WORLD_GENERATION(the_world), (int) world_count_schema(schema_menu_widgets.schema));
    }

    return;
}

void
plot_resource_levels(levels)
RESOURCE_LIST levels;
{
    NRESOURCES resource;
    
    for (resource = 0; resource < nresources; resource++){
	extend_plotter_line(resources_graph, resource, (int) WORLD_GENERATION(the_world), (int) levels[resource]);
    } 

    return;
}

void
plot_species_levels(levels)
AGENT_POPULATION_SIZE *levels;
{
    AGENT_POPULATION_SIZE species;

    for (species = 0; species < nspecies; species++){
	extend_plotter_line(species_graph, species, (int) WORLD_GENERATION(the_world), (int) levels[species]);
    }

    return;
}

int
create_species_graph_line(entry, dummy1, dummy2)
void *entry;
void *dummy1;
void *dummy2;
{
    STRING name = ((SPECIES_NAME_HASH_DATA *)entry)->name;
    
    add_line_to_plotter(species_graph, name, name, TRUE);
    
    return 1;
}

void
initialize_population_graph()
{
    clear_plotter(population_graph);
    return;
}

void
initialize_schema_graph()
{
    clear_plotter(schema_graph);
    return;
}

void
initialize_variant_graph()
{
    clear_plotter(variant_graph);
    return;
}

static void
set_new_schema(w, who_to_popdown, dummy2)
Widget w;
XtPointer who_to_popdown;
XtPointer dummy2;
{
    String value;

    value = XawDialogGetValueString(schema_menu_widgets.schema_dialog);
    
    if (!value || !strlen(value)){
	if (schema_menu_widgets.schema){
	    Free(schema_menu_widgets.schema);
	    schema_menu_widgets.schema = NULL;
	}
	
	if (who_to_popdown){
	    XtPopdown((Widget)who_to_popdown);
	}
	
	return;
    }

    if (legal_schema(value) == FALSE){
	popup_message("That schema is not legal!");
	return;
    }
    
    if (who_to_popdown){
	XtPopdown((Widget)who_to_popdown);
    }
    
    if (!schema_menu_widgets.schema){
	schema_menu_widgets.schema = strdup(value);
	name_plotter_line(schema_graph, 0, value);
    }
    else {
	if (strcmp(value, schema_menu_widgets.schema)){
	    Free(schema_menu_widgets.schema);
	    schema_menu_widgets.schema = strdup(value);
	    name_plotter_line(schema_graph, 0, value);
	}
    }

    clear_plotter(schema_graph);
    return;
}


void
initialize_species_graph(levels)
AGENT_POPULATION_SIZE *levels;
{
    static BOOLEAN lines_added = FALSE;
    
    if (lines_added == FALSE){
	hash_traverse(species_name_hash, create_species_graph_line, 0, 0);
	lines_added = TRUE;
    }

    clear_plotter(species_graph);
    return;
}

void
initialize_resource_graph()
{
    if (resources_graph->nlines < nresources){
	register int i;
	char widget_name[6];
	char legend_name[2];

	widget_name[1] = 'L';
	widget_name[2] = 'i';
	widget_name[3] = 'n';
	widget_name[4] = 'e';
	widget_name[5] = '\0';

	legend_name[1] = '\0';

	for (i = resources_graph->nlines; i < nresources; i++){
	    legend_name[0] = widget_name[0] = 'a' + i;
	    add_line_to_plotter(resources_graph, widget_name, legend_name, FALSE);
	} 
    }
    else if (resources_graph->nlines > nresources){
	while(resources_graph->nlines > nresources){
	    remove_line_from_plotter(resources_graph, nresources);
	}
    }
    
    clear_plotter(resources_graph);
    
    return;
}

static void
popup_stack(row, col)
int row;
int col;
{
    Arg args[1];

    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    edit_menu_widgets.stack_site = WORLD_SITE(the_world, row, col);
        
    edit_menu_widgets.stack_fp = Fopen(edit_menu_widgets.stack_file, "w");
    site_list_agents(edit_menu_widgets.stack_site, FALSE, edit_menu_widgets.stack_fp);
    /* printf("stack for generation %d\n", (int) WORLD_GENERATION(the_world)); */
    /* site_list_agents(edit_menu_widgets.stack_site, FALSE, stdout); */
    Fclose(edit_menu_widgets.stack_fp);
    
    XtSetArg(args[0], "string", edit_menu_widgets.stack_file);
    XtSetValues(edit_menu_widgets.stack_text, args, 1);

    XtPopup(edit_menu_widgets.stack_popup, XtGrabNone);
    
    return;
}

static void 
popdown_stack(w, who_to_popdown, dummy2)
Widget w;
XtPointer who_to_popdown;
XtPointer dummy2;
{
    XtPopdown((Widget)who_to_popdown);
    Unlink(edit_menu_widgets.stack_file);
    
    return;
}

static void
resume_from_stack(w, who_to_popdown, dummy2)
Widget w;
XtPointer who_to_popdown;
XtPointer dummy2;
{
    Arg args[1];
    Widget src;

    XtSetArg(args[0], "textSource", &src);
    XtGetValues(edit_menu_widgets.stack_text, args, 1);
    
    if (XawAsciiSave(src) == TRUE){
	edit_menu_widgets.stack_fp = Fopen(edit_menu_widgets.stack_file, "r");
	if (site_read_stack_from_file(edit_menu_widgets.stack_site, edit_menu_widgets.stack_fp, edit_menu_widgets.stack_file) == TRUE){
	    XtPopdown((Widget)who_to_popdown);
	}
	Fclose(edit_menu_widgets.stack_fp);
	Unlink(edit_menu_widgets.stack_file);
    }
    else {
	popup_message("Could not save the stack! Sorry.");
    }
    
    return;
}

void
process_graphics_events()
{
    /*
     * Process X events if there are any there. This function
     * will not block if there are no events to be processed.
     */
    XEvent event;

    while (XtAppPending(context)){
	XtAppNextEvent(context, &event);
	XtDispatchEvent(&event);
    }
    
    return;
}

void
await_graphic_event()
{
    /*
     * Wait for and process an X event.  This function is called by
     * the main running loop when it knows the next thing that must
     * happen is a graphics event. For example, when there is no more
     * work to be done and a graphic event is needed to create some.
     */
    XEvent event;

    XtAppNextEvent(context, &event);
    XtDispatchEvent(&event);

    return;
}

static void
rerun_world()
{
    if (!the_world){
	popup_message("Please select a world for running before you try this.");
	return;
    }
    
    seed = old_seed;

    instantiate_world(NULL, NULL, genome_hash_size);
    return;
}

void
update_all_plotters()
{
    update_plotter(species_graph);
    update_plotter(resources_graph);
    update_plotter(population_graph);
    update_plotter(variant_graph);
    update_plotter(schema_graph);

    return;
}

#endif /* NO_GRAPHICS */
