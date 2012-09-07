/*
    FileComp.h: public header file for a file selector widget
    Copyright (C) 1992 Robert Forsman

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 */

/*
 * $Log:	FileComp.h,v $
 * Revision 0.15  92/05/20  18:01:53  thoth
 * removed a secret parameter from a function typedef
 * 
 * Revision 0.14  92/05/20  17:12:06  thoth
 * this was released with FWF 3.0
 * 
 * Revision 0.13  92/05/12  13:40:31  thoth
 * added prototype for new XfwfFileCompleteTrimToPath
 * convenience procedure.
 * 
 * Revision 0.12  92/05/07  12:34:02  thoth
 * Added prototypes for new acceptableFilenameProcs that filter
 * out tilde backups.
 * 
 * Revision 0.11  92/05/06  10:54:00  thoth
 * still Beta release
 * 
 * Revision 0.10  92/05/06  10:47:47  thoth
 * still
 * 
 */

#ifndef _FileComplete_h
#define _FileComplete_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _FileCompleteClassRec *FileCompleteWidgetClass;
extern WidgetClass fileCompleteWidgetClass;
typedef struct _FileCompleteRec *FileCompleteWidget;

#define XtNfilename			"filename"
#define XtNsortProc			"sortProc"

#define XtNstatThreshold		"statThreshold"
#define XtNacceptableFilenameProc	"acceptableFilenameProc"
#define XtNfilenameProcData		"filenameProcData"
#define XtNacceptableStatsProc		"acceptableStatsProc"
#define XtNstatsProcData		"statsProcData"

#define XtNokLabel			"okLabel"
#define XtNshowOkButton			"showOkButton"

#define XtNcancelLabel			"cancelLabel"
#define XtNcancelCallback		"cancelCallback"
#define XtNshowCancelButton		"showCancelButton"

#define XtNfollowLinks			"followLinks"
#define XtNdirnamesOk			"dirnamesOk"
#define XtNstatLinks			"statLinks"

#define XtCFilename	"Filename"
#define XtCSortProc			"SortProc"

#define XtCStatThreshold                "StatThreshold"
#define XtCAcceptableFilenameProc       "AcceptableFilenameProc"
#define XtCFilenameProcData		"FilenameProcData"
#define XtCAcceptableStatsProc          "AcceptableStatsProc"
#define XtCStatsProcData		"StatsProcData"
	                                
#define XtCShowButton                   "ShowButton"
	                                
#define XtCFollowLinks                  "FollowLinks"
#define XtCDirnamesOk                   "DirnamesOk"
#define XtCStatLinks                    "StatLinks"

#define XtRAcceptableFileProc		"AcceptableFileProc"
#define XtRAcceptableStatsProc		"AcceptableStatsProc"
#define XtRFollowLinks                  "FollowLinks"

/* this is really an internal structure */
typedef struct {
  char	*filename;
  char	*repr;
} fileent;

typedef void (*FCSortProc)(
#if NeedFunctionPrototypes
fileent *base,
int nfiles
#endif
);

/**********************************************************************/

typedef Boolean (*FCAcceptableFilenameProc)(
#if NeedFunctionPrototypes
char *filename,
XtPointer client_data
#endif
);

#define FCAnyFilename	((FCAcceptableFilenameProc)0)

extern Boolean FCNoDotFiles(
#if NeedFunctionPrototypes
char *filename,
XtPointer ignored
#endif
);

extern Boolean FCNoTildeBackups(
#if NeedFunctionPrototypes
char *filename,
XtPointer ignored
#endif
);

extern Boolean FCNoTildeBackupsOrDotFiles(
#if NeedFunctionPrototypes
char *filename,
XtPointer ignored
#endif
);

extern Boolean FCRegexFiles(
#if NeedFunctionPrototypes
char *filename,
XtPointer regex,
XtAppContext super_secret_parameter___death_to_programmers_who_use_this
#endif
);

/**********************************************************************/

typedef Boolean (*FCAcceptableStatsProc)(
#if NeedFunctionPrototypes
char *filename,
struct stat *filestats,
XtPointer client_data,
XtAppContext super_secret_parameter___death_to_programmers_who_use_this
#endif
);

extern Boolean FCDirsOrRegexFiles(
#if NeedFunctionPrototypes
char *filename,
struct stat *filestats,
XtPointer regex,
XtAppContext super_secret_parameter___death_to_programmers_who_use_this
#endif
);

#define FCAnyStats	((FCAcceptableStatsProc)0)

/**********************************************************************/

enum FCFollowLinks {
  FCNever, FCWhenShorter, FCAlways
};

Widget XfwfFileCompleteText(
#if NeedFunctionPrototypes
Widget	w
#endif
);
 
Widget XfwfFileCompleteStatus(
#if NeedFunctionPrototypes
Widget	w
#endif
);
 
Widget XfwfFileCompleteFileList(
#if NeedFunctionPrototypes
Widget	w
#endif
);
 
Widget XfwfFileCompleteBox(
#if NeedFunctionPrototypes
Widget	w
#endif
);
 
Widget XfwfFileCompleteAddButton(
#if NeedFunctionPrototypes
     Widget	w,
     char	*name,
     XtCallbackProc	func,
     XtPointer	client_data
#endif
);
 
void XfwfFileCompleteTrimToPath(
#if NeedFunctionPrototypes
Widget	w
#endif
);

#ifdef __cplusplus
}
#endif

#endif
