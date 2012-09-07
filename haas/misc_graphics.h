extern String remove_underscores();
extern Widget create_label_and_text();
extern String get_text_widget_text();
extern void set_text_widget_text();
extern Widget create_command_button();
extern void toggle_verbose();
extern void quit();
extern void popup_widget();
extern void popdown_widget();
extern void clear_text_widget();
extern void clear_dialog_widget();
extern void call_instantiate_world();
extern void call_pause_world();
extern void call_continue_world();
extern void call_run_for_x_generations();
extern void call_run_world_indefinitely();
extern void left_justify_dialog_label();
extern void set_dialog_value();
extern void set_x_axis();
extern void set_y_axis();
extern void cluster();

#ifndef NO_TCL
extern void call_pop_viewer();
extern void call_mut_viewer();
extern void call_res_viewer();
#endif
