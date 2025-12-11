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
#ifndef __MTDStore__
#define __MTDStore__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MTDBase.hxx"

/* template class to read/write the settings for an MTD from/to disk
*/
class MTDStore
{
public:
	MTDStore()
	{
	}
	int Save(MTDBase& mtd,const char* filename)
	{
		int i,j;
		FILE* f = fopen(filename,"w");
		fprintf(f,"#MTD####################################\n");
		fprintf(f,"# multi tap delay settings\n");
		fprintf(f,"# maarten@resorama.com\n");
		fprintf(f,"########################################\n");
		fprintf(f,"ndelays %d\n",mtd.ndelays);
		for (i=0;i<mtd.ndelays+mtd.nchannels;i++)
		{
			fprintf(f,"########################################\n");
			for (j=0;j<mtd.ndelays+mtd.nchannels+1;j++)
			{
				fprintf(f,"gain %d %d %f\n",i,j,mtd.GetGain(i,j));
			}
		}
		fprintf(f,"########################################\n");
		for (i=0;i<mtd.ndelays;i++)
			{
				fprintf(f,"delaylen %d %d\n",i,mtd.delaylen[i]);
			}
		fprintf(f,"########################################\n");
		fclose(f);
		return 0;
	}
	
	int Load(MTDBase& mtd,const char* filename)
	{
		char tmp[256];
		FILE* f = fopen(filename,"r");
		while (fgets(tmp,255,f))
		{
			char* keyword = strtok(tmp," ");
			if (keyword) {
				if (strcmp(keyword,"ndelays")==0) {
					char* val = strtok(0," ");
					if (val) {
						int ndelays = atoi(val);
						if (ndelays!=mtd.ndelays)
						{
							return -1;
						}
					}
				}
				else if (strcmp(keyword,"nchannels")==0) {
					char* val = strtok(0," ");
					if (val) {
						int nchannels = atoi(val);
						if (nchannels!=mtd.nchannels)
						{
							return -1;
						}
					}
				}
				else if (strcmp(keyword,"gain")==0) {
					char* mixerid = strtok(0," ");
					char* gainid = strtok(0," ");
					char* val = strtok(0," ");
					if (mixerid && gainid && val) {
						mtd.SetGain(atoi(mixerid),atoi(gainid),atof(val));
					}
				}
				else if (strcmp(keyword,"delaylen")==0) {
					char* tapid = strtok(0," ");
					char* val = strtok(0," ");
					if (tapid && val) {
						mtd.delaylen[atoi(tapid)] = atoi(val);
					}
				}
			}
		}
		fclose(f);
		return 0;
	}
};

#endif
