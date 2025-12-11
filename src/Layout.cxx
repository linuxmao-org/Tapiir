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
#ifndef _LAYOUT_STATIC_

#include <FL/Fl_Menu_Button.H>

#include "Layout.hxx"
#include "LayoutDefines.hxx"
#include "GUI.hxx"

#define __DO_DECL__ int
#include "LayoutSet.cxx"

extern void layout_small(void);
extern void layout_big(void);

void layout(int t)
{
	if (t==0) {
		layout_small();
	}else{
		layout_big();	
	}
}

void layout_cb(Fl_Menu_Button* b)
{
	layout(b->value());
	GUI* gui=new GUI(TOTALW,TOTALH);
	gui->show();
	delete b->window();
}

#endif
