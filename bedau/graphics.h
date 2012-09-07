extern void initialize_graphics();
#ifndef NO_CANVAS
extern void create_canvas_widgets();
extern void destroy_canvas_widgets();
#endif
extern void update_all_plotters();
extern void plot_population_level();
extern void plot_resource_levels();
extern void plot_species_levels();
extern void plot_schema_level();
extern void plot_variant_levels();
extern void popup_message();
extern void process_graphics_events();
extern void await_graphic_event();
extern void initialize_resource_graph();
extern void initialize_population_graph();
extern void initialize_species_graph();
extern void initialize_schema_graph();
extern void initialize_variant_graph();


