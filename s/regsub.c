#ifndef lint
static char *rcsid = "$Header: /tmp_mnt/vida/disks/disk5/Users/terry/r/echo/RCS/regsub.c,v 1.2 1992/11/16 07:05:45 terry Exp terry $";
#endif


/* !!!! THESE SOURCES ARE NOT THE H.S. ORIGINALS !!!! */


/*
 * regsub
 * @(#)regsub.c	1.3 of 2 April 86
 *
 *	Copyright (c) 1986 by University of Toronto.
 *	Written by Henry Spencer.  Not derived from licensed software.
 *
 *	Permission is granted to anyone to use this software for any
 *	purpose on any computer system, and to redistribute it freely,
 *	subject to the following restrictions:
 *
 *	1. The author is not responsible for the consequences of use of
 *		this software, no matter how awful, even if they arise
 *		from defects in it.
 *
 *	2. The origin of this software must not be misrepresented, either
 *		by explicit claim or by omission.
 *
 *	3. Altered versions must be plainly marked as such, and must not
 *		be misrepresented as being the original software.
 */
#include <stdio.h>
#include "types.h"
#include <regexp.h>
#include "graphics.h"
#include "regmagic.h"
#include "globals.h"

#ifndef CHARBITS
#define	UCHARAT(p)	((int)*(unsigned char *)(p))
#else
#define	UCHARAT(p)	((int)*(p)&CHARBITS)
#endif

/*
 - regsub - perform substitutions after a regexp match
 */
void
regsub(prog, source, dest)
regexp *prog;
char *source;
char *dest;
{
	register char *src;
	register char *dst;
	register char c;
	register int no;
	register int len;
	extern char *strncpy();

	rcsid = rcsid;

	if (prog == NULL || source == NULL || dest == NULL) {
		popup_message("NULL parm to regsub");
		return;
	}
	if (UCHARAT(prog->program) != MAGIC) {
		popup_message("damaged regexp fed to regsub");
		return;
	}

	src = source;
	dst = dest;
	while ((c = *src++) != '\0') {
		if (c == '&')
			no = 0;
		else if (c == '\\' && '0' <= *src && *src <= '9')
			no = *src++ - '0';
		else
			no = -1;

		if (no < 0) {	/* Ordinary character. */
			if (c == '\\' && (*src == '\\' || *src == '&'))
				c = *src++;
			*dst++ = c;
		} else if (prog->startp[no] != NULL && prog->endp[no] != NULL) {
			len = prog->endp[no] - prog->startp[no];
			(void) strncpy(dst, prog->startp[no], len);
			dst += len;
			if (len != 0 && *(dst-1) == '\0') {	/* strncpy hit NUL. */
				popup_message("damaged match string");
				return;
			}
		}
	}
	*dst++ = '\0';
}
