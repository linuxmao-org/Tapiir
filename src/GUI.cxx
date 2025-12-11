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
#include <string.h>
#include <stdio.h>

#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Button.H>

#include <FL/fl_ask.H>
#include <FL/fl_file_chooser.H>

#include "tapiir_image.cxx" 

#include "GUI.hxx"
#include "Tap.hxx"
#include "Mixer.hxx"
#include "IRWindow.hxx"

#include "FileIO.hxx"

#include "Layout.hxx"

#include "globals.hxx"
#include "MTDdecl.hxx"

Fl_Menu_Item layout_menu[] = {
 {"small", 0,  0, 0, 0, 0, 0, BUTTONFONTSIZE, 0},
 {"big", 0,  0, 0, 0, 0, 0, BUTTONFONTSIZE, 0},
 {0}
};

int division1 = 10;
int division2 = 4;
int bpm = 120;
bool usebpm = 0;

GUI::GUI(int w_,int h_)
:Fl_Window(w_,h_,"tapiir")
{
	mIRWindow=0;
	mFilename=0;

		char* tap_label[4][2]=
			{	
				{"Tap 1","Tap 4"},
				{"Tap 2","Tap 5"},
				{"Tap 3","Tap 6"},
			};


	char* mixer_label[4][2]=
	{	
		{"Tap 1 Input Mixer","Tap 4 Input Mixer"},
		{"Tap 2 Input Mixer","Tap 5 Input Mixer"},
		{"Tap 3 Input Mixer","Tap 6 Input Mixer"},
		{"Left Output Mixer","Right Output Mixer"}
	};
	int mixer_id[4][2]=
	{ 
	  { 0 , 3 },
		{ 1 , 4 },
		{ 2 , 5 },
		{ mtd->ChannelID(0) , mtd->ChannelID(1) }
	};
	int tap_id[3][2]=
	{
		{0,3},{1,4},{2,5}
	};

	for (int j=0;j<4;j++) {
		for (int i=0;i<2;i++) {
			mMixers[mixer_id[j][i]]=new Mixer(
				XOFFSET+i*BDX,YOFFSET+j*BDY,
				MIXERW,
				MIXERH,
				mixer_label[j][i],
				mixer_id[j][i]);
		}
	}
	
	for (int j=0;j<3;j++) {
		for (int i=0;i<2;i++) {
			Tap* t =new Tap(
				XOFFSET+i*BDX+
					MIXERW
					+SPACE1,
				YOFFSET+j*BDY,
				TAPW,MIXERH,
				tap_label[j][i],
				tap_id[j][i]);

			mTaps[tap_id[j][i]] = t;
		}
	}

	{
		Fl_Box* o = new Fl_Box(XOFFSET+1*BDX+
                                        MIXERW
                                        +SPACE1,
					YOFFSET+3*BDY,
					TAPW,MIXERH-30);
		pixmap_tapiir.label(o);
	}
	{
		Fl_Box* o = new Fl_Box(XOFFSET+1*BDX+
                                        MIXERW
                                        +SPACE1,
                                	YOFFSET+3*BDY+MIXERH-30,
					TAPW,15,"Maarten de Boer");
		o->labelsize(12);
	}
	{
		Fl_Box* o = new Fl_Box(XOFFSET+1*BDX+
                                        MIXERW
                                        +SPACE1,
                                	YOFFSET+3*BDY+MIXERH-15,
					TAPW,15,"maarten@@resorama.com");
		o->labelsize(10);
		o->labelcolor(FL_BLUE);
	}

	int bx=5;
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Load");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) load_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Reload");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) reload_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Save");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) save_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Save as..");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) saveas_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Show IR");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) show_ir_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	{
		Fl_Button* o = new Fl_Button(bx,5,MENUBUTTONWIDTH,20,"Diagram");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) show_diagram_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	#ifndef _LAYOUT_STATIC_
	{
		Fl_Menu_Button* o = new Fl_Menu_Button(bx,5,MENUBUTTONWIDTH,20,"Layout");
		o->box(FL_THIN_UP_BOX); o->labelsize(BUTTONFONTSIZE);
		o->menu(layout_menu);
		o->callback((Fl_Callback*) layout_cb);
		bx+=MENUBUTTONWIDTH+5;
	}
	#endif
	{
		Fl_Button* o = new Fl_Button(bx,10,10,10);
		o->callback((Fl_Callback*)usebpm_cb);
		o->box(FL_THIN_DOWN_BOX);
		o->type(FL_TOGGLE_BUTTON);
		o->color(FL_WHITE,FL_GREEN);
		o->value(usebpm);
		bx+=10+5;
	}
	{
		bx += BPMLABELWIDTH;
		Fl_Input* o = new Fl_Input(bx,5,BPMINPUTWIDTH,20,"BPM");
		char tmp[20];
		sprintf(tmp,"%d",bpm);
		o->value(tmp);
		o->textfont(FL_COURIER);
		o->textsize(BUTTONFONTSIZE);
		o->box(FL_THIN_DOWN_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) bpm_cb);
		o->when(FL_WHEN_ENTER_KEY);
		bx+=BPMINPUTWIDTH+5;
		mBPMInput = o;
	}
	{
		bx += BPMLABELWIDTH;
		Fl_Input* o = new Fl_Input(bx,5,BPMINPUTWIDTH,20,"DIV");
		char tmp[20];
		sprintf(tmp,"%d",division2);
		o->value(tmp);
		o->textfont(FL_COURIER);
		o->textsize(BUTTONFONTSIZE);
		o->box(FL_THIN_DOWN_BOX); o->labelsize(BUTTONFONTSIZE);
		o->callback((Fl_Callback*) div_cb);
		o->when(FL_WHEN_ENTER_KEY);
		bx+=BPMINPUTWIDTH+5;
		mDIVInput = o;
	}
	UseBPM(usebpm);
	SetValues();
}

void GUI::SetValues(void)
{
	int i;
	for (i=0;i<8;i++) {
		mMixers[i]->SetValues();
	}
	for (i=0;i<6;i++) {
		mTaps[i]->SetValues();
	}
}

void GUI::SetFilename(char* filename) {
	mFilename=filename;
	char tmp[256];
	strncpy(tmp,"MTD - ",7);
	strncat(tmp,filename,248);
	label(tmp);
}
	
void GUI::Load(char* filename)
{
	if (filename) {
		SetFilename(filename);
		if (load_mtd(filename)!=0) {
			fl_alert("Not a valid MTD file");
		}else{
			SetValues();
		}
	}
}

void GUI::Save(char* filename)
{
	if (filename) {
		SetFilename(filename);
		save_mtd(mFilename);
	}
}

void GUI::SaveAs(void) {
	char* tmp=mFilename;
	mFilename=0;
	Save();
	if (mFilename==0) {
		mFilename=tmp;
	}
}

void GUI::Reload(void)
{
	Load(mFilename);
}

void GUI::Load(void)
{
	char* filename = fl_file_chooser("Load","*.mtd","");
	Load(filename);
}

void GUI::Save(void)
{
	if (mFilename==0){
		char* filename = fl_file_chooser("Save","*.mtd","untitled.mtd");
		Save(filename);
	}else{
		Save(mFilename);
	}
}

void GUI::ShowIR(void) {
	if (mIRWindow==0) {
		mIRWindow=new IRWindow();
	}
	mIRWindow->show();			
	mIRWindow->run();
	mIRWindow->redraw();	
}

void GUI::SetBPM(int val)
{
	int i;
	bpm = val;
	for (i=0;i<6;i++) {
		mTaps[i]->Update(true);
	}
}

void GUI::SetDivision(int val)
{
	int i;
	bpm = val;
	division2 = val;
	for (i=0;i<6;i++) {
		mTaps[i]->Update(true);
	}
}

void GUI::UseBPM(int val)
{
	if (val) {
		mBPMInput->activate();
		mDIVInput->activate();
	} else {
		mBPMInput->deactivate();
		mDIVInput->deactivate();
	}
	if (usebpm!=val)
	{
		int i;
		usebpm = val;
		for (i=0;i<6;i++) {
			mTaps[i]->Update(false);
		}
	}
}

#ifdef DEMO

#include <FL/filename.H>
#include <FL/Fl_Hold_Browser.H>

extern int infile_size;
extern int infile_index;
extern char* infile_buf;


class MyBrowser:public Fl_Hold_Browser
{
public:
	MyBrowser(int x,int y,int w,int h):
	Fl_Hold_Browser(x,y,w,h)
	{
	}
	int handle(int e)
	{
		if (e==FL_SHORTCUT && Fl::event_key()==32)
		{
			do_callback();
		}else
		return Fl_Hold_Browser::handle(e); 
	}
};

class SoundList:public Fl_Window
{
public:
	Fl_Browser* b; 
	SoundList():Fl_Window(200,200,"Sounds")
	{
		b = new MyBrowser(0,0,200,200);
		dirent **list;
		int nfiles = filename_list("/root/dafx/",&list);
		int i;
		if (nfiles>0)
		{
			for (i = 0 ; i<nfiles; i++) 
			{
				if (!filename_isdir(list[i]->d_name) && 
					strcmp(filename_ext(list[i]->d_name),".raw")==0)
				{
					char tmp[1024];
					strcpy(tmp,"/root/dafx/");
					strcat(tmp,list[i]->d_name);
					b->add(list[i]->d_name,strdup(tmp));
				}
			}
			for (i = nfiles; i > 0;) 
				free((void*)(list[--i]));free((void*)list);
		}
		b->add("STOP",0);
		b->callback((Fl_Callback*) cb);
		resizable(b);
	}
	static void cb(Fl_Browser* b)
	{
		if (	Fl::event_key() && (
			Fl::event_key()==FL_Up ||
			Fl::event_key()==FL_Down)
		) return;
		char* filename = (char*) b->data(b->value());
		if (filename)  {
		printf("playing %s\n",filename);
// this loop should be in another thread, with RT priority,
// and the infile_buf should be smaller and circular.
			FILE* infile;
			infile_size = 0;
			infile_index = 0;
			infile = fopen(filename,"rb");
			int n = 0;
			do {
				n = fread(infile_buf+infile_size,1,4096,infile);
				if (n>0) infile_size += n;
			} while (n>0);
			fclose(infile);
		}else{
			infile_size = 0;
			infile_index = 0;
		}
	}
};

class MTDList:public Fl_Window
{
public:
	GUI* gui;
	MTDList(GUI* g):Fl_Window(200,200,"Examples")
	{
		gui = g;
		Fl_Browser* b = new MyBrowser(0,0,200,200);
		dirent **list;
		int nfiles = filename_list("/root/dafx/",&list);
		int i;
		if (nfiles>0)
		{
			for (i = 0 ; i<nfiles; i++) 
			{
				if (!filename_isdir(list[i]->d_name) && 
					strcmp(filename_ext(list[i]->d_name),".mtd")==0)
				{
					char tmp[1024];
					strcpy(tmp,"/root/dafx/");
					strcat(tmp,list[i]->d_name);
					b->add(list[i]->d_name,strdup(tmp));
				}
			}
			for (i = nfiles; i > 0;) 
				free((void*)(list[--i]));free((void*)list);
		}
		b->callback((Fl_Callback*) cb);
		resizable(b);
		user_data(this);
	}
	static void cb(Fl_Browser* b)
	{
		if (	Fl::event_key() && (
			Fl::event_key()==FL_Up ||
			Fl::event_key()==FL_Down)
		) return;
		char* filename = (char*) b->data(b->value());
		if (filename)
			((MTDList*)(b->parent()->user_data()))->gui->Load(filename);
	}
};

#endif //DEMO

void run_gui(void)
{
	GUI* gui=new GUI(TOTALW,TOTALH);
	gui->show();
	gui->end();
#ifdef DEMO
	{
		SoundList * p = new SoundList;
		p->show();
	}
	
	{
		MTDList * p = new MTDList(gui);
		p->show();
	}
#endif //DEMO
	
	Fl::run();
}

