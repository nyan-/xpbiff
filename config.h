/* config.h */

/*
 *	For X11R3
 */
#undef X11R3

/*
 *	For X11R6
 */
#define X11R6

/*
 *	For System V
 */
#undef USG

/*
 *	For Solaris
 */
#undef SOLARIS
#undef UcbLibDir	-L/usr/ucblib

/*
 *	For slow work station
 */
#undef SLOW_MACHINE

/*
 *	No cuserid() in library
 */

#undef NO_CUSERID

/*
 *      Your system has getpwuid() in library
 */

#define HAVE_GETPWUID

/*
 *	For MH scan users
 */

#undef MHSCAN

/*
 *	For Shape Extension
 */

#define SHAPE

/*
 *	Check Environment Variable MAIL for check file
 */
#define GETENV_MAIL

/*
 *	Set Local Spool pathname
 */
#undef SPOOLPATH	"/usr/mail"

/*
 *	For X pixmap
 */
#define XPM

#ifdef XPM
#ifdef __FreeBSD__
#undef DepXpmLib
#else
#define DepXpmLib	/usr/local/lib/libXpm.a
#endif /* __FreeBSD__ */
#endif /* XPM */

/*
 *	For using I18N Xaw
 */
#ifndef XI18N
#ifdef __FreeBSD__
#define XI18N
#endif
#endif

#ifdef XI18N
#undef X11R5_Xsi	/* define if X11R5 Xsi */
#endif

#ifdef X11R5_Xsi	/* When X11R5 Xsi, define followings if you want */
#undef DepXawLib	/usr/local/lib/I18N/libXaw.a
#undef DepXwcharLib	/usr/local/lib/I18N/libXwchar.a
#undef Xi18nLibDir	-L/usr/local/lib/I18N \
			-L/usr/local/lib
#undef Xi18nIncDir	-I/usr/local/X11/include
#endif

/*
 *	Convert JIS to Japanese EUC
 */
#define JCONVERT
#undef GUESS_JIS

/*
 *	Process MIME header, charset=ISO-2022-JP, base 64 encoding
 */
#define ISO_2022_JP

/*
 *	For SUN Audio
 */
#undef SUN_AUDIO
#undef AudioIncDir /usr/demo/SOUND

/*
 * For BSD Audio (FreeBSD)
 */
#ifndef __alpha
#define BSD_AUDIO
#endif

/* 
 * For YOUBIN Service
*/
#undef YOUBIN

#ifdef YOUBIN
#define YoubinBinDir %YOUBINBINDIR%
#define YOUBIN_DEFAULT_SERVER_HOST "server"
#endif /* YOUBIN */
