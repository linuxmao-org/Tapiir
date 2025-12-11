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
#include "Mixer.hxx"

#include "Layout.hxx"

#include "MTDdecl.hxx"
#include "globals.hxx"

#include <FL/Fl_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <cmath>

class MixerSlider:public Fl_Group
{
friend class Mixer;
private:
	int mID;
	
	Fl_Slider* mSlider;
	Fl_Button* mButton;
	
public:
	MixerSlider(int xi,int yi,char* li,int id);
	void value(float v);
	float value(void);
private:
	static void SliderCB(Fl_Slider* o);
	static void SignCB(Fl_Button* o);
};

void MixerSlider::value(float v)
{
	mSlider->value(fabs(v));
	mButton->value(v<0);
	mButton->label(v<0?"-":"+");
}

float MixerSlider::value(void)
{
	return mButton->value() ? -mSlider->value() : mSlider->value();
}

MixerSlider::MixerSlider(int xi,int yi,char* li,int id)
:Fl_Group(xi,yi,MIXERSLIDERW+SPACE0+PLUSMINUSBUTTONW,H,li)
{
	mID = id;
	
	labeltype(FL_NO_LABEL);
	
	{	
		Fl_Slider* o = new Fl_Slider(x(), y(), MIXERSLIDERW, h(), label());
		o->type(1);
		o->box(FL_THIN_DOWN_BOX);
		o->labelsize(SLIDERLABELFONTSIZE);
		o->align(FL_ALIGN_LEFT);
		o->range(0,1.);
		o->callback((Fl_Callback*)SliderCB);
		mSlider = o;
	}

	{
		Fl_Button* o=new Fl_Button(
			x()+MIXERSLIDERW+SPACE0,y(),PLUSMINUSBUTTONW,h(),"+");
		o->type(FL_TOGGLE_BUTTON);
		o->box(FL_THIN_DOWN_BOX);		
		o->labelsize(BUTTONFONTSIZE);
		o->labelfont(FL_COURIER);
		o->callback((Fl_Callback*)SignCB);
		mButton = o;
	}

	end();
}

void MixerSlider::SliderCB(Fl_Slider* o)
{
	MixerSlider* ms = (MixerSlider*) o->parent();
	ms->do_callback();
}

void MixerSlider::SignCB(Fl_Button* o)
{
	MixerSlider* ms = (MixerSlider*) o->parent();
	
	o->label(o->value()?"-":"+");
	o->redraw();

	ms->do_callback();
}

Mixer::Mixer(int xi,int yi,int wi,int hi,char* li,int id)
:Fl_Group(xi,yi,wi,hi,li)
{
	mID=id;

	labeltype(FL_NO_LABEL);

	box(FL_ENGRAVED_FRAME);
	
	{
		Fl_Box* o = new Fl_Box(x()+2, y()+2, w()-4, HEADERH, label() );
		o->box(FL_FLAT_BOX);
		o->color(33);
		o->labelsize(HEADERLABELSIZE);
		o->labelcolor(7);
	}

	char* sliderLabel[5][2]=
	{	
		{"left","right"},
		{"tap 1","tap 4"},
		{"tap 2","tap 5"},
		{"tap 3","tap 6"},
		{"gain",0}
	};

	int sliderID[5][2]=
	{ { mtd->ChannelID(0) , mtd->ChannelID(1) },
	  { 0 , 3 },
		{ 1 , 4 },
		{ 2 , 5 },
		{ mtd->OverallID() , -1 }
	};

	int j;
	for (j=0;j<5;j++) {
		for (int i=0;i<2;i++) {
			if (sliderID[j][i]!=-1)
			{
				mGainSliders[sliderID[j][i]] = new MixerSlider(
					x()+MIXERSLIDEROFFSET+
						i*(MIXERSLIDERW+PLUSMINUSBUTTONW+MIXERSLIDEROFFSET),
					y()+YOFFSET+j*DY,
					sliderLabel[j][i], 
					sliderID[j][i]
				);
				
				mGainSliders[sliderID[j][i]]->callback( (Fl_Callback*) SliderCB );
			}
		}
	}

	end();

	SetValues();
}

void Mixer::SetValues(void)
{
	for (int id=0;id<=8;id++) {
		mGainSliders[id]->value(mtd->GetGain(mID,id));
	}
}

void Mixer::SliderCB(MixerSlider* s)
{
	Mixer* m = (Mixer*) s->parent();
	mtd->SetGain(m->mID,s->mID,s->value());
}
