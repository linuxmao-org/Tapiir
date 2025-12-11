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
#include "MTD.hxx"
#include "MTDStore.hxx"
#include <unistd.h>
#include <fcntl.h>

/* convert old tapiir (<=0.6) files to new format */

MTD<float,short> mtd(0,2,6);
MTDStore<float,short> store;

int load_mtd(char* filename)
{
	int i,j;
	int fd=open(filename,O_RDONLY);
	char dum[4];
	read(fd,dum,4);
	if (memcmp(dum,"MTD0",4)!=0) {
		close(fd);
		return -1;
	}
	
	int CONVID[9]={6,7,0,1,2,3,4,5,8};
	
	for (j=0;j<8;j++) {
		for (i=0;i<9;i++) {
			int g;
			read(fd,&g,4);

			mtd.SetGain(CONVID[j],CONVID[i],float(g)/256.);
		}
	}
	for (i=0;i<6;i++) {
		int val;
		read(fd,&val,4);
		mtd.delaylen[i]=val;
	}

	close(fd);
	return 0;
}

main(int argc,char** argv)
{
	load_mtd(argv[1]);
	store.Save(mtd,argv[2]);
}
