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
#ifndef __Mixer__
#define __Mixer__

#include <FL/Fl_Group.H>

class MixerSlider;

class Mixer:public Fl_Group
{
private:
	int mID;
	MixerSlider* mGainSliders[9];
public:
	Mixer(int,int,int,int,char*,int);
	void SetValues(void);
private:
	static void SliderCB(MixerSlider* s);
};

#endif
