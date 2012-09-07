
#ifdef JHH_SILLINESS
    jhh_bitmap = XCreateBitmapFromData(XtDisplay(real_top), RootWindowOfScreen(XtScreen(real_top)), 
				       smaller_head_bits, smaller_head_width, smaller_head_height);
    
    if (jhh_bitmap != None){
	i = 0;
	XtSetArg(args[i], XtNlabel, "Do it My Way...");                         i++;
	XtSetArg(args[i], XtNleftMargin, smaller_head_width + 25);              i++;
	XtSetArg(args[i], XtNjustify, XtJustifyCenter);                         i++;
	XtSetArg(args[i], XtNleftBitmap, jhh_bitmap);                           i++;
	/* XtSetArg(args[i], XtNvertSpace, 500);                                   i++; */
	control_menu_widgets.entry = XtCreateManagedWidget("myWay", smeBSBObjectClass, control_menu_widgets.menu, args, i);
	XtAddCallback(control_menu_widgets.entry, XtNcallback, toggle_jhh_output, NULL);
	
	/* Separate John from the exit button with a line. */
	control_menu_widgets.entry = XtCreateManagedWidget("line", smeLineObjectClass, control_menu_widgets.menu, NULL, ZERO);
    }
    else {
	fprintf(stderr, "Could not create the head bitmap.");
#endif
	
	i = 0;
	XtSetArg(args[i], XtNlabel, "John Holland Output");                         i++;
	control_menu_widgets.entry = XtCreateManagedWidget("myWay", smeBSBObjectClass, control_menu_widgets.menu, args, i);
	XtAddCallback(control_menu_widgets.entry, XtNcallback, toggle_jhh_output, NULL);
	
#ifdef JHH_SILLINESS
    }
#endif
    
