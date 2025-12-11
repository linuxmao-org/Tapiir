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
#ifndef __TapSliderGroup__
#define __TapSliderGroup__

#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/Fl.H>

#include <stdio.h>
#include <stdlib.h>

/* the following code really needs a clean-up */
class TapSliderGroup:public Fl_Group
{
private:
	static const int pointPos = 3;

	Fl_Slider* sliders[8];
	Fl_Input* input;
	Fl_Input* samples;
	int val;
	char normalized[9];
public:		
	void value(int v)
	{
		val = v;
		UpdateSamples();
		UpdateFromSamples();
	}
	int value(void)
	{
		return val;
	}

	void Update(bool recalc = false)
	{
		SetSliderRange();
		if (recalc) {
			CalcSamples();
			UpdateSamples();
		}else{
			UpdateFromSamples();
		}
	}

	void UpdateSliders(void)
	{
		for (int i=0;i<8;i++)
		{
			sliders[i]->value(normalized[i]-'0');
		}
	}
	
	void UpdateSamples(void)
	{
		char tmp[20];
		sprintf(tmp,"%10d",val);
		samples->value(tmp);
	}
	
	void CalcSamples(void)
	{
		unsigned int s = 0;
		int division = division1;
		unsigned int t = 0;
		int i;
		for (i=0;i<8;i++)
		{
			if (i==pointPos && usebpm) division = division2;
			t *= division;
			t+=normalized[i]-'0';
		}
		if (usebpm)
			s = int((double(t)*48000.*60./double(bpm))/256.);
		else
			s = int((double(t)*48000./100000.));
			
		if (s==0) {
			s = 1;
		}
		val = s;
	}
	
	void UpdateInput(void)
	{
		char tmp[20];
		int j = 0;
		for (int i=0;i<8;i++)
		{
			tmp[j++] = normalized[i];
			tmp[j++] = ' ';
		}
		tmp[pointPos*2-1]='.';
		tmp[15]=0;
		input->value(tmp);
	}
	void UpdateFromInput(const char* str)
	{
		int i=0,j=0;
		while (str[j])
		{
			if (str[j]>='0' && str[j]<='9') {
				normalized[i++] = str[j];
			}
			if (str[j]=='.')
			{
				int k=pointPos;
				
				while (i) {
					k--; i--;
					normalized[k]=normalized[i];
				}
				while (k)
				{
					k--;
					normalized[k]='0';
				}
				i = pointPos;
			}
			j++;
		}
		while (i<8)
		{
			normalized[i++]='0';
		}
		normalized[i]=0;
		UpdateInput();
		UpdateSliders();
		CalcSamples();
		UpdateSamples();
	}
	void UpdateFromSliders(Fl_Slider* w)
	{
		int i;

		int reset = Fl::event_state(FL_CTRL);

		for (i=0;i<8;i++)
		{
			if (reset && w!=sliders[i]) {
				sliders[i]->value(0);
				normalized[i] = '0';
			}else{
				normalized[i] = '0'+int(sliders[i]->value());
			}
		}
		UpdateInput();
		CalcSamples();
		UpdateSamples();
	}
	void UpdateFromSamples(const char* str)
	{
		val  = atoi(str);
		UpdateFromSamples();
	}
	void UpdateFromSamples()
	{
		int t;
		int dec = 0;
		int division;
		char* ptr;
		unsigned int s = val;
		

		if (usebpm)
			t = int((double(s)*256.*double(bpm))/(48000.*60.));
		else
			t = int((double(s)*100000./48000.));
		
		ptr=normalized+8;
		*ptr = 0;
		if (usebpm)
			division = division2;
		else
			division = division1;
			
		while (dec<8-pointPos)
		{	
			ptr--;
			*ptr = '0'+t%division;
			t = t/division;
			dec++;
		}
		division = division1;
		while (dec<8)
		{	
			ptr--;
			*ptr = '0'+t%division;
			t = t/division;
			dec++;
		}
		UpdateSliders();
		UpdateInput();
	}
	
	static void SliderCB(Fl_Slider* o)
	{
		TapSliderGroup* g = (TapSliderGroup*) (o->parent());
		g->UpdateFromSliders(o);
		g->do_callback();
	}
	static void SamplesCB(Fl_Input* o)
	{
		TapSliderGroup* g = (TapSliderGroup*) (o->parent());
		g->UpdateFromSamples(o->value());
		g->do_callback();
	}
	static void InputCB(Fl_Input* o)
	{
		TapSliderGroup* g = (TapSliderGroup*) (o->parent());
		g->UpdateFromInput(o->value());
		g->do_callback();
	}
	
	void SetSliderRange(void)
	{
		int i;
		int division = division1;
		for (i=0;i<8;i++)
		{
			Fl_Slider* s = sliders[i];
			if (i==pointPos && usebpm) division = division2;
			s->range(0,division-1);
			s->step(1);
		}	
	}
	TapSliderGroup(int x_,int y_,int w_,int h_):Fl_Group(x_,y_,w_,h_)
	{
		int cx = x_;
		int cy = y_;
		int cw = TAPSLIDERW;
		int ch = h_-H-H-SPACE1-SPACE1-SPACE0-SPACE0;
		int i;
		for (i=0;i<8;i++)
		{
			Fl_Slider* s = new Fl_Slider(cx,cy,cw,ch);			
			s->slider_size(0.2);
			s->box(FL_THIN_DOWN_BOX);
			s->callback((Fl_Callback*)SliderCB);
			sliders[i] = s;
			cx += cw;
		}
		SetSliderRange();
		cx = x_;
		cy += ch+SPACE0;
		ch = H;
		cw = TAPSLIDERW*8+3;
		input = new Fl_Input(cx,cy,cw,ch);
		input->textfont(FL_COURIER);
		input->box(FL_THIN_DOWN_BOX);
		input->textsize(SLIDERLABELFONTSIZE);
		input->callback((Fl_Callback*)InputCB);
		input->when(FL_WHEN_ENTER_KEY);
		cy += ch+SPACE0;
		samples = new Fl_Input(cx,cy,cw,ch);
		samples->textfont(FL_COURIER);
		samples->box(FL_THIN_DOWN_BOX);
		samples->textsize(SLIDERLABELFONTSIZE);
		samples->callback((Fl_Callback*)SamplesCB);
		samples->when(FL_WHEN_ENTER_KEY);
		end();
	}
};

#endif
