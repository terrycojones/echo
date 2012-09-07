#
# Take out the -DDEBUG=1 to have the debugging stuff excluded.
# this will make the program take less space and run faster.
# It also makes the -verbose option give an error message.
#

EXECUTABLE = bedau_stats
CC = gcc
DEFINES = -DDEBUG=1
CFLAGS = -g -Wall -O $(DEFINES)
LIBS = -lm

OBJS = error.o main.o options.o syscalls.o hash.o

SRCS = defaults.h error.c globals.h hash.c hash.h main.c \
       main_options.c options.c options.h public.h syscalls.c \
       syscalls.h types.h


$(EXECUTABLE): $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)

clobber: clean
	rm -f $(EXECUTABLE)

depend:
	makedepend $(DEFINES) $(SRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

error.o: /usr/include/stdio.h types.h /usr/include/stdlib.h
error.o: /usr/include/sys/stdtypes.h hash.h public.h globals.h
hash.o: /usr/include/stdio.h /usr/include/string.h
hash.o: /usr/include/sys/stdtypes.h /usr/include/strings.h
hash.o: /usr/include/stdlib.h /usr/include/sys/types.h
hash.o: /usr/include/sys/sysmacros.h types.h hash.h public.h globals.h
hash.o: syscalls.h
main.o: /usr/include/stdio.h /usr/include/string.h
main.o: /usr/include/sys/stdtypes.h /usr/include/math.h
main.o: /usr/include/floatingpoint.h /usr/include/sys/ieeefp.h types.h
main.o: /usr/include/stdlib.h hash.h public.h globals.h syscalls.h defaults.h
main.o: options.h main_options.c
options.o: types.h /usr/include/stdlib.h /usr/include/sys/stdtypes.h hash.h
options.o: public.h globals.h options.h syscalls.h /usr/include/stdio.h
options.o: /usr/include/string.h /usr/include/ctype.h
syscalls.o: types.h /usr/include/stdlib.h /usr/include/sys/stdtypes.h hash.h
syscalls.o: public.h globals.h /usr/include/stdio.h /usr/include/malloc.h
syscalls.o: /usr/include/sys/types.h /usr/include/sys/sysmacros.h
syscalls.o: /usr/include/sys/stat.h
syscalls.o: /usr/include/stdio.h
types.o: /usr/include/stdlib.h /usr/include/sys/stdtypes.h hash.h public.h
types.o: globals.h
