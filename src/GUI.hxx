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
#ifndef __GUI__
#define __GUI__

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>

#include "tapiir_diagram.hxx"

#include <stdlib.h> /* atoi */

class IRWindow;
class Mixer;
class Tap;

class GUI:public Fl_Window
{
private:
	IRWindow* mIRWindow;

	char* mFilename;

	Mixer* mMixers[8];
	Tap* mTaps[6];

	Fl_Input* mBPMInput;
	Fl_Input* mDIVInput;
public:
	GUI(int wi,int hi);

	void SetFilename(char* filename);
	void SetValues(void);

	void Load(void);
	void Reload(void);
	void Save(void);
	void SaveAs(void);
	void Load(char* filename);
	void Save(char* filename);
	void ShowIR(void);
	void SetBPM(int val);
	void SetDivision(int val);
	void UseBPM(int val);
	
	static void load_cb(Fl_Button*b) {
		GUI* g=(GUI*) b->parent();		
		g->Load();
	}
	static void save_cb(Fl_Button*b) {
		GUI* g=(GUI*) b->parent();		
		g->Save();
	}
	static void saveas_cb(Fl_Button*b) {
		GUI* g=(GUI*) b->parent();		
		g->SaveAs();
	}
	static void reload_cb(Fl_Button*b) {
		GUI* g=(GUI*) b->parent();		
		g->Reload();
	}
		
	static void show_ir_cb(Fl_Button*b) {
		GUI* g=(GUI*) b->parent();		
		g->ShowIR();
	}
	static void bpm_cb(Fl_Input *b) {
		GUI* g=(GUI*) b->parent();		
		g->SetBPM(atoi(b->value()));
	}
	static void div_cb(Fl_Input *b) {
		GUI* g=(GUI*) b->parent();		
		g->SetDivision(atoi(b->value()));
	}
	static void usebpm_cb(Fl_Button *b) {
		GUI* g=(GUI*) b->parent();		
		g->UseBPM(b->value());
	}

	static void show_diagram_cb(Fl_Button*b) {
		tapiir_diagram()->show();
	}
};

#endif
