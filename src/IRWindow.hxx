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
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>

#include "MTD.hxx"
#include "globals.hxx"

class IRWidget:public Fl_Widget
{
private:
	short outbuffer[1024];
	MTD<float,short>* my_mtd;
public:
	IRWidget(int X,int Y);
	void draw(void);
	void run(void);
};

class IRWindow:public Fl_Window
{
public:
	IRWidget* w;
	IRWindow():Fl_Window(512+10,512+10,"tapiir Impulse Response")
	{
		w=new IRWidget(5,5);
	}
	void run(void) {
		w->run();
	}
};
