/*
** TAPIIR, A software multitap delay with realtime I/O
**
** This software is copyright (c) 2000-2002 by Maarten de Boer
**
** You are free to distribute this software under the terms of
** the GNU General Public License.
** On Debian systems, the complete text of the GNU General Public
** License can be found in /usr/share/common-licenses/GPL file.
**
*/
#ifndef __LAYOUT__
#define __LAYOUT__

#ifdef _LAYOUT_STATIC_

#include "LayoutDefines.H"

#define DY DY_
#define H H_
#define HEADERH HEADERH_

#define SPACE0 SPACE0_
#define SPACE1 SPACE1_
#define SPACE2 SPACE2_

#define HEADERLABELSIZE HEADERLABELSIZE_

#define MIXERSLIDEROFFSET MIXERSLIDEROFFSET_
#define MIXERSLIDERW	MIXERSLIDERW_
#define PLUSMINUSBUTTONW PLUSMINUSBUTTONW_
#define TAPSLIDERW TAPSLIDERW_
#define TAPINPUTW TAPINPUTW_

#define BUTTONFONTSIZE BUTTONFONTSIZE_
#define SLIDERLABELFONTSIZE SLIDERLABELFONTSIZE_
#define MENUBUTTONWIDTH MENUBUTTONWIDTH_

#define BPMLABELWIDTH_ BPMLABELWIDTH
#define BPMINPUTWIDTH_ BPMINPUTWIDTH

#else

extern int DY;
extern int H;
extern int HEADERH;
extern int SPACE0;
extern int SPACE1;
extern int SPACE2;
extern int HEADERLABELSIZE;
extern int MIXERSLIDEROFFSET;
extern int MIXERSLIDERW;
extern int PLUSMINUSBUTTONW;
extern int TAPSLIDERW;
extern int TAPINPUTW;
extern int BUTTONFONTSIZE;
extern int SLIDERLABELFONTSIZE;
extern int MENUBUTTONWIDTH;
extern int BPMLABELWIDTH;
extern int BPMINPUTWIDTH;

void layout_cb(class Fl_Menu_Button* b);

#endif

#define YOFFSET (HEADERH+SPACE2)

#define MIXERW	(\
	MIXERSLIDEROFFSET+MIXERSLIDERW+SPACE0+\
	PLUSMINUSBUTTONW+\
	MIXERSLIDEROFFSET+MIXERSLIDERW+SPACE0+\
	PLUSMINUSBUTTONW+\
	SPACE1)
#define BDX \
	( \
		MIXERW+SPACE1+\
		TAPW+SPACE2 \
	)
#define MIXERH (YOFFSET+5*DY+SPACE1)
#define BDY (MIXERH+SPACE2)
#define TAPW (XOFFSET+TAPSLIDERW*8+3+2+SPACE1)
#define TAPINPUTOFFSET (TAPW-TAPINPUTW-SPACE1-1)
#define XOFFSET SPACE2 
#define TOTALW (XOFFSET+BDX+BDX)
#define TOTALH (YOFFSET+BDY*4)

#endif
