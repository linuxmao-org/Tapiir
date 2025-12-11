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
#include "IRWindow.hxx"

#include "globals.hxx"

#include "MTDdecl.hxx"

IRWidget::IRWidget(int X,int Y):Fl_Widget(X,Y,512,512)
{
	my_mtd = 0;
}

void IRWidget::draw(void)
{
	fl_color(FL_WHITE);
	fl_rectf(x(),y(),w(),h());
	fl_color(FL_BLACK);
	
	int k=0;
	int xc=x();
	int yc1=y()+128;
	int yc2=y()+128+256;
	while (k<1024) {
		fl_yxline(xc,yc1,yc1-(outbuffer[k]>>8));
		k++;
		fl_yxline(xc,yc2,yc2-(outbuffer[k]>>8));
		k++;
		xc++;
	}
}

void IRWidget::run(void) 
{
	int i,j;
	
	if (my_mtd == 0) {
		my_mtd = new MTD<float,short>(2048,2,6);
	}
	
	for (j=0;j<8;j++) {
		for (i=0;i<9;i++) {
			my_mtd->SetGain(j,i,mtd->GetGain(j,i));
		}
	}
	
	for (i=0;i<6;i++) {
		my_mtd->delaylen[i] = mtd->delaylen[i];
		if (my_mtd->delaylen[i]>256) { // dont do delays with too long length
			my_mtd->delaylen[i]=1;
			my_mtd->SetGain(i,my_mtd->OverallID(),0);
		}
	}
	
	for (i=0;i<1024;i++) {
		outbuffer[i]=0;
	}
	outbuffer[0]=32767;

	my_mtd->Apply(outbuffer,512);
}
