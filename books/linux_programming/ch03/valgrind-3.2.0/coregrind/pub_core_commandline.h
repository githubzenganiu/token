
/*--------------------------------------------------------------------*/
/*--- Command line handling.                pub_core_commandline.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2006 Julian Seward 
      jseward@acm.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#ifndef __PUB_CORE_COMMANDLINE_H
#define __PUB_CORE_COMMANDLINE_H


/* Split up the args presented by the launcher to m_main.main(), and
   park them in VG_(args_for_client), VG_(args_for_valgrind) and
   VG_(args_for_valgrind_extras).  The latter are acquired from
   $VALGRIND_OPTS, ./.valgrindrc and ~/.valgrindrc. */

extern void VG_(split_up_argv)( Int argc, HChar** argv );


#endif   // __PUB_CORE_COMMANDLINE_H

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
