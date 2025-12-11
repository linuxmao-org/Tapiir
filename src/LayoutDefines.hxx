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
#ifndef __LAYOUTDEFINES__
#define __LAYOUTDEFINES__

#ifdef _LAYOUT_BIG_

#define DY_ 24
#define H_ 22
#define HEADERH_ 22

#define SPACE0_ 1
#define SPACE1_ 3
#define SPACE2_ 5

#define HEADERLABELSIZE_ 14

#define MIXERSLIDEROFFSET_ (40)
#define MIXERSLIDERW_	(80+8+2)
#define PLUSMINUSBUTTONW_ (22)
#define TAPSLIDERW_ (14)
#define TAPINPUTW_ (80)

#define BUTTONFONTSIZE_ 14
#define SLIDERLABELFONTSIZE_ 12

#define MENUBUTTONWIDTH_ 96

#define BPMLABELWIDTH_ 38
#define BPMINPUTWIDTH_ 38

#else

#define DY_ 17
#define H_ 16
#define HEADERH_ 22

#define SPACE0_ 1
#define SPACE1_ 3
#define SPACE2_ 5

#define HEADERLABELSIZE_ 12

#define MIXERSLIDEROFFSET_ (35)
#define MIXERSLIDERW_	(63+8+2)
#define PLUSMINUSBUTTONW_ (16)
#define TAPSLIDERW_ (12)
#define TAPINPUTW_ (50)

#define BUTTONFONTSIZE_ 12
#define SLIDERLABELFONTSIZE_ 10

#define MENUBUTTONWIDTH_ 80

#define BPMLABELWIDTH_ 30
#define BPMINPUTWIDTH_ 30

#endif

#endif
