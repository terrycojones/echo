##
## Makefile for Echo-1.0
##
## You will need to change the definitions below for
## X_INCLUDES and X_LIBS. Nothing else should have to 
## change.
##
## The first of these is the directory in which your
## system has the X window system include files.
##
## The second is where you have the X libraries stored.
##

X_INCLUDES = /network/software/packages/X/include
X_LIBS = /network/software/packages/X/lib


##############################################################################
#
# Nothing below should need to be touched.
#
# If you have gcc, you might want to change the next line to use it,
# in which case you can put "$(GCCFLAGS)" into the CFLAGS line.
#
CC       = cc
DEFINES  = -DNO_COMBAT -DNO_TRADE -DNO_SEX -DNO_ZAP -DNO_CANVAS
INCLUDES = -I. -I$(X_INCLUDES) -IWIDGETS/fwf/include -IWIDGETS/At/include \
           -IWIDGETS/Athena3d/include
GCCFLAGS = -Wall
CFLAGS   = -g $(INCLUDES) $(DEFINES)
LDFLAGS  = -L$(X_LIBS) -LWIDGETS/Athena3d/lib -LWIDGETS/fwf/lib \
           -LWIDGETS/At/lib -Bstatic
LIBS     = WIDGETS/Athena3d/lib/libXaw3d.a \
           WIDGETS/Athena3d/lib/libXaw3d.sa.5.0 \
           WIDGETS/At/lib/libAt.a WIDGETS/fwf/lib/libDir.a \
           WIDGETS/fwf/lib/libfwf.a

OBJS     = agents.o chromosomes.o error.o free.o graphics.o hash.o \
           interaction.o main.o misc_graphics.o options.o plotter.o \
           random.o regexp.o regsub.o resources.o running.o schema.o \
           sites.o syscalls.o worlds.o

SRC      = agents.c chromosomes.c error.c free.c graphics.c hash.c \
           interaction.c main.c misc_graphics.c options.c plotter.c \
           random.c regexp.c regsub.c resources.c running.c schema.c \
           sites.c syscalls.c worlds.c

Echo-haas : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) \
        -lfwf -lDir -lAt -lXaw3d -lXmu -lXt -lXext -lX11 -lm

all : libs Echo-haas cluster run-echo-setup

simple-setup : x-setup sh-setup

x-setup:
	@echo "Setting up X resources for Echo... "
	@./echo-x-setup
	@echo Done.

sh-setup:
	@echo "Setting up environment variables for Echo... "
	@./echo-sh-setup $$SHELL
	@echo Done.

run-echo-setup:
	@echo "Making run-echo shell script... "
	@./build-run-echo
	@echo Done.

cluster :
	cd cluster-2.2; make
	cp cluster-2.2/cluster .

libs :
	for i in $(LIBS); do \
		ranlib $$i ; \
	done

depend:
	makedepend $(INCLUDES) $(DEFINES) $(SRC)

clean :
	rm -f *.o

clobber : clean
	rm -f Echo


# DO NOT DELETE THIS LINE -- make depend depends on it.

agents.o: /usr/include/stdio.h /usr/include/stdlib.h
agents.o: /usr/include/sys/stdtypes.h /usr/include/string.h
agents.o: /usr/include/strings.h /usr/include/math.h
agents.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
agents.o: /usr/include/sys/param.h /usr/include/machine/param.h
agents.o: /usr/include/machine/devaddr.h /usr/include/sys/signal.h
agents.o: /usr/include/vm/faultcode.h types.h /usr/include/limits.h defs.h
agents.o: hash.h defaults.h globals.h syscalls.h agents.h sites.h worlds.h
agents.o: graphics.h chromosomes.h resources.h random.h free.h
chromosomes.o: /usr/include/stdio.h /usr/include/stdlib.h
chromosomes.o: /usr/include/sys/stdtypes.h /usr/include/string.h
chromosomes.o: /usr/include/memory.h types.h /usr/include/limits.h defs.h
chromosomes.o: hash.h globals.h chromosomes.h syscalls.h random.h
error.o: /usr/include/stdio.h types.h /usr/include/limits.h defs.h hash.h
error.o: globals.h
free.o: /usr/include/stdio.h /usr/include/stdlib.h
free.o: /usr/include/sys/stdtypes.h types.h /usr/include/limits.h defs.h
free.o: hash.h globals.h free.h syscalls.h
graphics.o: /usr/include/stdio.h /usr/include/stdlib.h
graphics.o: /usr/include/sys/stdtypes.h /usr/include/string.h
graphics.o: /usr/include/ctype.h
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
graphics.o: WIDGETS/Athena3d/include/AsciiSrc.h WIDGETS/fwf/include/FileSel.h
graphics.o: WIDGETS/fwf/include/ScrList.h WIDGETS/fwf/include/DirMgr.h
graphics.o: WIDGETS/fwf/include/Directory.h /usr/include/sys/param.h
graphics.o: /usr/include/machine/param.h /usr/include/machine/devaddr.h
graphics.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h
graphics.o: /usr/include/sys/stat.h /usr/include/dirent.h
graphics.o: /usr/include/sys/dirent.h WIDGETS/At/include/Plotter.h
graphics.o: WIDGETS/At/include/At.h /usr/include/math.h
graphics.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
graphics.o: /usr/include/assert.h /usr/include/strings.h
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
graphics.o: types.h /usr/include/limits.h defs.h hash.h plotter.h agents.h
graphics.o: sites.h worlds.h schema.h running.h resources.h globals.h
graphics.o: graphics.h syscalls.h free.h misc_graphics.h random.h
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
main.o: /usr/include/sys/param.h /usr/include/machine/param.h
main.o: /usr/include/machine/devaddr.h types.h /usr/include/limits.h defs.h
main.o: hash.h defaults.h globals.h main.h worlds.h running.h graphics.h
main.o: random.h syscalls.h options.h main_options.c
misc_graphics.o: /usr/include/stdio.h
misc_graphics.o: /network/software/packages/X/include/X11/Intrinsic.h
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
misc_graphics.o: /usr/include/machine/devaddr.h /usr/include/sys/signal.h
misc_graphics.o: /usr/include/vm/faultcode.h /usr/include/sys/stat.h
misc_graphics.o: /usr/include/dirent.h /usr/include/sys/dirent.h
misc_graphics.o: WIDGETS/At/include/AxisCore.h WIDGETS/At/include/At.h
misc_graphics.o: /usr/include/math.h /usr/include/floatingpoint.h
misc_graphics.o: /usr/include/sys/ieeefp.h /usr/include/assert.h
misc_graphics.o: /usr/include/strings.h
misc_graphics.o: /network/software/packages/X/include/X11/IntrinsicP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/CoreP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/CompositeP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/ConstrainP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/ObjectP.h
misc_graphics.o: /network/software/packages/X/include/X11/X11/RectObjP.h
misc_graphics.o: WIDGETS/At/include/Plot.h WIDGETS/At/include/Scale.h
misc_graphics.o: WIDGETS/At/include/FontFamily.h misc_graphics.h types.h
misc_graphics.o: /usr/include/limits.h defs.h hash.h agents.h sites.h
misc_graphics.o: worlds.h schema.h running.h resources.h chromosomes.h
misc_graphics.o: globals.h graphics.h syscalls.h free.h
options.o: /usr/include/stdio.h /usr/include/stdlib.h
options.o: /usr/include/sys/stdtypes.h /usr/include/string.h
options.o: /usr/include/strings.h /usr/include/ctype.h types.h
options.o: /usr/include/limits.h defs.h hash.h globals.h options.h syscalls.h
plotter.o: /network/software/packages/X/include/X11/Intrinsic.h
plotter.o: /network/software/packages/X/include/X11/X11/Xlib.h
plotter.o: /usr/include/sys/types.h /usr/include/sys/stdtypes.h
plotter.o: /usr/include/sys/sysmacros.h
plotter.o: /network/software/packages/X/include/X11/X.h
plotter.o: /network/software/packages/X/include/X11/Xfuncproto.h
plotter.o: /network/software/packages/X/include/X11/Xosdefs.h
plotter.o: /usr/include/stddef.h
plotter.o: /network/software/packages/X/include/X11/X11/Xutil.h
plotter.o: /network/software/packages/X/include/X11/X11/Xresource.h
plotter.o: /usr/include/string.h
plotter.o: /network/software/packages/X/include/X11/X11/Core.h
plotter.o: /network/software/packages/X/include/X11/X11/Composite.h
plotter.o: /network/software/packages/X/include/X11/X11/Constraint.h
plotter.o: /network/software/packages/X/include/X11/X11/Object.h
plotter.o: /network/software/packages/X/include/X11/X11/RectObj.h
plotter.o: /network/software/packages/X/include/X11/StringDefs.h
plotter.o: /network/software/packages/X/include/X11/Shell.h
plotter.o: WIDGETS/Athena3d/include/Form.h WIDGETS/At/include/Plotter.h
plotter.o: WIDGETS/At/include/At.h /usr/include/stdio.h /usr/include/math.h
plotter.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h
plotter.o: /usr/include/assert.h /usr/include/strings.h
plotter.o: /network/software/packages/X/include/X11/IntrinsicP.h
plotter.o: /network/software/packages/X/include/X11/X11/CoreP.h
plotter.o: /network/software/packages/X/include/X11/X11/CompositeP.h
plotter.o: /network/software/packages/X/include/X11/X11/ConstrainP.h
plotter.o: /network/software/packages/X/include/X11/X11/ObjectP.h
plotter.o: /network/software/packages/X/include/X11/X11/RectObjP.h
plotter.o: WIDGETS/At/include/Plot.h WIDGETS/At/include/Scale.h
plotter.o: WIDGETS/At/include/FontFamily.h WIDGETS/At/include/XYAxis.h
plotter.o: WIDGETS/At/include/AxisCore.h WIDGETS/Athena3d/include/Text.h
plotter.o: /network/software/packages/X/include/X11/Xaw/TextSink.h
plotter.o: /network/software/packages/X/include/X11/Xaw/TextSrc.h
plotter.o: /network/software/packages/X/include/X11/Xaw/AsciiSrc.h
plotter.o: /network/software/packages/X/include/X11/Xaw/AsciiSink.h
plotter.o: WIDGETS/At/include/Axis.h WIDGETS/At/include/XYLinePlot.h
plotter.o: WIDGETS/At/include/XYPlot.h types.h /usr/include/limits.h defs.h
plotter.o: hash.h globals.h plotter.h syscalls.h misc_graphics.h
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
running.o: graphics.h syscalls.h /usr/include/stdio.h /usr/include/string.h
running.o: /usr/include/sys/stdtypes.h globals.h
schema.o: types.h /usr/include/limits.h defs.h hash.h
sites.o: /usr/include/stdio.h /usr/include/stdlib.h
sites.o: /usr/include/sys/stdtypes.h /usr/include/string.h
sites.o: /usr/include/strings.h /usr/include/ctype.h /usr/include/sys/param.h
sites.o: /usr/include/machine/param.h /usr/include/machine/devaddr.h
sites.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h
sites.o: /usr/include/sys/stat.h /usr/include/sys/types.h
sites.o: /usr/include/sys/sysmacros.h types.h /usr/include/limits.h defs.h
sites.o: hash.h sites.h agents.h worlds.h chromosomes.h graphics.h
sites.o: resources.h interaction.h free.h syscalls.h random.h globals.h
sites.o: defaults.h regexp.h
syscalls.o: /usr/include/stdio.h /usr/include/stdlib.h
syscalls.o: /usr/include/sys/stdtypes.h /usr/include/malloc.h
syscalls.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
syscalls.o: /usr/include/sys/stat.h
worlds.o: /usr/include/stdio.h /usr/include/stdlib.h
worlds.o: /usr/include/sys/stdtypes.h /usr/include/string.h
worlds.o: /usr/include/strings.h /usr/include/sys/param.h
worlds.o: /usr/include/machine/param.h /usr/include/machine/devaddr.h
worlds.o: /usr/include/sys/signal.h /usr/include/vm/faultcode.h
worlds.o: /usr/include/sys/stat.h /usr/include/sys/types.h
worlds.o: /usr/include/sys/sysmacros.h types.h /usr/include/limits.h defs.h
worlds.o: hash.h globals.h syscalls.h worlds.h sites.h chromosomes.h
worlds.o: resources.h running.h free.h defaults.h graphics.h random.h
