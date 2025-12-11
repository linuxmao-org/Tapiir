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
#include "Tap.hxx"

#include "Layout.hxx"

#include "MTDdecl.hxx"
#include "globals.hxx"

#include "TapSliderGroup.hxx"

Tap::Tap(int xi,int yi,int wi,int hi,char* li,int id)
:Fl_Group(xi, yi, wi, hi, li)
{
	mID = id;

	labeltype(FL_NO_LABEL);

	box(FL_ENGRAVED_FRAME);

	{
		Fl_Box* o = new Fl_Box(x()+2, y()+2, w()-4, HEADERH, label() );
		o->box(FL_FLAT_BOX);
		o->color(33);
		o->labelsize(HEADERLABELSIZE);
		o->labelcolor(7);
	}
	
	mSliderGroup = 
		new TapSliderGroup(x()+XOFFSET,y()+YOFFSET,w()-XOFFSET,h()-YOFFSET);

	mSliderGroup->callback((Fl_Callback*) SliderCB);
		
	end();
}

void Tap::SliderCB(TapSliderGroup* s)
{
	Tap* t = (Tap*) s->parent();
	
	mtd->delaylen[t->mID] = s->value();
}

void Tap::SetValues(void)
{
	mSliderGroup->value(mtd->delaylen[mID]);
}

void Tap::Update(bool recalc)
{
	mSliderGroup->Update(recalc);
	mtd->delaylen[mID] = mSliderGroup->value();
}
