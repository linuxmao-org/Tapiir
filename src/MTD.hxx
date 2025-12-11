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
#ifndef __MTD__
#define __MTD__

#include "MTDBase.hxx"

/* template class for multi tap delay
**
** class T = internal processing data type, typically float or int
** class T_IO = I/O data type, typically float or short
*/
template <class T, class T_IO> class MTD:public MTDBase
{
private:
	T* buffer;
	T* gain;

	/**** the following might be specialized for specific data types ****/
	float FromGain(const T& g) const { return (float) g; }
	T ToGain(const float& v) const { return (T) v; }
	void MulAdj( T& i ) const { }	
	void Clip( T&) const {}
	
public:
	MTD(const int& _size,const int& _nchannels,const int& _ndelays)
	:MTDBase(_size,_nchannels,_ndelays)
	{
		int i;

		buffer = new T[size];
		
		gain = new T[ (ndelays+nchannels) * (ndelays+nchannels+1) ];

		T *aptr = gain;

		for (i = 0; i<(ndelays+nchannels) * (ndelays+nchannels+1); i++)
		{
			(*aptr++) = 0;
		}
	}

	~MTD()
	{
		delete [] buffer;
		delete [] gain;
	}

	/* set gain, normalized to 1 */	
	void SetGain(int mixerid,int gainid,const float& val)
	{
		gain[ mixerid * (ndelays+nchannels+1) + gainid ] = ToGain(val);
	}

	/* get gain, normalized to 1 */	
	float GetGain(int mixerid,int gainid) const
	{
		return FromGain(gain[ mixerid * (ndelays+nchannels+1) + gainid ]);
	}

	void Apply(T_IO* inplace,int insize)
	{
		ApplyInterleaved(inplace,inplace,insize);
	}

	void ApplyInterleaved(T_IO* isamples,T_IO* osamples,int insize)
	{
		T val[256];
		T *a = gain;
		int i,j;
		while (insize)
		{
			register T* aptr;
			register T* valptr;
			register T acc;
			
			valptr=val;

			for (i=0;i<ndelays;i++) {
				*valptr++ = buffer[(writeIndex[i]+size-delaylen[i])&mask];
			}
			for (i=0;i<nchannels;i++) {
				*valptr++ = (T) *isamples++;
			}

			aptr=a;
			for (i=0;i<ndelays;i++) {
				valptr=val;

				acc = (*aptr++) * (*valptr++);
				for (j=1;j<ndelays + nchannels;j++) {
					acc += (*aptr++) * (*valptr++);
				}
				MulAdj(acc);				

				acc*=*aptr++;
				MulAdj(acc);				

				Clip(acc);
				
				buffer[writeIndex[i]]=acc;
				writeIndex[i]=(writeIndex[i]+1)&mask;
			}
			for (i=0;i<nchannels;i++) {
				valptr=val;

				acc = (*aptr++) * (*valptr++);
				for (j=1;j<ndelays + nchannels;j++) {
					acc += (*aptr++) * (*valptr++);
				}
				MulAdj(acc);				

				acc*=*aptr++;
				MulAdj(acc);				

				Clip(acc);
				
				*osamples++ = (T_IO) acc;
			}
			insize--;
		}
	}

	void ApplyNonInterleaved(T_IO** isamples,T_IO** osamples,int insize)
	{
		T val[256];
		T *a = gain;
		int i,j;
		
		while (insize)
		{
			register T* aptr;
			register T* valptr;
			register T acc;
			
			valptr=val;

			for (i=0;i<ndelays;i++) {
				*valptr++ = buffer[(writeIndex[i]+size-delaylen[i])&mask];
			}
			for (i=0;i<nchannels;i++) {
				*valptr++ = (T) *isamples[i]++;
			}

			aptr=a;
			for (i=0;i<ndelays;i++) {
				valptr=val;

				acc = (*aptr++) * (*valptr++);
				for (j=1;j<ndelays + nchannels;j++) {
					acc += (*aptr++) * (*valptr++);
				}
				MulAdj(acc);				

				acc*=*aptr++;
				MulAdj(acc);				

				Clip(acc);
				
				buffer[writeIndex[i]]=acc;
				writeIndex[i]=(writeIndex[i]+1)&mask;
			}
			for (i=0;i<nchannels;i++) {
				valptr=val;

				acc = (*aptr++) * (*valptr++);
				for (j=1;j<ndelays + nchannels;j++) {
					acc += (*aptr++) * (*valptr++);
				}
				MulAdj(acc);				

				acc*=*aptr++;
				MulAdj(acc);				

				Clip(acc);
				
				*osamples[i]++ = (T_IO) acc;
			}
			insize--;
		}
	}

};

/****************************** specializations ******************************/

template<> inline float MTD<int,short>::FromGain(const int& g) const { return float(g)/256.; }
template<> inline float MTD<int,float>::FromGain(const int& g) const { return float(g)/256.; }

template<> inline int   MTD<int,short>::ToGain(const float& f) const { return (int)(f*256.); }
template<> inline int   MTD<int,float>::ToGain(const float& f) const { return (int)(f*256.); }

template<> inline void  MTD<float,short>::Clip( float& f) const
{ if (f>32767.) f = 32767.; else if (f<-32767.) f = -32767.; }
template<> inline void  MTD<int  ,short>::Clip( int&   i) const
{ if (i>32767) i = 32767; else if (i<-32767) i = -32767; }
template<> inline void  MTD<float,float>::Clip( float& f) const
{ if (f>1.) f = 1.; else if (f<-1.) f = -1.; }


template<> inline void  MTD<int,float>::MulAdj( int& i) const { i>>=8; }
template<> inline void  MTD<int,short>::MulAdj( int& i) const { i>>=8; }

/*****************************************************************************/

#endif
