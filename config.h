/* config.h */

/*
 *	For X11R3
 */
#undef X11R3

/*
 *	For X11R6
 */
#undef X11R6

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
 *	For MH scan users
 */

#undef MHSCAN

/*
 *	For Shape Extension
 */

#undef SHAPE

/*
 *	Check Environment Variable MAIL for check file
 */
#undef GETENV_MAIL

/*
 *	Set Local Spool pathname
 */
#undef SPOOLPATH	"/usr/mail"

/*
 *	For X pixmap
 */
#undef XPM
#undef DepXpmLib	/usr/local/lib/libXpm.a

/*
 *	For using I18N Xaw
 */
#undef XI18N
/* If you use X11R6, these settings below are not necessary */
#undef DepXawLib	/usr/local/lib/I18N/libXaw.a
#undef DepXwcharLib	/usr/local/lib/I18N/libXwchar.a
#undef Xi18nLibDir	-L/usr/local/lib/I18N \
			-L/usr/local/lib
#undef Xi18nIncDir	-I/usr/local/X11/include

/*
 *	Convert JIS to Japanese EUC
 */
#undef JCONVERT
#undef GUESS_JIS

/*
 *	Process MIME header, charset=ISO-2022-JP, base 64 encoding
 */
#undef ISO_2022_JP

/*
 *	For SUN Audio
 */
#undef SUN_AUDIO
#undef AudioIncDir /usr/demo/SOUND

/* 
 * For YOUBIN Service
*/
#undef YOUBIN
#undef YoubinBinDir /usr/local/bin
#undef YOUBIN_DEFAULT_SERVER_HOST "server"
