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
#include "Layout.hxx"

#include "LayoutDefines.hxx"


void layout_small(void)
{
#define __DO_DECL__
#include "LayoutSet.cxx"
}
