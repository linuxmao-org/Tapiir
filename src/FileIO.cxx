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
#include "FileIO.hxx"
#include "MTD.hxx"
#include "MTDdecl.hxx"
#include "MTDStore.hxx"
#include "globals.hxx"

int load_mtd(char* filename)
{
	MTDStore store;
	store.Load(*mtd,filename);
}

int save_mtd(char* filename)
{
	MTDStore store;
	store.Save(*mtd,filename);
}
