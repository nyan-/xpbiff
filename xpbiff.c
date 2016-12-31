
static char	sccsid[] = "@(#)xpbiff.c	1.27	10/25/93";

/*
 * xpbiff - popup biff for X
 * 
 * Author: Kazuhiko Shutoh, 1993
 *
 *  Special thanks for comment, bug fix, testing to:
 *
 *  Akihisa Kurashima <kura@mmp.cl.nec.co.jp>
 *  Andrew Krzywdzinski <andrew@cs.utk.edu>
 *  Andy Malis <malis@BBN.COM>
 *  Bob Primmer <rprimmer%lemond.webo.dg.com@RELAY.CS.NET>
 *  Chris Garrigues <7thSon@SLCS.SLB.COM>
 *  David Asano <david@comm.toronto.edu>
 *  Felix Ingrand <felix@ai.sri.com>
 *  Hidetsugu Nakashio <naka@soum.co.jp>
 *  Kazuyoshi SEI <sei@randd.jba.co.jp> 
 *  Ken Nelson <nelson@wrl.EPI.COM>
 *  Luis Miguel Silveira <lms@sobolev.mit.edu>
 *  MAKOTO "Mark'n" Matsushita <matusita@ics.es.osaka-u.ac.jp>
 *  Michael Glad <glad@daimi.aau.dk>
 *  Peter Jackson <peterj@cs.adelaide.edu.au>
 *  Ricardo Cardenas <rcardena@us.oracle.com>
 *  Robert Lupton (the Good) <rhl@astro.Princeton.EDU>
 *  Steven Reiz <sreiz@cs.vu.nl>
 *  Sting Takubon TAKEUCHI <take@train.ncl.omron.co.jp>
 *  Yoshikazu Yamamoto <y-yamamt@ics.es.osaka-u.ac.jp>
 *  chiba@is.s.u-tokyo.ac.jp (Chiba)
 *  hkato@krc.sony.co.jp (Hiroshi KATO)
 *  hotta@flab.fujitsu.co.jp (Yuji Hotta)
 *  imp@Solbourne.COM (Warner Losh)
 *  jgraf@smdgr1.intel.com (Jim Graf )
 *  lindheim@sampson.ccsf.caltech.edu (Jan Lindheim)
 *  lusk@antares.mcs.anl.gov (Rusty Lusk)
 *  masuhara@is.s.u-tokyo.ac.jp
 *  murase@ctf5.drl.mei.co.jp (Kouichi Murase)
 *  nishida@phantom.src.ricoh.co.jp (Akihiro Nishida)  
 *  rekers@cwi.nl (Jan Rekers)
 *  remy@cs.uoregon.edu (Remy Evard)
 *  saka@pfu.fujitsu.co.JP (SAKAEDA Ken-ichi)
 *  sako@joke.cl.nec.co.jp (SAKO Shinji)
 *  site@mfd.cs.fujitsu.co.jp (SAITOH-Ely-Yasuhiro)
 *  tshira@mtl.t.u-tokyo.ac.jp
 *  vita@oxford.dab.ge.com (Mark Vita)
 *
 *
 *						THANKS A LOT!!!!
 *
 *
 * Permission to use, copy, modify and distribute without charge this software,
 * documentation, images, etc. is granted, provided that this comment and the
 * author's name is retained.  The author assumes no responsibility for lost
 * sleep as a consequence of use of this software.
 * 
 * Send any comments, bug reports, etc. to: shutoh@lm.emi.yamaha.co.jp
 * 
 */

#include "config.h"

#include <paths.h>	/* _PATH_MAILDIR */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#ifdef SUN_AUDIO
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <multimedia/audio_filehdr.h>
#include <sun/audioio.h>
#ifndef min
#define min(x,y)	((x) < (y)? (x): (y))
#endif
#endif /* SUN_AUDIO */

#ifdef BSD_AUDIO
#include <sys/soundcard.h>
#define MAX_SOUND_VOL 95
#define MIN_SOUND_VOL 05
#endif

#ifndef X11R3
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xmu/Drawing.h>
#include <X11/Xmu/Converters.h>
#ifdef XI18N
#ifdef X11R6
#include <X11/Xlocale.h>
#else
#include <X11/Xaw/Xawi18n.h>
#endif /* X11R6 */
#endif /* XI18N */
#else
#include <X11/Box.h>
#include <X11/Label.h>
#include <X11/Command.h>
#include <X11/Cardinals.h>
typedef char* XtPointer;
#endif

#ifdef XPM
#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#define RPixmap2	"pixmap2"
typedef Pixmap		Pixmap2[2];
#else
#define RPixmap2	XtRBitmap
typedef Pixmap		Pixmap2;
#endif
#ifndef O_RDONLY
#include <fcntl.h>
#endif

#ifdef SHAPE
#include <X11/extensions/shape.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef HAVE_GETPWUID
#include <pwd.h>
#endif /* HAVE_GETPWUID */

#ifdef YOUBIN

#ifdef SYSV
#include <string.h>                     /* For strchr(). */
#else /* not SYSV */
#include <strings.h>
#define strchr(x, y)        index((x), (y)) 
#define strrchr(x, y)       rindex((x), (y))
#endif /* not SYSV */
#include <ctype.h>			/* For isspace() */
#include <sys/param.h>
#include <sys/signal.h>

#include <errno.h>
#endif /* YOUBIN */

#include "patchlevel.h"
#include "arrive.bitmap"
#include "no.bitmap"

#define PI	3.141592654
#define ARRIVE	1
#define NO	0
#ifdef	SLOW_MACHINE
#define ROTATION	180	/* For Slow Server */
#else
#define ROTATION	720
#endif

#ifndef YOUBIN

#define SPOOLPATH	_PATH_MAILDIR

#else /* YOUBIN */

#define CHILD_PROGRAM_NAME	"youbin"
#define MESS_LEN            	256             /* Max message length. */
#define HEADER_LEN      	(MESS_LEN + (80 * (3 + 7)))

#ifndef EXIT_FAILURE
#define EXIT_FAILURE		1
#endif /* ! EXIT_FAILURE */

#ifndef BINDIR
#define	BINDIR			"."
#endif	/* !BINDIR */

#endif /* YOUBIN */

#ifndef DEFAULT_SCAN_COMMAND_PATH
#define DEFAULT_SCAN_COMMAND_PATH	NULL
#endif

#ifndef DEFAULT_SCAN_COMMAND
#define DEFAULT_SCAN_COMMAND	"scan -width 160 -format '\
%<(mymbox{from})To: %(friendly{to})%|\
From: %<{posted-from}%(friendly{posted-from})%|%(friendly{from})%>%>\
%<{subject}\nSubject: %{subject}%>' -file"
#endif

XtCallbackProc	quit();
void		redraw_window();
#ifdef SHAPE
void		shape_window();
#endif
XtCallbackProc	redraw_callback();
XtCallbackProc	BreakPopup();
XtCallbackProc	popup_again();
void		PopupMailHeader();
void		AnimateBiff();

#ifndef YOUBIN 
static void		split_prefer();
static char		*read_headers();
static Boolean		read_header();
static struct field	*read_field();
static void		put_in_fbuf();
static void		put_in_str();
static void		put_in();
#endif /* !YOUBIN */

static void beep();

#ifndef XI18N
#undef JCONVERT
#endif

#ifdef JCONVERT
typedef enum {UNKNOWN, JIS, SJIS, EUC} Jcode;
typedef struct {
    char	*name;
    Jcode	code;
} Locale_ent;
static void		jis2euc();
static void		jis2sjis();
#endif
#ifdef ISO_2022_JP
typedef enum {NOMATCH, BASE64, QUOTED} Encoding;
static char		*bit2byte();
static char		*quoted2byte();
static void		mime2jis();
static Encoding		check_charset();
#endif

#ifdef MHSCAN
static char		*mh_scan();
#endif

Widget		toplevel, biff, header, info_base, info, popdown_button;

#ifndef YOUBIN

XtTimerCallbackProc Polling();
char           *GetMailHeader();

#else  /* define YOUBIN */

static void     get_mail_header();
static void     read_from_child();
char            *prog_name;                     /* This program name. */
char            child_process[MAXPATHLEN + 1];
static int exit_hundler();

#endif /* YOUBIN */

/*
 * widget tree:
 * 
 * toplevel --- biff ... header --- info_base ---+--- popdown_button 
 *						 +--- info
 * 
 */

GC		gcArrive, gcNo, gcAnimate, gcAnimateBack;
Pixmap		workPixmap;
int		width, height;
unsigned char	current_status = NO;
unsigned char	showing_header = 0;
long		mail_size = 0;

unsigned int	Arrive_pixmap_width, Arrive_pixmap_height;
unsigned int	Arrive_pixmap_depth;

unsigned int	No_pixmap_width, No_pixmap_height;
unsigned int	No_pixmap_depth;

#ifdef YOUBIN
XtIntervalId	timeout_id;
#else
XtIntervalId	polling_id;
char		spool_path[100];
#endif

static String		fallback_resources[] = {
    "*info_base.hSpace: 0",
    "*info_base.vSpace: 0",
    "*popdown_button.label: PopDown",
#ifdef XI18N
    "*fontSet: -*--14-*",
#ifdef X11R6
    "*international: True",
#endif
#endif
    NULL
};

typedef struct _appRes {
#ifdef XPM
    Pixmap	arrivePixmaps[2];
    Pixmap	noPixmaps[2];
#define	arrivePixmap	arrivePixmaps[0]
#define	noPixmap	noPixmaps[0]
#else
    Pixmap	arrivePixmap;
    Pixmap	noPixmap;
#endif
    Pixel	colors[4];
#ifndef YOUBIN
    String	prefers;
    int		polling_time;
    String	spoolPath;
#endif
    Boolean	mono;
    String	popupGeometry;
    Boolean	raise;
    Boolean	stay_raised;
    Boolean	animate;
    Boolean	popup;
    int		popup_time;
    Boolean	bell;
    Boolean	popdown;
#ifdef YOUBIN
    Boolean	use_pipe;
    String	server;
    Boolean	preserve;
#endif
#ifdef MHSCAN
    Boolean	mhscan;
    String	scanCommand;
    String	scanCommandPath;
#endif
#ifdef SHAPE
    Boolean	shape;
    Pixmap	arrivePixmapMask;
    Pixmap	noPixmapMask;
#endif
#if (defined(SUN_AUDIO) || defined(BSD_AUDIO))
    String	mail_sndfile;
    int		volume;
#endif
} AppResourcesRec, *AppResources;

#define	SetResOffset(n, field)	resources[n].resource_offset = ((char *)&appResources.field - (char *)&appResources)
#define	offset(field)	XtOffset(struct _appRes *, field)

AppResourcesRec appResources;

static XtResource	resources[] = {
    {"arrivePixmap", "ArrivePixmap", RPixmap2, sizeof(Pixmap2),
     offset(arrivePixmap), XtRString, "None"},
    {"nomailPixmap", "NomailPixmap", RPixmap2, sizeof(Pixmap2),
     offset(noPixmap), XtRString, "None"},
    {"nomailForeground", "NomailForeground", XtRPixel, sizeof(Pixel),
     offset(colors[0]), XtRString, "black"},
    {"nomailBackground", "NomailBackground", XtRPixel, sizeof(Pixel),
     offset(colors[1]), XtRString, "cyan"},
    {"arriveForeground", "ArriveForeground", XtRPixel, sizeof(Pixel),
     offset(colors[2]), XtRString, "yellow"},
    {"arriveBackground", "ArriveBackground", XtRPixel, sizeof(Pixel),
     offset(colors[3]), XtRString, "red"},
#ifndef YOUBIN
    {"pollingTime", "PollingTime", XtRInt, sizeof(int),
     offset(polling_time), XtRImmediate, (XtPointer)15000},
    {"prefer", "Prefer", XtRString, sizeof(String),
     offset(prefers), XtRString, (XtPointer)"From Subject"},
    {"spoolPath", "SpoolPath", XtRString, sizeof(String),
     offset(spoolPath), XtRString, None},
#endif
    {"mono", "Mono", XtRBoolean, sizeof(Boolean),
     offset(mono), XtRImmediate, (XtPointer)False},
    {"popupGeometry", "PopupGeometry", XtRString, sizeof(String),
     offset(popupGeometry), XtRString, NULL},
    {"raise", "Raise", XtRBoolean, sizeof(Boolean),
     offset(raise), XtRImmediate, (XtPointer)True},
    {"stayRaised", "StayRaised", XtRBoolean, sizeof(Boolean),
     offset(stay_raised), XtRImmediate, (XtPointer)False},
    {"animate", "Animate", XtRBoolean, sizeof(Boolean),
     offset(animate), XtRImmediate, (XtPointer)True},
    {"popup", "Popup", XtRBoolean, sizeof(Boolean),
     offset(popup), XtRImmediate, (XtPointer)True},
    {"popupTime", "PopupTime", XtRInt, sizeof(int),
     offset(popup_time), XtRImmediate, (XtPointer)15000},
    {"bell", "Bell", XtRBoolean, sizeof(Boolean),
     offset(bell), XtRImmediate, (XtPointer)True},
    {"popdown", "Popdown", XtRBoolean, sizeof(Boolean),
     offset(popdown), XtRImmediate, (XtPointer)True},
#ifdef YOUBIN
    {"pipe", "Pipe", XtRBoolean, sizeof(Boolean),
     offset(use_pipe), XtRString, "false"},
    {"server", "Server", XtRString, sizeof(String),
     offset(server), XtRString, YOUBIN_DEFAULT_SERVER_HOST},
    {"preserve", "preserve", XtRBoolean, sizeof(Boolean),
     offset(preserve), XtRString, "true"},
#endif
#ifdef MHSCAN
    {"mhscan", "Mhscan", XtRBoolean, sizeof(Boolean),
     offset(mhscan), XtRImmediate, (XtPointer)False},
    {"scanCommand", "ScanCommand", XtRString, sizeof(String),
     offset(scanCommand), XtRString, DEFAULT_SCAN_COMMAND},
    {"scanCommandPath", "ScanCommandPath", XtRString, sizeof(String),
     offset(scanCommandPath), XtRString, DEFAULT_SCAN_COMMAND_PATH},
#endif
#ifdef SHAPE
    {"shape", "Shape", XtRBoolean, sizeof(Boolean),
     offset(shape), XtRImmediate, (XtPointer)False},
    {"arrivePixmapMask", "ArrivePixmapMask", XtRBitmap, sizeof(Pixmap),
     offset(arrivePixmapMask), XtRBitmap, None},
    {"nomailPixmapMask", "NomailPixmapMask", XtRBitmap, sizeof(Pixmap),
     offset(noPixmapMask), XtRBitmap, None},
#endif
#if (defined(SUN_AUDIO) || defined(BSD_AUDIO))
    {"mailSndFile", "MailSndFile", XtRString, sizeof(String),
     offset(mail_sndfile), XtRString, None},
    {"volume", "Volume", XtRInt, sizeof(int),
     offset(volume), XtRString, "33"},
#endif    
};

static XrmOptionDescRec options[] = {
    {"-nfg",		"*nomailForeground",	XrmoptionSepArg,	NULL},
    {"-nbg",		"*nomailBackground",	XrmoptionSepArg,	NULL},
    {"-afg",		"*arriveForeground",	XrmoptionSepArg,	NULL},
    {"-abg",		"*arriveBackground",	XrmoptionSepArg,	NULL},
#ifndef YOUBIN    
    {"-polling_time",	"*pollingTime",		XrmoptionSepArg,	NULL},
#endif
    {"-noraise",	"*popup",		XrmoptionNoArg,		"false"},
    {"-keepup",		"*stayRaised",		XrmoptionNoArg,		"true"},
    {"-nopopup",	"*popup",		XrmoptionNoArg,		"false"},
    {"-noanimate",	"*animate",		XrmoptionNoArg,		"false"},
    {"-nobell",		"*bell",		XrmoptionNoArg,		"false"},
    {"-nopopdown",	"*popdown",		XrmoptionNoArg,		"false"},
    {"-arrive_bitmap",	"*arrivePixmap",	XrmoptionSepArg,	NULL},
    {"-no_bitmap",	"*nomailPixmap",	XrmoptionSepArg,	NULL},
    {"-popup_geometry", "*popupGeometry",	XrmoptionSepArg,	NULL},
    {"-popup_time",	"*popupTime",		XrmoptionSepArg,	NULL},
#ifndef YOUBIN 
    {"-prefer",		"*prefer",		XrmoptionSepArg,	NULL},
#endif
#ifdef MHSCAN
    {"-mhscan",		"*mhscan",		XrmoptionNoArg,		"true"},
    {"-nomhscan",	"*mhscan",		XrmoptionNoArg,		"false"},
    {"-scan_command",	"*scanCommand",		XrmoptionSepArg,	NULL},
    {"-scan_command_path", "*scanCommandPath",	XrmoptionSepArg,	NULL},
#endif
#ifdef SHAPE
    {"-shape",		"*shape",		XrmoptionNoArg,		"true"},
    {"-noshape",	"*shape",		XrmoptionNoArg,		"false"},
    {"-arrive_bitmap_mask", "*arrivePixmapMask", XrmoptionSepArg,	NULL},
    {"-no_bitmap_mask", "*nomailPixmapMask",	XrmoptionSepArg,	NULL},
#endif
#ifdef YOUBIN
    {"-pipe",		"*pipe",		XrmoptionNoArg,		"false"},
    {"-server",		"*server",		XrmoptionSepArg,	NULL},
    {"-preserve",	"*preserve",		XrmoptionNoArg,		"true"},
#endif /* YOUBIN */
#if (defined(SUN_AUDIO) || defined(BSD_AUDIO))
    {"-sndfile",	"*mailSndFile",		XrmoptionSepArg,	NULL},
    {"-volume",		"*volume",		XrmoptionSepArg,	NULL},
#endif
};

#ifdef XPM
static XtConvertArgRec convertArgs[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof(Screen *)},
};
extern void	CvtStringToPixmap();
#endif

#ifdef JCONVERT
static Locale_ent	locale_list[] = {
    {"ja_JP.EUC",	EUC},
    {"ja_JP.eucJP",	EUC},
    {"ja_JP.SJIS",	SJIS},
    {"ja_JP.jis7",	JIS},
    {"ja_JP.jis8",	JIS},
    {"ja_JP.pjis",	JIS},
    {"ja_JP.mscode",	SJIS},
    {"ja_JP.ujis",	EUC},
    {"japanese",	EUC},
    {"ja", EUC},
    {NULL, UNKNOWN},
};
static Jcode	jcode;
#endif

static Atom	wm_delete_window;

main(argc, argv)
int		argc;
char	      **argv;
{
    XtAppContext	app_con;
    Arg			args[10];
    XGCValues		values;
    XtTranslations	newTranslations;
    static XtActionsRec redrawActions[] = {
	{"expose", (XtActionProc) redraw_callback},
	{"select", (XtActionProc) popup_again},
	{"quit", (XtActionProc) quit},
    };

    static char		*overrideTranslations =
	"<Expose>:	expose() \n\
	 <ResReq>:	expose() \n\
	 <Btn1Up>:	select()";

    int			dummy;
    int			i;

#ifdef HAVE_GETPWUID
    struct passwd	*pw;
#endif
#ifdef JCONVERT
    char		*locale_name;
    Locale_ent		*p;
#endif

#ifdef YOUBIN
    FILE            *pfile;     /* For popen(). */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif    
    char            serv_name[MAXHOSTNAMELEN];
#else
    XtTimerCallbackProc Polling();
#endif /* !YOUBIN */

#ifdef YOUBIN
    if ((prog_name = strrchr(argv[0], '/')) != NULL) {
	prog_name++;
    } else {
	prog_name = argv[0];
    }
#else	/* !YOUBIN */
    if (appResources.spoolPath != NULL && appResources.spoolPath[0] != '\0') {
	strcpy(spool_path, appResources.spoolPath);
    } else {
    	strcpy(spool_path, SPOOLPATH);
    }
    if (spool_path[strlen(spool_path) - 1] != '/') {
	strcat(spool_path, "/");
    }

#ifdef GETENV_MAIL
    if (getenv("MAIL") != NULL) {
	strcpy(spool_path, getenv("MAIL"));
    } else {
#endif /* GETENV_MAIL */

#ifdef HAVE_GETPWUID
	pw = getpwuid(getuid());
	strcat(spool_path, pw->pw_name);
#elif defined(NO_CUSERID)
	strcat(spool_path, getenv("USER"));
#else
	strcat(spool_path, (char *)cuserid(NULL));
#endif /* NO_CUSERID && HAVE_GETPWUID */

#ifdef GETENV_MAIL
    }
#endif /* GETENV_MAIL */

#endif /* !YOUBIN */

#ifdef XI18N
#ifdef JCONVERT
    /* Use environment variable LANG as Locale */
    if ((locale_name = setlocale(LC_CTYPE, "")) != NULL) {
	Locale_ent	*p;

	jcode = UNKNOWN;
	for (p = locale_list; p->name; p++) {
	    if (strcmp(p->name, locale_name) == 0) {
		jcode = p->code;
		break;
	    }
	}
    }
#else
    setlocale(LC_CTYPE, "");
#endif
    XtSetLanguageProc(NULL, NULL, NULL);
#endif
    toplevel = XtAppInitialize(&app_con, "XPbiff",
			       options, XtNumber(options), &argc, argv,
			       fallback_resources, NULL, ZERO);

    if (argc > 1) {
	if (strcmp("-patchlevel", *++argv) == 0) {
	    printf("xpbiff -  written by Kazuhiko Shutoh <shutoh@lm.emi.yamaha.co.jp>\nSCCS ID : %s\nPatchlevel : %d\n", sccsid, PATCHLEVEL);
	    printf("\nSpecial thanks to:\n\
	Akihisa Kurashima <kura@mmp.cl.nec.co.jp>\n\
	Andrew Krzywdzinski <andrew@cs.utk.edu>\n\
	Andy Malis <malis@BBN.COM>\n\
	Bob Primmer <rprimmer%lemond.webo.dg.com@RELAY.CS.NET>\n\
	Chris Garrigues <7thSon@SLCS.SLB.COM>\n\
	David Asano <david@comm.toronto.edu>\n\
	Felix Ingrand <felix@ai.sri.com>\n\
	Hidetsugu Nakashio <naka@soum.co.jp>\n\
	Kazuyoshi SEI <sei@randd.jba.co.jp> \n\
	Ken Nelson <nelson@wrl.EPI.COM>\n\
	Luis Miguel Silveira <lms@sobolev.mit.edu>\n\
	MAKOTO Matsushita <matusita@ics.es.osaka-u.ac.jp>\n\
	Michael Glad <glad@daimi.aau.dk>\n\
	Peter Jackson <peterj@cs.adelaide.edu.au>\n\
	Ricardo Cardenas <rcardena@us.oracle.com>\n\
	Robert Lupton (the Good) <rhl@astro.Princeton.EDU>\n\
	Steven Reiz <sreiz@cs.vu.nl>\n\
	Sting Takubon TAKEUCHI <take@train.ncl.omron.co.jp>\n\
	Yoshikazu Yamamoto <y-yamamt@ics.es.osaka-u.ac.jp>\n\
	chiba@is.s.u-tokyo.ac.jp (Chiba)\n\
	hkato@krc.sony.co.jp (Hiroshi KATO)\n\
	hotta@flab.fujitsu.co.jp (Yuji Hotta)\n\
	imp@Solbourne.COM (Warner Losh)\n\
	jgraf@smdgr1.intel.com (Jim Graf )\n\
	lindheim@sampson.ccsf.caltech.edu (Jan Lindheim)\n\
	lusk@antares.mcs.anl.gov (Rusty Lusk)\n\
	masuhara@is.s.u-tokyo.ac.jp\n\
	murase@ctf5.drl.mei.co.jp (Kouichi Murase)\n\
	nishida@phantom.src.ricoh.co.jp (Akihiro Nishida)\n\
	rekers@cwi.nl (Jan Rekers)\n\
	remy@cs.uoregon.edu (Remy Evard)\n\
	saka@pfu.fujitsu.co.JP (SAKAEDA Ken-ichi)\n\
	sako@joke.cl.nec.co.jp (SAKO Shinji)\n\
	site@mfd.cs.fujitsu.co.jp (SAITOH-Ely-Yasuhiro)\n\
	tshira@mtl.t.u-tokyo.ac.jp\n\
	vita@oxford.dab.ge.com (Mark Vita)\n");
	    exit(0);
	}
	else {
	    fprintf(stderr, "Usage : xpbiff [Toolkit-Options]\n");
	    fprintf(stderr, "		[-nfg noarrive_bitmap_foreground color]\n");
	    fprintf(stderr, "		[-nbg noarrive_bitmap_background_color]\n");
	    fprintf(stderr, "		[-afg arrive_bitmap_foreground_color]\n");
	    fprintf(stderr, "		[-abg arrive_bitmap_background_color]\n");
#ifndef YOUBIN	    
	    fprintf(stderr, "		[-polling_time	time(msec)]\n");
#endif
	    fprintf(stderr, "		[-noraise]\n");
	    fprintf(stderr, "		[-nopopup]\n");
	    fprintf(stderr, "		[-noanimate]\n");
	    fprintf(stderr, "		[-nopopdown]\n");
	    fprintf(stderr, "		[-keepup]\n");
	    fprintf(stderr, "		[-nobell]\n");
#ifndef YOUBIN
	    fprintf(stderr, "		[-prefer prefer_list]\n");
#endif
#ifdef MHSCAN
	    fprintf(stderr, "		[-mhscan]\n");
	    fprintf(stderr, "		[-nomhscan]\n");
	    fprintf(stderr, "		[-scan_command command_string]\n");
	    fprintf(stderr, "		[-scan_command_path command_path]\n");
#endif
#ifdef SHAPE
	    fprintf(stderr, "		[-shape]\n");
	    fprintf(stderr, "		[-noshape]\n");
#endif
	    fprintf(stderr, "		[-mono]\n");
	    fprintf(stderr, "		[-patchlevel]\n");
	    fprintf(stderr, "		[-no_bitmap bitmap_file]\n");
	    fprintf(stderr, "		[-arrive_bitmap bitmap_file]\n");
#ifdef SHAPE
	    fprintf(stderr, "		[-no_bitmap_mask bitmap_file]\n");
	    fprintf(stderr, "		[-arrive_bitmap_mask bitmap_file]\n");
#endif
	    fprintf(stderr, "		[-popup_geometry geometry]\n");
	    fprintf(stderr, "		[-popup_time time(msec)]\n"); 
#ifdef YOUBIN
	    fprintf(stderr, "		[-pipe]\n");
	    fprintf(stderr, "		[-server host]\n");
	    fprintf(stderr, "		[-nopreserve]\n");
#endif
#if (defined(SUN_AUDIO) || defined(BSD_AUDIO))
	    fprintf(stderr, "		[-sndfile audio_file]\n");
	    fprintf(stderr, "		[-volume percentage]\n");
#endif
	    exit(0);
	}
    }

#ifdef XPM
    XtAppAddConverter(app_con, XtRString, RPixmap2, CvtStringToPixmap,
		      convertArgs, XtNumber(convertArgs));
#endif

    XtGetApplicationResources(toplevel, (XtPointer)&appResources,
			      resources, XtNumber(resources), NULL, 0);

    if (appResources.mono == True) {
	appResources.colors[0] = appResources.colors[3] = BlackPixelOfScreen(XtScreen(toplevel));
	appResources.colors[1] = appResources.colors[2] = WhitePixelOfScreen(XtScreen(toplevel));
    }

#ifndef YOUBIN
    split_prefer(appResources.prefers);
#endif

#ifdef XPM
    if (appResources.arrivePixmapMask == None)
	appResources.arrivePixmapMask = appResources.arrivePixmaps[1];
    if (appResources.noPixmapMask == None)
	appResources.noPixmapMask = appResources.noPixmaps[1];
#endif

#ifdef SHAPE
    if (appResources.shape == True
       && !XShapeQueryExtension(XtDisplay(toplevel), &dummy, &dummy))
	appResources.shape = False;
#endif

    if (appResources.noPixmap == None) {
	appResources.noPixmap = XCreatePixmapFromBitmapData
	    (XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
	     no_bits, no_width, no_height, 1, 0, 1);
	No_pixmap_width = no_width;
	No_pixmap_height = no_height;
	No_pixmap_depth = 1;
    }
    else {
	Window r;
	unsigned int x, y, bw;
	XGetGeometry(XtDisplay(toplevel), appResources.noPixmap,
		     &r, &x, &y,
		     &No_pixmap_width, &No_pixmap_height,
		     &bw, &No_pixmap_depth);
    }

    if (appResources.arrivePixmap == None) {
	appResources.arrivePixmap = XCreatePixmapFromBitmapData
	    (XtDisplay(toplevel), RootWindowOfScreen(XtScreen(toplevel)),
	     arrive_bits, arrive_width, arrive_height,
	     1, 0, 1);
	Arrive_pixmap_width = arrive_width;
	Arrive_pixmap_height = arrive_height;
	Arrive_pixmap_depth = 1;
    }
    else {
	Window r;
	unsigned int x, y, bw;
	XGetGeometry(XtDisplay(toplevel), appResources.arrivePixmap,
		     &r, &x, &y,
		     &Arrive_pixmap_width, &Arrive_pixmap_height,
		     &bw, &Arrive_pixmap_depth);
    }

    i = 0;
    XtSetArg(args[i], XtNwidth, No_pixmap_width);
    i++;
    XtSetArg(args[i], XtNheight, No_pixmap_height);
    i++;
    biff = XtCreateManagedWidget("biff", widgetClass, toplevel, args, i);

    XtAppAddActions(app_con, redrawActions, XtNumber(redrawActions));
    newTranslations = XtParseTranslationTable(overrideTranslations);
    XtOverrideTranslations(biff, newTranslations);
    XtOverrideTranslations
	(toplevel, XtParseTranslationTable("<Message>WM_PROTOCOLS: quit()"));

    XtRealizeWidget(toplevel);

    wm_delete_window = XInternAtom
	(XtDisplay(toplevel), "WM_DELETE_WINDOW", False);
    (void) XSetWMProtocols
	(XtDisplay(toplevel), XtWindow(toplevel), &wm_delete_window, 1);

    /* Get graphic context	 */

    values.foreground	= appResources.colors[0];
    values.background	= appResources.colors[1];
    values.graphics_exposures = False;
    gcNo = XtGetGC(toplevel,
		   GCForeground | GCBackground | GCGraphicsExposures,
		   &values);
    values.foreground	= appResources.colors[2];
    values.background	= appResources.colors[3];
    values.graphics_exposures = False;
    gcArrive = XtGetGC(toplevel,
		       GCForeground | GCBackground | GCGraphicsExposures,
		       &values);

    /* Create Pixmap's */

    workPixmap = XCreatePixmap(XtDisplay(toplevel), XtWindow(biff),
			       Arrive_pixmap_width, Arrive_pixmap_height,
			       DefaultDepthOfScreen(XtScreen(toplevel)));

    values.foreground	= appResources.colors[2];
    values.graphics_exposures = False;
    gcAnimate = XtGetGC(toplevel,
			GCForeground | GCGraphicsExposures,
			&values);

    values.foreground	= appResources.colors[3];
    values.graphics_exposures = False;
    gcAnimateBack = XtGetGC(toplevel,
			    GCForeground | GCGraphicsExposures,
			    &values);

    /* Interval timer start	 */
#ifndef YOUBIN
    polling_id = XtAppAddTimeOut(app_con,
				 (unsigned long) appResources.polling_time, (XtTimerCallbackProc) Polling, NULL);
#else  /* YOUBIN */
    if (appResources.server == NULL) {
	gethostname(serv_name, sizeof(serv_name));
	appResources.server = serv_name;
    }

    /* make new process group */
    if (setsid() == -1)
	if (errno != EPERM){	/* current process isn't the process leader, */
	    perror("setsid");	/* but an error happened */
	    exit(EXIT_FAILURE);
	}

    if (appResources.use_pipe == False) {
	sprintf(child_process, "%s/%s -b -s %s",
		BINDIR, CHILD_PROGRAM_NAME, appResources.server);
	if ((pfile = popen(child_process, "r")) == NULL) {
	    perror("popen");
	    exit(EXIT_FAILURE);
	}
	XtAppAddInput(app_con, fileno(pfile), (XtPointer)XtInputReadMask,
		      read_from_child, NULL);
    } else {
	XtAppAddInput(app_con, fileno(stdin), (XtPointer)XtInputReadMask,
		      read_from_child, NULL);
    }

    /* when the xpbiff window is destroyed, all the children will be killed */
    XSetIOErrorHandler(exit_hundler);
#endif /* YOUBIN */
#ifdef I18N
    if (XSupportsLocale() == False){
	fprintf(stderr, "%s: Can't use Locale.", argv[0]);
	exit(1);
    }
#endif
#ifdef SHAPE
    shape_window();
#endif
    XtAppMainLoop(app_con);
}

XtCallbackProc quit(w, event, params, nparams)
Widget		w;
XEvent		*event;
String		*params;
Cardinal	*nparams;
{
    exit(0);
}

void redraw_window()
{
    if (current_status == ARRIVE) {
	if (Arrive_pixmap_depth == 1)
	    XCopyPlane(XtDisplay(toplevel), appResources.arrivePixmap, XtWindow(biff),
		       gcArrive,
		       0, 0, Arrive_pixmap_width, Arrive_pixmap_height, 0, 0, 1);
	else
	    XCopyArea(XtDisplay(toplevel), appResources.arrivePixmap, XtWindow(biff),
		      gcArrive,
		      0, 0, Arrive_pixmap_width, Arrive_pixmap_height, 0, 0);
    }
    else {
	if (No_pixmap_depth == 1)
	    XCopyPlane(XtDisplay(toplevel), appResources.noPixmap, XtWindow(biff),
		       gcNo,
		       0, 0, No_pixmap_width, No_pixmap_height, 0, 0, 1);
	else
	    XCopyArea(XtDisplay(toplevel), appResources.noPixmap, XtWindow(biff),
		      gcNo, 0, 0, No_pixmap_width, No_pixmap_height, 0, 0);
    }
}

#ifdef SHAPE
void shape_window()
{
    if (appResources.shape == True) {
	if (current_status == ARRIVE) {
	    if (appResources.arrivePixmapMask == None) {
		XRectangle	rect;
		rect.x = rect.y = 0;
		rect.width = Arrive_pixmap_width;
		rect.height = Arrive_pixmap_height;
		XShapeCombineRectangles(XtDisplay(toplevel), XtWindow(toplevel),
					ShapeBounding, 0, 0, &rect, 1,
					ShapeSet, 0);
	    }
	    else
		XShapeCombineMask(XtDisplay(toplevel), XtWindow(toplevel),
				  ShapeBounding, 0, 0, appResources.arrivePixmapMask,
				  ShapeSet);
	}
	else {
	    if (appResources.noPixmapMask == None) {
		XRectangle	rect;
		rect.x = rect.y = 0;
		rect.width = No_pixmap_width;
		rect.height = No_pixmap_height;
		XShapeCombineRectangles(XtDisplay(toplevel), XtWindow(toplevel),
					ShapeBounding, 0, 0, &rect, 1,
					ShapeSet, 0);
	    }
	    else
		XShapeCombineMask(XtDisplay(toplevel), XtWindow(toplevel),
				  ShapeBounding, 0, 0, appResources.noPixmapMask,
				  ShapeSet);
	}
    }
}
#endif

XtCallbackProc redraw_callback(w, event, params, nparams)
Widget		w;
XEvent	       *event;
String	       *params;
Cardinal       *nparams;
{
    redraw_window();
}

#ifndef YOUBIN
XtTimerCallbackProc
Polling(client_data, id)
caddr_t		client_data;
XtIntervalId	*id;
{

    struct stat	    file_stat;
    char	   *mail_header;


    if ((stat(spool_path, &file_stat) == 0) && (file_stat.st_size != 0)) {
	/* There are Mail */

	if (current_status == NO) {
	    /* NEW mail !! */
	    current_status = ARRIVE;
	    mail_size = file_stat.st_size;
	    if (appResources.popup == True)
		mail_header = GetMailHeader();
	    XtResizeWidget(toplevel, Arrive_pixmap_width, Arrive_pixmap_height, 1);
	    XtResizeWidget(biff,  Arrive_pixmap_width, Arrive_pixmap_height, 0);
	    if (appResources.raise == True)
		XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    if (appResources.animate == True)
		AnimateBiff();
	    redraw_window();
#ifdef SHAPE
	    shape_window();
#endif
	    if (appResources.popup == True)
		PopupMailHeader(mail_header);
	    XSync(XtDisplay(toplevel), 0);
	    if (appResources.bell == True)
		beep(XtDisplay(toplevel));
	    /*	XBell(XtDisplay(toplevel), 0); */
	}
	else if (file_stat.st_size > mail_size) {
	    mail_size = file_stat.st_size;
	    if (appResources.popup == True)
		mail_header = GetMailHeader();
	    /* more come! only ringing bell. */
	    if (appResources.popup == True)
		BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	    XtResizeWidget(toplevel,  Arrive_pixmap_width, Arrive_pixmap_height, 1);
	    XtResizeWidget(biff,  Arrive_pixmap_width, Arrive_pixmap_height, 0);
	    if (appResources.raise == True)
		XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    if (appResources.animate == True)
		AnimateBiff();
	    redraw_window();
#ifdef SHAPE
	    shape_window();
#endif
	    if (appResources.popup == True)
		PopupMailHeader(mail_header);
	    XSync(XtDisplay(toplevel), 0);
	    if (appResources.bell == True)
		beep(XtDisplay(toplevel));
	    /*	XBell(XtDisplay(toplevel), 0); */
	}
	else {
	    mail_size = file_stat.st_size;
	    if (appResources.raise == True && appResources.stay_raised == False)
		XLowerWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    if (appResources.popup == True)
		BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	}
    } else
	/* No mail */
	if (current_status == ARRIVE) {
	    current_status = NO;
	    XtResizeWidget(toplevel,  No_pixmap_width, No_pixmap_height, 1);
	    XtResizeWidget(biff,  No_pixmap_width, No_pixmap_height, 0);
	    redraw_window();
#ifdef SHAPE
	    shape_window();
#endif
	    if (appResources.raise == True && appResources.stay_raised == False)
		XLowerWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    if (appResources.popup == True)
		BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	}
    /* No arrive */

    polling_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
				 (unsigned long) appResources.polling_time, (XtTimerCallbackProc) Polling, NULL);

}
#endif /* !YOUBIN */

void 
AnimateBiff()
{

    XPoint	    points[4];
    double	    r, angle, t1, t2, t3, t4;

#ifdef SHAPE
    if(appResources.shape == True) {
	XRectangle	rect;
	rect.x = rect.y = 0;
	rect.width = Arrive_pixmap_width;
	rect.height = Arrive_pixmap_height;
	XShapeCombineRectangles(XtDisplay(toplevel), XtWindow(toplevel),
				ShapeBounding, 0, 0, &rect, 1, ShapeSet, 0);
    }
#endif

    for (r = 0, angle = 90; angle < (ROTATION + 90);
	 angle++, r += (double) (Arrive_pixmap_width) / (double) (ROTATION * 2)) {

	t1 = (angle + 40) * PI / 180.0;
	t2 = (angle + 140) * PI / 180.0;
	t3 = (angle + 220) * PI / 180.0;
	t4 = (angle + 320) * PI / 180.0;

	points[0].x = (short) (sin(t1) * r + (double) Arrive_pixmap_width / 2);
	points[0].y = (short) (cos(t1) * r + (double) Arrive_pixmap_height / 2 - 5);
	points[1].x = (short) (sin(t2) * r + (double) Arrive_pixmap_width / 2);
	points[1].y = (short) (cos(t2) * r + (double) Arrive_pixmap_height / 2 - 5);
	points[2].x = (short) (sin(t3) * r + (double) Arrive_pixmap_width / 2);
	points[2].y = (short) (cos(t3) * r + (double) Arrive_pixmap_height / 2 - 5);
	points[3].x = (short) (sin(t4) * r + (double) Arrive_pixmap_width / 2);
	points[3].y = (short) (cos(t4) * r + (double) Arrive_pixmap_height / 2 - 5);

	XFillRectangle(XtDisplay(toplevel), workPixmap, gcAnimateBack, 0, 0, Arrive_pixmap_width, Arrive_pixmap_height);
	XFillPolygon(XtDisplay(toplevel), workPixmap, gcAnimate, points, 4, Convex, CoordModeOrigin);
	XCopyArea(XtDisplay(toplevel), workPixmap, XtWindow(biff), gcAnimate, 0, 0, Arrive_pixmap_width, Arrive_pixmap_height, 0, 0);
    }
}

void 
PopupMailHeader(head)
char	       *head;
{

    Arg		    args[5];
    int		    arg_count;

    /* Create Popup Shell */

    if (appResources.popupGeometry != NULL) {
	arg_count = 0;
	XtSetArg(args[arg_count], XtNgeometry, appResources.popupGeometry);
	arg_count++;
    }
    else {
	Position	biff_x, biff_y, root_x, root_y;

	arg_count = 0;
	XtSetArg(args[arg_count], XtNwidth, &biff_x);
	arg_count++;
	XtSetArg(args[arg_count], XtNheight, &biff_y);
	arg_count++;
	XtGetValues(biff, args, arg_count);

	XtTranslateCoords(biff, biff_x, biff_y, &root_x, &root_y);

	arg_count = 0;
	XtSetArg(args[arg_count], XtNx, root_x);
	arg_count++;
	XtSetArg(args[arg_count], XtNy, root_y);
	arg_count++;
    }

    header = XtCreatePopupShell("header", transientShellWidgetClass, biff,
				args, arg_count);

    info_base = XtCreateManagedWidget("info_base", boxWidgetClass, header, args,
				      arg_count);

    if (appResources.popdown == True) {
	popdown_button = XtCreateManagedWidget("popdown_button", commandWidgetClass, info_base, NULL,
					       ZERO);
	XtAddCallback(popdown_button, XtNcallback, (XtCallbackProc) BreakPopup, (XtPointer) NULL);
    }
    arg_count = 0;
    XtSetArg(args[arg_count], XtNlabel, head);
    arg_count++;

    info = XtCreateManagedWidget("info", labelWidgetClass, info_base, args,
				 arg_count);

    XtPopup(header, XtGrabNone);

    showing_header = 1;
}

#ifndef YOUBIN
char *
GetMailHeader()
{
    char	*p;

#ifdef MHSCAN
    if (mhscan && (p = mh_scan()) != NULL)
	return p;
#endif
    return read_headers();
}
#endif /* !YOUBIN */

XtCallbackProc 
BreakPopup(w, client_data, call_data)
Widget		w;
XtPointer	client_data;
XtPointer	call_data;
{

    if(showing_header) {

	XtPopdown(header);
	XtDestroyWidget(header);
	showing_header = 0;

    }
}

#ifndef YOUBIN
XtCallbackProc 
popup_again(widget, client_data, call_data)
Widget widget;
caddr_t client_data;
caddr_t call_data;
{
    struct stat file_stat;
    char *mail_header;

    if ((stat(spool_path, &file_stat) == 0) && (file_stat.st_size != 0)) {
	/* There are mail */
	if (current_status == NO) {
	    /* the click catchs new mail */
	    current_status = ARRIVE;
	    mail_size = file_stat.st_size;
	    if (appResources.popup == True)
		mail_header = GetMailHeader();
	    XtResizeWidget(toplevel, Arrive_pixmap_width, Arrive_pixmap_height, 1);
	    XtResizeWidget(biff, Arrive_pixmap_width, Arrive_pixmap_height, 0);
	    if (appResources.raise == True)
		XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    if (appResources.animate == True)
		AnimateBiff();
	    redraw_window();
#ifdef SHAPE
	    shape_window();
#endif
	    if (appResources.popup == True)
		PopupMailHeader(mail_header);
	    XSync(XtDisplay(toplevel), 0);
	    if (appResources.bell == True)
		beep(XtDisplay(toplevel));
	    /*	XBell(XtDisplay(toplevel), 0); */
	}
	else {
	    mail_size = file_stat.st_size;
	    if (appResources.popup == True)
		mail_header = GetMailHeader();
	    if (appResources.popup == True && showing_header)
		BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	    if (appResources.raise == True)
		XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	    redraw_window();
#ifdef SHAPE
	    shape_window();
#endif
	    if (appResources.popup == True)
		PopupMailHeader(mail_header);
	    XSync(XtDisplay(toplevel), 0);
	}
    }

    XtRemoveTimeOut(polling_id);
    polling_id = XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
				 (unsigned long) appResources.polling_time, (XtTimerCallbackProc) Polling, NULL);
}
#endif /* !YOUBIN */

#ifndef YOUBIN
#define GAIN_SIZE	256
#define NONE_STRING	" (none)"

struct prefer {
    char	*name;
    char	*value;
    int		optional;
};

struct field {
    char	*name;
    char	*value;
};

static struct prefer	*prefer_table;

static char		*fbuf = NULL;
static char		*fbufp = NULL;
static int		fbuf_size = 0;
static int		fbuf_rest = 0;
static char		*hbuf = NULL;
static char		*hbufp = NULL;
static int		hbuf_size = 0;
static int		hbuf_rest = 0;

static void split_prefer(s)
char	*s;
{
    struct prefer	*pfp;
    int			nfields = 1;
    char		*p;

    for (p = s; *p; p++) {
	if (*p == ' ' || *p == '\t') {
	    while (*p == ' ' || *p == '\t')
		p++;
	    nfields++;
	}
    }
    p = XtMalloc(p - s + 1);
    prefer_table = (struct prefer *)
	XtCalloc(nfields + 1, sizeof(struct prefer));
    strcpy(p, s);
    for (pfp = prefer_table, s = p; *s; s++) {
	if (*s == ' ' || *s == '\t') {
	    if (*p == '&') {
		pfp->name = p + 1;
		pfp->optional = True;
	    }
	    else {
		pfp->name = p;
		pfp->optional = False;
	    }
	    pfp->value = NULL;
	    pfp++;
	    *s++ = '\0';
	    while (*s == ' ' || *s == '\t')
		s++;
	    p = s;
	}
    }
    if (*p == '&') {
	pfp->name = p + 1;
	pfp->optional = True;
    }
    else {
	pfp->name = p;
	pfp->optional = False;
    }
    pfp->value = NULL;
    pfp++;
    pfp->name = NULL;
}

static char *
read_headers()
{
    FILE		*fp;
    struct prefer	*pfp;

    if (hbuf != NULL) {
	XtFree(hbuf);
	hbuf = hbufp = NULL;
	hbuf_size = hbuf_rest = 0;
    }
    if ((fp = fopen(spool_path, "r")) == NULL)
	return NULL;
    while (read_header(fp)) {
	for (pfp = prefer_table; pfp->name != NULL; pfp++) {
	    if (pfp->value != NULL) {
		put_in_str(pfp->name);
		put_in(':');
#ifdef ISO_2022_JP
		mime2jis(pfp->value);
#endif
#ifdef JCONVERT
		switch (jcode) {
		  case SJIS:
		    jis2sjis(pfp->value);
		    break;
		  case EUC:
		    jis2euc(pfp->value);
		    break;
		}
#endif
		put_in_str(pfp->value);
		put_in('\n');
		XtFree(pfp->value);
		pfp->value = NULL;
	    }
	    else if (!pfp->optional) {
		put_in_str(pfp->name);
		put_in(':');
		put_in_str(NONE_STRING);
		put_in('\n');
	    }
	}
    }
    put_in('\0');
    fclose(fp);
    return hbuf;
}

static Boolean
read_header(fp)
FILE	*fp;
{
    struct prefer	*pfp;
    struct field	*fdp;
    int			c;

    for (;;) {
	if ((c = getc(fp)) == 'F' && (c = getc(fp)) == 'r' &&
	    (c = getc(fp)) == 'o' && (c = getc(fp)) == 'm' &&
	    (c = getc(fp)) == ' ') {
	    while ((c = getc(fp)) != '\n' && c != EOF)
		;
	    break;
	}
	while (c != '\n' && c != EOF)
	    c = getc(fp);
	if (c == EOF)
	    return False;
    }
    while ((fdp = read_field(fp)) != NULL) {
	for (pfp = prefer_table; pfp->name != NULL; pfp++) {
	    if (strcmp(pfp->name, fdp->name) == 0) {
		pfp->value = XtNewString(fdp->value);
		break;
	    }
	}
    }
    return True;
}

static struct field *
read_field(fp)
FILE	*fp;
{
    static struct field fd;
    int			c, offset;

    if (fbuf != NULL) {
	XtFree(fbuf);
	fbuf = fbufp = NULL;
	fbuf_rest = fbuf_size = 0;
    }

    if ((c = getc(fp)) == '\n' || c == EOF)
	return NULL;
    put_in_fbuf(c);
    while ((c = getc(fp)) != ':' && c != '\n' && c != EOF)
	put_in_fbuf(c);
    if (c != ':')
	return NULL;
    put_in_fbuf('\0');
    offset = fbufp - fbuf;
    for (;;) {
	if ((c = getc(fp)) == EOF)
	    break;
	if (c == '\n') {
	    if ((c = getc(fp)) == '\t' || c == ' ') {
		while ((c = getc(fp)) == '\t' || c == ' ')
		    ;
		if (c != EOF)
		    ungetc(c, fp);
		continue;
	    }
	    if (c != EOF)
		ungetc(c, fp);
	    break;
	}
	put_in_fbuf(c);
    }
    put_in_fbuf('\0');
    fd.name = fbuf;
    fd.value = fbuf + offset;
    return &fd;
}

static void put_in_fbuf(c)
int	c;
{
    if (fbuf_rest == 0) {
	fbuf_size += GAIN_SIZE;
	fbuf_rest += GAIN_SIZE;
	if (fbuf == NULL)
	    fbuf = fbufp = XtMalloc(fbuf_size);
	else {
	    fbuf = XtRealloc(fbuf, fbuf_size);
	    fbufp = fbuf + fbuf_size - GAIN_SIZE;
	}
    }
    *fbufp++ = c;
    fbuf_rest--;
}

static void put_in_str(s)
char	*s;
{
    for (; *s; s++)
	put_in(*s);
}

static void put_in(c)
int	c;
{
    if (hbuf_rest == 0) {
	hbuf_size += GAIN_SIZE;
	hbuf_rest += GAIN_SIZE;
	if (hbuf == NULL)
	    hbuf = hbufp = XtMalloc(hbuf_size);
	else {
	    hbuf = XtRealloc(hbuf, hbuf_size);
	    hbufp = hbuf + hbuf_size - GAIN_SIZE;
	}
    }
    *hbufp++ = c;
    hbuf_rest--;
}
#endif /* YOUBIN */

#ifdef JCONVERT
static void jis2euc(src)
char	*src;
{
    char	kanji = False, kana = False;
    char	*dst = src;

    if (src) {
	while(*src) {
	    if (*src == 27) {
		if (*(src+1) == '$' && (*(src+2) == '@' || *(src+2) == 'B')) {
		    src += 3;
		    kanji = True;
		}
		else if (*(src+1) == '(' && (*(src+2) == 'J' || *(src+2) == 'B')) {
		    src += 3;
		    kanji = False;
		    kana = False;
		}
		else if (*(src+1) == '(' && *(src+2) == 'I') {
		    src += 3;
		    kana = True;
		}
		else
		    *dst++ = *src++;
	    }
#ifdef GUESS_JIS
	    else if (*src == '$' && (*(src+1) == '@' || *(src+1) == 'B')) {
		src += 2;
		kanji = True;
	    }
	    else if (*src == '(' && (*(src+1) == 'J' || *(src+1) == 'B')) {
		src += 2;
		kanji = False;
	    }
	    else if (*src == '(' && *(src+1) == 'I') {
		src += 2;
		kana = True;
	    }
#endif
	    else if (*src == 14) {			     /* Shift out */
		kana = True;
		src++;
	    }
	    else if (*src == 15) {			     /* Shift in */
		kana = False;
		src++;
	    }
	    else if (kanji == True) {
		*dst++ = (*src++ | 128);
		*dst++ = (*src++ | 128);
	    }
	    else if ((kana == True) || (*src & 128)) {
		*dst++ = 142;
		*dst++ = (*src++ | 128);
	    }
	    else
		*dst++ = *src++;
	}
	*dst = '\0';
    }
}

static void jis2sjis(src)
char	*src;
{
    char	kanji = False, kana = False;
    char	*dst = src;

    if (src) {
	while (*src) {
	    if (*src == 27) {
		if (*(src+1) == '$' && (*(src+2) == '@' || *(src+2) == 'B')) {
		    src += 3;
		    kanji = True;
		}
		else if (*(src+1) == '(' && (*(src+2) == 'J' || *(src+2) == 'B')) {
		    src += 3;
		    kanji = False;
		}
		else if (*(src+1) == '(' && *(src+2) == 'I') {
		    src += 3;
		    kana = True;
		}
		else
		    *dst++ = *src++;
	    }
#ifdef GUESS_JIS
	    else if (*src == '$' && (*(src+1) == '@' || *(src+1) == 'B')) {
		src += 2;
		kanji = True;
	    }
	    else if (*src == '(' && (*(src+1) == 'J' || *(src+1) == 'B')) {
		src += 2;
		kanji = False;
		kana = False;
	    }
	    else if (*src == '(' && *(src+1) == 'I') {
		src += 2;
		kana = True;
	    }
#endif
	    else if (*src == 14) {			     /* Shift out */
		kana = True;
		src++;
	    }
	    else if (*src == 15) {			     /* Shift in */
		kana = False;
		src++;
	    }
	    else if (kanji == True) {
		*dst++ = (*src < 0x5f) ? (*src + 0xe1) >> 1 : (*src + 0x161) >> 1;
		if (*src++ & 1)
		    *dst++ = *src + ((*src < 0x60) ? 0x1f : 0x20);
		else
		    *dst++ = *src + 0x7e;
		src++;
	    }
	    else if (kana == True)
		*dst++ = (*src++ | 128);
	    else
		*dst++ = *src++;
	}
	*dst = '\0';
    }
}
#endif /* JCONVERT */

#ifdef ISO_2022_JP
static char *bit2byte(src, dst)
    char	*src, *dst;
{
    static char base64_tbl[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    char	ch;
    int		pat, pad_flag;

    for(pat = 0, pad_flag = 0; *src && !pad_flag; src++) {
	ch = (strchr(base64_tbl, *src) - base64_tbl) & 255;
	/* '=' (Pad) char found? */
	if (ch >= 64) {
	    pad_flag = 1;
	    break;
	}
	switch (pat) {
	case 0:
	    *dst = ch << 2;
	    pat++;
	    break;
	case 1:
	    *dst++ |= ch >> 4;
	    *dst = ch << 4;
	    pat++;
	    break;
	case 2:
	    *dst++ |= ch >> 2;
	    *dst = ch << 6;
	    pat++;
	    break;
	case 3:
	    *dst++ |= ch;
	    pat = 0;
	    break;
	}
    }
    *dst = '\0';
    return dst;
}

static char *quoted2byte(src, dst)
    char	*src, *dst;
{
    int		h, l;

    for (; *src; src++) {
	if (*src  == '_')
	    *dst++ = ' ';
	else if (*src == '=' && *(src+1) && *(src+2)) {
	    h = *++src;
	    if (h >= '0' && h <= '9')
		h -= '0';
	    else if (h >= 'A' && h <= 'F')
		h -= 'A' - 10;
	    else if (h >= 'a' && h <= 'f')
		h -= 'a' - 10;
	    else
		h = 0;
	    l = *++src;
	    if (l >= '0' && l <= '9')
		l -= '0';
	    else if (l >= 'A' && l <= 'F')
		l -= 'A' - 10;
	    else if (l >= 'a' && l <= 'f')
		l -= 'a' - 10;
	    else
		l = 0;
	    *dst++ = ((h & 0xf) << 4) | (l & 0xf);
	}
	else
	    *dst++ = *src;
    }
    *dst = '\0';
    return dst;
}

static void 
mime2jis(src)
    char	*src;
{
    char	*ch, *dst = src;
    Encoding	encode;

    if (src) {
	while (*src) {
	    if ((encode = check_charset(&src, "iso-2022-jp"))) {
		ch = src;
		while (*src && !(*src == '?' && *(src+1) == '='))
		    src++;
		if (*src) {
		    *src = '\0';
		    src += 2;
		    if (*src == ' ' && *(src+1) != '<')
			src++;
		    if (encode == BASE64)
			dst = bit2byte(ch, dst);
		    else
			dst = quoted2byte(ch, dst);
		}
		else {
		    while (*ch)
			*dst++ = *ch++;
		}
	    }
	    else
		*dst++ = *src++;
	}
	*dst = '\0';
    }
}

static Encoding check_charset(src, charset)
    char	**src, *charset;
{
    char	*s = *src, c, e;

    if (*s++ != '=' || *s++ != '?')
	return False;
    for (; *charset; charset++) {
	c = *s++;
	if (c >= 'A' && c <= 'Z')
	    c -= 'A' - 'a';
	if (c != *charset)
	    return False;
    }
    if (*s++ != '?' || !(e = *s++) || *s++ != '?')
	return False;
    if (e == 'B' || e == 'b') {
	*src = s;
	return BASE64;
    }
    else if (e == 'Q' || e == 'q') {
	*src = s;
	return QUOTED;
    }
    else
	return NOMATCH;
}
#endif /* ISO_2022_JP */

#ifdef MHSCAN
static char *mh_scan()
{
    static char *scan_command = NULL;
    FILE	*fp;
    int		c;

    if (hbuf != NULL) {
	XtFree(hbuf);
	hbuf = hbufp = NULL;
	hbuf_size = hbuf_rest = 0;
    }
    if (scanCommand != NULL && scanCommand[0] != '\0'
	&& scan_command == NULL) {
	char*	p;
	p = scan_command = (char*) malloc(sizeof(spool_path) + 10
					  + (scanCommandPath != NULL ? strlen(scanCommandPath) : 0)
					  + strlen(scanCommand));
	if (scanCommand[0] != '/'
	    && scanCommandPath != NULL && scanCommandPath[0] != '\0') {
	    strcpy(p, scanCommandPath);
	    p += strlen(p) - 1;
	    if (*p ++ != '/') {
		*p ++ = '/';
		*p = '\0';
	    }
	}
	strcat(p, scanCommand);
	strcat(p, " ");
	strcat(p, spool_path);
    }
    else {
	mhscan = False;
    }
    if (mhscan && (fp = popen(scan_command, "r")) != NULL) {
	while ((c = getc(fp)) != EOF)
	    put_in(c);
	put_in('\0');
	pclose(fp);
	if (*hbuf != '\0') {
#ifdef ISO_2022_JP
	    mime2jis(hbuf);
#endif
#ifdef JCONVERT
	    switch (jcode) {
	    case SJIS:
		jis2sjis(hbuf);
		break;
	    case EUC:
		jis2euc(hbuf);
		break;
	    }
#endif
	    return hbuf;
	}
    }
    mhscan = False;
    return NULL;
}
#endif

static void beep (display)
    Display *display; 
{
#if defined(SUN_AUDIO) || defined(BSD_AUDIO)
    int		audiofd, filefd;
    int		rn;
#ifdef SUN_AUDIO
    int		wn, len;
    unsigned char	buf[256];
    Audio_filehdr	*au_hdr;
    audio_info_t 	ais;
#elif defined(BSD_AUDIO)
    int mixerfd, mixerid, supported;
    static int bsize;
    static char *buf;
    struct stat sbuf;
    int level, level_tmp;
#endif
	
    if (appResources.mail_sndfile) {
	audiofd = open( "/dev/audio", O_WRONLY | O_NDELAY ); 
	if (audiofd < 0)
	    {
		fprintf(stderr, "%s: Problem opening /dev/audio.\n",
			"Mailbox widget");
		close(audiofd);
		return;
	    }

#ifdef SUN_AUDIO
	if( ioctl( audiofd, AUDIO_GETINFO, &ais ) )
	    {
		fprintf(stderr, "%s: Problem retrieving /dev/audio info.\n",
			"Mailbox widget");
		return;
	    }
	/* re-initialize */
	AUDIO_INITINFO(&ais);
	ais.play.gain = volume;
	if( ioctl( audiofd, AUDIO_SETINFO, &ais ) ) {
	    fprintf(stderr, "%s: Problem setting /dev/audio info.\n",
		    "Mailbox widget");
	    close(audiofd);
	    return;
	}

#elif defined(BSD_AUDIO)
	/* Open the mixer device */
	mixerfd = open ("/dev/mixer", O_RDWR, 0);
	if (mixerfd > 0)
	    {
		if (appResources.volume > MAX_SOUND_VOL)
			appResources.volume = MAX_SOUND_VOL;
		else if (appResources.volume < MIN_SOUND_VOL)
			appResources.volume = MIN_SOUND_VOL;
		mixerid = SOUND_MIXER_PCM;
		    /* storing volume level */
		ioctl(mixerfd,MIXER_READ(mixerid),&level_tmp);

		level = (appResources.volume << 8) | appResources.volume;
		ioctl(mixerfd,MIXER_WRITE(mixerid),&level);
	}
#endif

	filefd = open(appResources.mail_sndfile, O_RDONLY);
	if (filefd < 0)
	    {
		fprintf(stderr, "%s: Couldn't play file \"%s\"\n", 
			"Mailbox widget", appResources.mail_sndfile);
		close(audiofd);
		return;
	    }

#ifdef SUN_AUDIO
	/* Read in the audio header */
	rn = read(filefd, buf, sizeof(Audio_filehdr)); 

	if (rn > 0 && strncmp(buf, ".snd", 4) != 0) {
	    close(filefd);
	    close(audiofd);
	    fprintf(stderr, "%s: Invalid audio file format.\n",
		    "Mailbox widget");
	    return;
	}

	/* Strip the header */
	au_hdr = (Audio_filehdr *)buf;
	rn = au_hdr->hdr_size - sizeof(Audio_filehdr);
	for( ; rn > 0; )
	    {
		len = min(rn, sizeof(buf));
		len = read(filefd, buf, len); 
		rn -= len;	
	    }

	while(1)
	    {
		rn = read(filefd, buf, sizeof(buf));
		if (rn < 0)
		    {
			fprintf(stderr, "%s: Error reading from file \"%s\"\n", 
				"Mailbox widget", mail_sndfile);
			close(audiofd);
			close(filefd);
			return;
		    }
		if (rn == 0)
		    break;
		while(1) {

		    wn = write(audiofd, buf, rn);
		    if ( wn < 0 )
			{
			    fprintf(stderr, "%s: Error writing to /dev/audio.\n", 
				    "Mailbox widget");
			    close(audiofd);
			    close(filefd);
			    break;
			}
		    if ( wn != 0 )
			break;
		    usleep(1000);
		}
	    }
#elif defined(BSD_AUDIO)
	ioctl(audiofd, SNDCTL_DSP_RESET, 0);
	fstat(filefd, &sbuf);
	bsize = sbuf.st_size;
	buf = malloc((u_int)bsize);
	if ( (rn = read(filefd, buf, bsize)) > 0)
	    write(audiofd, buf, rn);
	ioctl(audiofd, SNDCTL_DSP_SYNC, 0);
	free(buf);
	if (mixerfd > 0)
	    {
		    /* restoring volume level */
		ioctl(mixerfd,MIXER_WRITE(mixerid),&level_tmp);
		close(mixerfd);
	    }
#endif
	close(audiofd); 
	close(filefd);
    }
    else
    XBell (display, 0);
#else  /* !SUN_AUDIO && !BSD_AUDIO */
    XBell (display, 0);
#endif /* SUN_AUDIO */
    return;
}

#ifdef YOUBIN
/*
 * Functions for YOUBIN
 */

static char	*mail_header = NULL;
static long	saved_mail_size = 0;

static void	
read_from_child(w, fid, id)
    Widget      w;
    int         *fid;
    XtInputId   *id;
{
    struct stat		file_stat;
    int			len;
    long		date;	/* unused */
    static char        	buff[HEADER_LEN];
    char		*cp, *cp2;
    long		strtol();

    if ((len = read(*fid, buff, HEADER_LEN)) == 0) {
        /* Erros occurs in child process. */
        fprintf(stderr, "%s: Child process killed: %s\n",
                prog_name, child_process);
        exit(EXIT_FAILURE);
    } else if (len == -1) {                         /* Unknown error. */
        fprintf(stderr, "%s: Can not read from child process: %s\n",
                prog_name, child_process);
        exit(EXIT_FAILURE);
    }
  
    mail_size = (int)strtol(buff, &cp, 10);

    if (mail_size > saved_mail_size) {
	/* NEW mail */
	if (appResources.popup == True || *cp == ' ') {
	    date = strtol(cp, &cp2, 10);
	    if (*cp2 == '\0' || isspace(*cp2)) {
		get_mail_header((*cp2 == '\0') ? NULL : (cp2 + 1));
	    }
	}
	
	current_status = ARRIVE;
	XtResizeWidget(toplevel, Arrive_pixmap_width, Arrive_pixmap_height, 1);
	XtResizeWidget(biff, Arrive_pixmap_width, Arrive_pixmap_height, 0);
	if (appResources.popup == True) {
	    BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	}
	if (appResources.raise == True) {
	    XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	}
	if (appResources.animate == True && mail_header != NULL) {
	    AnimateBiff();
	}
	redraw_window();
#ifdef SHAPE
	shape_window();
#endif
	if (appResources.popup == True) {
	    if (mail_header != NULL) {
		PopupMailHeader(mail_header);

		if (appResources.popup_time)
		    XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
				    (unsigned long) appResources.popup_time, (XtTimerCallbackProc) BreakPopup, NULL);
	    }
	}
	XSync(XtDisplay(toplevel), 0);
	if (appResources.bell == True) {
	    beep(XtDisplay(toplevel));
	}
    } else if (mail_size < saved_mail_size){
	/* No mail */
	current_status = NO;
	XtResizeWidget(toplevel,  No_pixmap_width, No_pixmap_height, 1);
	XtResizeWidget(biff,  No_pixmap_width, No_pixmap_height, 0);

	redraw_window();
#ifdef SHAPE
	shape_window();
#endif
	if (appResources.popup == True) {
	    BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	}
	if (appResources.raise == True && appResources.stay_raised == False) {
	    XLowerWindow(XtDisplay(toplevel), XtWindow(toplevel));
	}
	free(mail_header);
	mail_header = NULL;
    }
    saved_mail_size = mail_size;
}

#define STRLEN(x)	((x) == NULL ? -1 : strlen(x))

static void
get_mail_header(buf)
    char *buf;
{
    char	*from = NULL;
    char	*subject = NULL;
    char	*cp;
    int		header_count = 0;
    char	tmp[HEADER_LEN];
    char	*strtok();
    char	*new_header;

    if (buf == NULL)	return;
    strcpy(tmp, buf);
    cp = strtok(tmp, "\n");
    while(cp != NULL) {
	if (strncmp("From:", cp, strlen("From:")) == 0) {
	    from = cp;
	    header_count++;
	} else if (strncmp("Subject:", cp, strlen("Subject:")) == 0) {
	    subject = cp;
	    header_count++;
	}
	if (header_count > 1 || *cp == '\0')	break;
	cp = strtok(NULL, "\n");
    }
    if (appResources.preserve == False) {
	free(mail_header);
	mail_header = NULL;
    }
    if (header_count == 0)	return;
#ifdef ISO_2022_JP
    mime2jis(from);
    mime2jis(subject);
#endif
#ifdef JCONVERT
    switch(jcode) {
    case SJIS:
	jis2sjis(from);
	jis2sjis(subject);
	break;
    case EUC:
	jis2euc(from);
	jis2euc(subject);
	break;
    }
#endif
    new_header = (char *)malloc(STRLEN(mail_header)
				+ STRLEN(from) + STRLEN(subject) + 3);
    *new_header = '\0';
    if (mail_header != NULL) {
	strcat(new_header, mail_header);
	strcat(new_header, "\n");
    }
    if (from != NULL) {
	strcat(new_header, from);
    }
    if (subject != NULL) {
	strcat(new_header, "\n");
	strcat(new_header, subject);
    }
    free(mail_header);
    mail_header = new_header;
}

XtCallbackProc
popup_again(widget, client_data, call_data)
     Widget widget;
     caddr_t client_data;
     caddr_t call_data;
{
    struct stat file_stat;

    if (current_status == ARRIVE) {
	redraw_window();

	if (appResources.popup == True) {
	    BreakPopup(toplevel, (XtPointer) NULL, (XtPointer) NULL);
	    if (mail_header != NULL) {
		PopupMailHeader(mail_header);
	    }
	    if (appResources.popup_time)
		XtAppAddTimeOut(XtWidgetToApplicationContext(toplevel),
				(unsigned long) appResources.popup_time, (XtTimerCallbackProc) BreakPopup, NULL);
	}
	if (appResources.raise == True) {
	    XRaiseWindow(XtDisplay(toplevel), XtWindow(toplevel));
	}
#ifdef SHAPE
	shape_window();
#endif
	XSync(XtDisplay(toplevel), 0);
    }
}

static int
exit_hundler(disp)
    Display *disp;		/* unused */
{
    kill(0, SIGTERM);		/* kill all the children */
}
#endif /* YOUBIN */
