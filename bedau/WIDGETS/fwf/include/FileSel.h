/*****************************************************************************

	FileSelector.h

	This file contains the user includes for the FileSelector widget.

	October 1990, Brian Totty

******************************************************************************/

#ifndef _FILESELECTOR_H_
#define _FILESELECTOR_H_

#include "ScrList.h"
#include "DirMgr.h"

extern WidgetClass	fileSelectorWidgetClass;

typedef struct _FileSelectorClassRec *FileSelectorWidgetClass;
typedef struct _FileSelectorRec      *FileSelectorWidget;

#define	XtNpathname XtNcurrentDirectory		/* For Compatibility */

#ifndef XtNtitle
#define	XtNtitle			"title"
#endif

#define	XtNcurrentDirectory		"currentDirectory"
#define	XtNcurrentFile			"currentFile"
#define	XtNsortMode			"sortMode"
#define	XtNpattern			"pattern"
#define	XtNokButtonCallback		"okButtonCallback"
#define	XtNcancelButtonCallback		"cancelButtonCallback"
#define	XtNselectionChangeCallback	"selectionChangeCallback"
#define	XtNshowOkButton			"showOkButton"
#define	XtNshowCancelButton		"showCancelButton"
#define	XtNfileSelected			"fileSelected"
#define	XtNflagLinks			"flagLinks"
#define	XtNcheckExistence		"checkExistence"

#define	XtCPathname			"Pathname"
#define	XtCFilename			"Filename"

typedef struct _FileSelectorOkButtonReturnStruct
{
	char *path;
	char *file;
	char *file_box_text;
} FileSelectorOkButtonReturnStruct;

typedef struct _FileSelectorSelectionChangeReturnStruct
{
	Boolean file_selected;
	char *path;
	char *file;
} FileSelectorSelectionChangeReturnStruct;

void FileSelectorChangeDirectory();
void FileSelectorRefresh();

#endif
