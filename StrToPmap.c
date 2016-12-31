/* Change for pixmap with None color by tatuya on Oct 25, 1994 */

/* Change for xpbiff by Yoshikazu Yamamoto(y-yamamt@ics.es.osaka-u.ac.jp) at Oct 19, 1993 */

/* * Last edited: Aug 29 14:37 1991 (mallet) */
#ifndef lint
/* static char rcsid[] = "$Id: StrToPmap.c,v 1.1 1991/09/06 07:48:33 mallet Exp $"; */
#endif /* lint */

/*
 * Copyright 1991 Lionel Mallet
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Lionel MALLET not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  Lionel MALLET makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Lionel MALLET DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL Lionel MALLET BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Lionel Mallet - SIMULOG
 */


#include	<X11/Intrinsic.h>
#include	<X11/StringDefs.h>
#include	<X11/Xmu/Converters.h>
#include	<X11/Xmu/Drawing.h>

typedef Pixmap	Pixmap2[2];

/*
 * CvtStringToPixmap:
 *
 * creates a depth-1 Pixmap suitable for window manager icons.
 * "string" represents a pixmap(1) filename which may be absolute,
 * or relative to the global resource pixmapFilePath, class
 * PixmapFilePath.  If the resource is not defined, the default
 * value is the build symbol BITMAPDIR.
 *
 * To use, include the following in your ClassInitialize procedure:

static XtConvertArgRec screenConvertArg[] = {
    {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen), sizeof(Screen *)}
};

    XtAddConverter("String", "Pixmap2", CvtStringToPixmap,
		   screenConvertArg, XtNumber(screenConvertArg));
 *
 */

#define	done(address, type) \
	{ (*toVal).size = sizeof(type); (*toVal).addr = (caddr_t) address; }


/*ARGSUSED*/
void CvtStringToPixmap(args, num_args, fromVal, toVal)
    XrmValuePtr args;
    Cardinal    *num_args;
    XrmValuePtr	fromVal;
    XrmValuePtr	toVal;
{
    static Pixmap2 pixmap;		/* static for cvt magic */
    char *name = (char *)fromVal->addr;

    if (*num_args != 1)
     XtErrorMsg("wrongParameters","cvtStringToPixmap","XtToolkitError",
             "String to pixmap conversion needs screen argument",
              (String *)NULL, (Cardinal *)NULL);

    if (strcmp(name, "None") == 0) {
	pixmap[0] = pixmap[1] = None;
	done(pixmap, Pixmap2);
	return;
    }

    if (strcmp(name, "ParentRelative") == 0) {
	pixmap[0] = ParentRelative;
	pixmap[1] = None;
	done(pixmap, Pixmap2);
	return;
    }

    pixmap[0] = locate_pixmap_file (*((Screen **) args[0].addr), name,
			       NULL, 0, NULL, NULL, &pixmap[1]);

    if (pixmap[0] != None) {
	done (pixmap, Pixmap2);
    } else {
	XtStringConversionWarning (name, "Pixmap2");
	return;
    }
}
