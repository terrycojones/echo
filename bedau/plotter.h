#ifndef NO_GRAPHICS

typedef struct {
    BOOLEAN delete_if_y_zero;
    BOOLEAN printing;
    Widget line;
    AtIntPoint *data;
} PLOTTER_LINE;

typedef struct {
    Widget popup;
    Widget form;
    Widget plotter;
    Widget xaxis;
    Widget yaxis;
    int max_points;
    int npoints;
    int history_length;
    double xmin;
    double original_xmin;
    double xmax;
    double original_xmax;
    double ymin;
    double original_ymin;
    double ymax;
    double original_ymax;
    PLOTTER_LINE *lines;
    int nlines;
    int max_lines;
} PLOTTER;

extern PLOTTER *create_plotter();
extern void clear_plotter();
extern void clear_plotter_line();
extern void add_line_to_plotter();
extern void remove_line_from_plotter();
extern void extend_plotter_line();
extern void name_plotter_line();
extern void remove_all_lines_from_plotter();
extern void update_plotter();

#endif /* NO_GRAPHICS */
