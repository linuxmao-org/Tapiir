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
#ifndef __MTDBASE__
#define __MTDBASE__

typedef unsigned int delaylength_t;

class MTDBase
{
public:
	const int nchannels;
	const int ndelays;
	delaylength_t *delaylen;
protected:
	const int size;
	const int mask;
	delaylength_t *writeIndex;
	
	MTDBase(const int& _size,const int& _nchannels,const int& _ndelays)
	:nchannels(_nchannels),ndelays(_ndelays),
	 size(_size),mask(_size-1)
	{
		int i;

		writeIndex = new delaylength_t[ndelays];
		delaylen = new delaylength_t[ndelays];

		for (i=0;i<ndelays;i++) {
			writeIndex[i]=i*(size/ndelays);
			delaylen[i]=1;
		}
	}
	
	~MTDBase()
	{
		delete [] writeIndex;
		delete [] delaylen;
	}

public:
	int ChannelID(const int& chn) const
	{
		return chn + ndelays;
	}

	int OverallID(void) const
	{
		return ndelays + nchannels;
	}
	virtual void SetGain(int mixerid,int gainid,const float& val) = 0;
	virtual float GetGain(int mixerid,int gainid) const = 0;
};

#endif

