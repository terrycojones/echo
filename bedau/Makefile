#
# Makefile for Echo.
#
X_INCLUDES = /network/software/packages/X/include
X_LIBS = /network/software/packages/X/lib
VERSION  = 1.0
RELEASE  = Echo-$(VERSION)
CC       = cc
DEFINES  = -DNO_CANVAS -DCOMPRESS_OUTPUT
INCLUDES = -I. -I$(X_INCLUDES) -IWIDGETS/fwf/include -IWIDGETS/At/include -IWIDGETS/Athena3d/include
GCCFLAGS = -Wall
CFLAGS   = -g $(INCLUDES) $(DEFINES) # $(GCCFLAGS)
LDFLAGS  = -L$(X_LIBS) -LWIDGETS/Athena3d/lib -LWIDGETS/fwf/lib -LWIDGETS/At/lib

SRC      = agents.c agents.h chromosomes.c chromosomes.h defaults.h defs.h error.c \
           free.c free.h globals.h graphics.c graphics.h hash.c hash.h interaction.c \
           interaction.h main.c main.h main_options.c misc_graphics.c misc_graphics.h \
           options.c options.h plotter.c plotter.h random.c random.h regexp.c regexp.h \
           regsub.c regmagic.h resources.c resources.h running.c running.h schema.c \
           schema.h sites.c sites.h syscalls.c syscalls.h types.h worlds.c worlds.h

OBJS     = agents.o chromosomes.o error.o free.o graphics.o hash.o interaction.o \
           main.o misc_graphics.o options.o plotter.o random.o regexp.o regsub.o \
           resources.o running.o schema.o sites.o syscalls.o worlds.o

Echo : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) -lfwf -lDir -lAt -lXaw3d -lXmu -lXt -lXext -lX11 -lm

depend:
	makedepend $(INCLUDES) $(DEFINES) $(SRC)

tags: co
	etags *.[ch]

clean :
	rm -f *.o

clobber : clean
	rm -f Echo Makefile.bak

wc : co
	wc *.[ch] Makefile

co:
	@for i in $(SRC); do \
		if [ ! -f $$i ]; then co $$i; fi \
	done

col:
	@for i in $(SRC); do \
		if [ ! -f $$i ]; then co -l $$i; fi \
	done

tar:
	rm -fr $(RELEASE) OBJECTS/core
	mkdir $(RELEASE)
	for i in $(SRC); do \
		if [ -f $$i ]; then cp $$i $(RELEASE)/$$i; else co -p $$i > $(RELEASE)/$$i; fi \
	done
	for i in 0 1 2 3 4 5; do \
		cp 765011475.options.gene$$i $(RELEASE); \
	done
	cp $$HOME/app-defaults/Echo $(RELEASE)/Echo.ad
	cp Makefile.release $(RELEASE)/Makefile
	cp 765011475.options $(RELEASE)
	cp .window_manager echo-sh-setup echo-x-setup echo-sh-common build-run-echo $(RELEASE)
	cp print run run-ng $(RELEASE)
	cp -r tools $(RELEASE)
	cp -r OBJECTS $(RELEASE)
	find $(RELEASE)/OBJECTS -type f -exec chmod 644 {} \;
	cp -r cluster2.3 $(RELEASE)
	cp -r WIDGETS $(RELEASE)
	mkdir $(RELEASE)/DOCUMENTATION
	cp ../tex/how-to/how-to.tex ../tex/how-to/how-to.ps $(RELEASE)/DOCUMENTATION
	tar cvf $(RELEASE).tar $(RELEASE)
	gzip -f $(RELEASE).tar
	rm -fr $(RELEASE)

# DO NOT DELETE THIS LINE -- make depend depends on it.

agents.o: /usr/include/stdio.h /usr/include/stdlib.h
agents.o: /usr/include/sys/stdtypes.h /usr/include/string.h
agents.o: /usr/include/strings.h /usr/include/sys/param.h
agents.o: /usr/include/machine/param.h /usr/include/sys/signal.h
agents.o: /usr/include/vm/faultcode.h types.h /usr/include/limits.h defs.h
agents.o: hash.h globals.h syscalls.h agents.h sites.h worlds.h graphics.h
agents.o: chromosomes.h resources.h random.h free.h
chromosomes.o: /usr/include/stdio.h /usr/include/stdlib.h
chromosomes.o: /usr/include/sys/stdtypes.h /usr/include/string.h types.h
chromosomes.o: /usr/include/limits.h defs.h hash.h globals.h chromosomes.h
chromosomes.o: syscalls.h random.h
error.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h hash.h
error.o: globals.h
free.o: /usr/include/stdio.h /usr/include/stdlib.h
free.o: /usr/include/sys/stdtypes.h types.h /usr/include/limits.h defs.h
free.o: hash.h globals.h free.h syscalls.h
graphics.o: /usr/include/stdio.h /usr/include/stdlib.h
graphics.o: /usr/include/sys/stdtypes.h /usr/include/string.h
graphics.o: /usr/include/ctype.h types.h /usr/include/limits.h defs.h hash.h
graphics.o: agents.h sites.h worlds.h schema.h running.h resources.h
graphics.o: globals.h graphics.h syscalls.h free.h
graphics.o: /network/software/packages/X/include/X11/Intrinsic.h
graphics.o: /network/software/packages/X/include/X11/X11/Xlib.h
graphics.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
graphics.o: /network/software/packages/X/include/X11/X.h
graphics.o: /network/software/packages/X/include/X11/Xfuncproto.h
graphics.o: /network/software/packages/X/include/X11/Xosdefs.h
graphics.o: /usr/include/stddef.h
graphics.o: /network/software/packages/X/include/X11/X11/Xutil.h
graphics.o: /network/software/packages/X/include/X11/X11/Xresource.h
graphics.o: /network/software/packages/X/include/X11/X11/Core.h
graphics.o: /network/software/packages/X/include/X11/X11/Composite.h
graphics.o: /network/software/packages/X/include/X11/X11/Constraint.h
graphics.o: /network/software/packages/X/include/X11/X11/Object.h
graphics.o: /network/software/packages/X/include/X11/X11/RectObj.h
graphics.o: /network/software/packages/X/include/X11/StringDefs.h
graphics.o: /network/software/packages/X/include/X11/Shell.h
graphics.o: WIDGETS/Athena3d/include/Box.h
graphics.o: /network/software/packages/X/include/X11/Xmu/Converters.h
graphics.o: WIDGETS/Athena3d/include/Label.h
graphics.o: WIDGETS/Athena3d/include/Simple.h
graphics.o: WIDGETS/Athena3d/include/Dialog.h WIDGETS/Athena3d/include/Form.h
graphics.o: WIDGETS/Athena3d/include/Form.h
graphics.o: WIDGETS/Athena3d/include/Viewport.h
graphics.o: WIDGETS/Athena3d/include/Reports.h
graphics.o: WIDGETS/Athena3d/include/Command.h
graphics.o: WIDGETS/Athena3d/include/Label.h
graphics.o: WIDGETS/Athena3d/include/MenuButton.h
graphics.o: WIDGETS/Athena3d/include/Command.h
graphics.o: WIDGETS/Athena3d/include/SimpleMenu.h
graphics.o: WIDGETS/Athena3d/include/Sme.h WIDGETS/Athena3d/include/SmeBSB.h
graphics.o: WIDGETS/Athena3d/include/SmeThreeD.h
graphics.o: WIDGETS/Athena3d/include/Sme.h WIDGETS/Athena3d/include/SmeLine.h
graphics.o: WIDGETS/Athena3d/include/Cardinals.h
graphics.o: WIDGETS/Athena3d/include/AsciiText.h
graphics.o: WIDGETS/Athena3d/include/Text.h
graphics.o: /network/software/packages/X/include/X11/Xaw/TextSink.h
graphics.o: /network/software/packages/X/include/X11/Xaw/TextSrc.h
graphics.o: /network/software/packages/X/include/X11/Xaw/AsciiSrc.h
graphics.o: /network/software/packages/X/include/X11/Xaw/AsciiSink.h
graphics.o: WIDGETS/Athena3d/include/AsciiSrc.h misc_graphics.h
graphics.o: WIDGETS/fwf/include/Canvas.h WIDGETS/fwf/include/FileSel.h
graphics.o: WIDGETS/fwf/include/ScrList.h WIDGETS/fwf/include/DirMgr.h
graphics.o: WIDGETS/fwf/include/Directory.h /usr/include/sys/param.h
graphics.o: /usr/include/machine/param.h /usr/include/sys/signal.h
graphics.o: /usr/include/vm/faultcode.h /usr/include/sys/stat.h
graphics.o: /usr/include/dirent.h /usr/include/sys/dirent.h
graphics.o: WIDGETS/At/include/Plotter.h WIDGETS/At/include/At.h
graphics.o: /usr/include/math.h /usr/include/floatingpoint.h
graphics.o: /usr/include/sys/ieeefp.h /usr/include/assert.h
graphics.o: /usr/include/strings.h
graphics.o: /network/software/packages/X/include/X11/IntrinsicP.h
graphics.o: /network/software/packages/X/include/X11/X11/CoreP.h
graphics.o: /network/software/packages/X/include/X11/X11/CompositeP.h
graphics.o: /network/software/packages/X/include/X11/X11/ConstrainP.h
graphics.o: /network/software/packages/X/include/X11/X11/ObjectP.h
graphics.o: /network/software/packages/X/include/X11/X11/RectObjP.h
graphics.o: WIDGETS/At/include/Plot.h WIDGETS/At/include/Scale.h
graphics.o: WIDGETS/At/include/FontFamily.h WIDGETS/At/include/XYAxis.h
graphics.o: WIDGETS/At/include/AxisCore.h WIDGETS/At/include/Axis.h
graphics.o: WIDGETS/At/include/XYLinePlot.h WIDGETS/At/include/XYPlot.h
hash.o: /usr/include/stdio.h /usr/include/string.h
hash.o: /usr/include/sys/stdtypes.h /usr/include/strings.h
hash.o: /usr/include/stdlib.h /usr/include/sys/types.h
hash.o: /usr/include/sys/sysmacros.h types.h /usr/include/limits.h defs.h
hash.o: hash.h syscalls.h
interaction.o: /usr/include/stdio.h /usr/include/memory.h
interaction.o: /usr/include/string.h /usr/include/sys/stdtypes.h types.h
interaction.o: /usr/include/limits.h defs.h hash.h globals.h agents.h sites.h
interaction.o: resources.h interaction.h random.h
main.o: /usr/include/stdio.h /usr/include/string.h
main.o: /usr/include/sys/stdtypes.h /usr/include/strings.h
main.o: /usr/include/memory.h /usr/include/signal.h /usr/include/sys/signal.h
main.o: /usr/include/vm/faultcode.h /usr/include/sys/file.h
main.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
main.o: /usr/include/sys/fcntlcom.h /usr/include/sys/stat.h
main.o: /usr/include/sys/param.h /usr/include/machine/param.h types.h
main.o: /usr/include/limits.h defs.h hash.h defaults.h globals.h main.h
main.o: worlds.h running.h graphics.h random.h syscalls.h options.h
main.o: main_options.c
misc_graphics.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h
misc_graphics.o: hash.h agents.h sites.h worlds.h schema.h running.h
misc_graphics.o: resources.h chromosomes.h globals.h graphics.h syscalls.h
misc_graphics.o: free.h /network/software/packages/X/include/X11/Intrinsic.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Xlib.h
misc_graphics.o: /usr/include/sys/types.h /usr/include/sys/stdtypes.h
misc_graphics.o: /usr/include/sys/sysmacros.h
misc_graphics.o: /network/software/packages/X/include/X11/X.h
misc_graphics.o: /network/software/packages/X/include/X11/Xfuncproto.h
misc_graphics.o: /network/software/packages/X/include/X11/Xosdefs.h
misc_graphics.o: /usr/include/stddef.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Xutil.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Xresource.h
misc_graphics.o: /usr/include/string.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Core.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Composite.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Constraint.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/Object.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/RectObj.h
misc_graphics.o: /network/software/packages/X/include/X11/StringDefs.h
misc_graphics.o: /network/software/packages/X/include/X11/Shell.h
misc_graphics.o: WIDGETS/Athena3d/include/Label.h
misc_graphics.o: WIDGETS/Athena3d/include/Simple.h
misc_graphics.o: /network/software/packages/X/include/X11/Xmu/Converters.h
misc_graphics.o: WIDGETS/Athena3d/include/Dialog.h
misc_graphics.o: WIDGETS/Athena3d/include/Form.h
misc_graphics.o: WIDGETS/Athena3d/include/Command.h
misc_graphics.o: WIDGETS/Athena3d/include/Label.h
misc_graphics.o: WIDGETS/Athena3d/include/AsciiText.h
misc_graphics.o: WIDGETS/Athena3d/include/Text.h
misc_graphics.o: /network/software/packages/X/include/X11/Xaw/TextSink.h
misc_graphics.o: /network/software/packages/X/include/X11/Xaw/TextSrc.h
misc_graphics.o: /network/software/packages/X/include/X11/Xaw/AsciiSrc.h
misc_graphics.o: /network/software/packages/X/include/X11/Xaw/AsciiSink.h
misc_graphics.o: WIDGETS/Athena3d/include/AsciiSrc.h
misc_graphics.o: WIDGETS/fwf/include/FileSel.h WIDGETS/fwf/include/ScrList.h
misc_graphics.o: WIDGETS/fwf/include/DirMgr.h WIDGETS/fwf/include/Directory.h
misc_graphics.o: /usr/include/sys/param.h /usr/include/machine/param.h
misc_graphics.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h
misc_graphics.o: /usr/include/sys/stat.h /usr/include/dirent.h
misc_graphics.o: /usr/include/sys/dirent.h WIDGETS/At/include/AxisCore.h
misc_graphics.o: WIDGETS/At/include/At.h /usr/include/math.h
misc_graphics.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
misc_graphics.o: /usr/include/assert.h /usr/include/strings.h
misc_graphics.o: /network/software/packages/X/include/X11/IntrinsicP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/CoreP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/CompositeP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/ConstrainP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/ObjectP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/RectObjP.h
misc_graphics.o: WIDGETS/At/include/Plot.h WIDGETS/At/include/Scale.h
misc_graphics.o: WIDGETS/At/include/FontFamily.h misc_graphics.h
options.o: /usr/include/stdio.h /usr/include/stdlib.h
options.o: /usr/include/sys/stdtypes.h /usr/include/string.h
options.o: /usr/include/strings.h /usr/include/ctype.h types.h
options.o: /usr/include/limits.h defs.h hash.h globals.h options.h syscalls.h
random.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h hash.h
random.o: globals.h /usr/include/sys/time.h /usr/include/sys/time.h
regexp.o: /usr/include/stdio.h /usr/include/string.h
regexp.o: /usr/include/sys/stdtypes.h types.h /usr/include/limits.h defs.h
regexp.o: hash.h globals.h graphics.h syscalls.h regexp.h regmagic.h
regsub.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h hash.h
regsub.o: regexp.h graphics.h regmagic.h globals.h
resources.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h hash.h
resources.o: globals.h
running.o: types.h /usr/include/limits.h defs.h hash.h sites.h worlds.h
running.o: graphics.h random.h syscalls.h /usr/include/stdio.h
running.o: /usr/include/string.h /usr/include/sys/stdtypes.h globals.h
schema.o: types.h /usr/include/limits.h defs.h hash.h
sites.o: /usr/include/stdio.h /usr/include/stdlib.h
sites.o: /usr/include/sys/stdtypes.h /usr/include/string.h
sites.o: /usr/include/strings.h /usr/include/ctype.h /usr/include/sys/param.h
sites.o: /usr/include/machine/param.h /usr/include/sys/signal.h
sites.o: /usr/include/vm/faultcode.h /usr/include/sys/stat.h
sites.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h types.h
sites.o: /usr/include/limits.h defs.h hash.h sites.h agents.h worlds.h
sites.o: chromosomes.h graphics.h resources.h interaction.h free.h syscalls.h
sites.o: random.h globals.h defaults.h regexp.h
syscalls.o: /usr/include/stdio.h /usr/include/stdlib.h
syscalls.o: /usr/include/sys/stdtypes.h /usr/include/malloc.h
syscalls.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
syscalls.o: /usr/include/sys/stat.h
syscalls.o: /usr/include/stdio.h
types.o: /usr/include/limits.h defs.h hash.h
worlds.o: /usr/include/stdio.h /usr/include/stdlib.h
worlds.o: /usr/include/sys/stdtypes.h /usr/include/string.h
worlds.o: /usr/include/strings.h /usr/include/sys/param.h
worlds.o: /usr/include/machine/param.h /usr/include/sys/signal.h
worlds.o: /usr/include/vm/faultcode.h /usr/include/sys/stat.h
worlds.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h types.h
worlds.o: /usr/include/limits.h defs.h hash.h globals.h syscalls.h worlds.h
worlds.o: sites.h chromosomes.h resources.h running.h free.h defaults.h
worlds.o: graphics.h
