
/*--------------------------------------------------------------------*/
/*--- The thread state.                            m_threadstate.c ---*/
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

#include "pub_core_basics.h"
#include "pub_core_threadstate.h"
#include "pub_core_libcassert.h"

/*------------------------------------------------------------*/
/*--- Data structures.                                     ---*/
/*------------------------------------------------------------*/

ThreadId VG_(running_tid) = VG_INVALID_THREADID;

ThreadState VG_(threads)[VG_N_THREADS];

/*------------------------------------------------------------*/
/*--- Operations.                                          ---*/
/*------------------------------------------------------------*/

const HChar* VG_(name_of_ThreadStatus) ( ThreadStatus status )
{
   switch (status) {
   case VgTs_Empty:     return "VgTs_Empty";
   case VgTs_Init:      return "VgTs_Init";
   case VgTs_Runnable:  return "VgTs_Runnable";
   case VgTs_WaitSys:   return "VgTs_WaitSys";
   case VgTs_Yielding:  return "VgTs_Yielding";
   case VgTs_Zombie:    return "VgTs_Zombie";
   default:             return "VgTs_???";
  }
}

ThreadState *VG_(get_ThreadState)(ThreadId tid)
{
   vg_assert(tid >= 0 && tid < VG_N_THREADS);
   vg_assert(VG_(threads)[tid].tid == tid);
   return &VG_(threads)[tid];
}

Bool VG_(is_valid_tid) ( ThreadId tid )
{
   /* tid is unsigned, hence no < 0 test. */
   if (tid == 0) return False;
   if (tid >= VG_N_THREADS) return False;
   if (VG_(threads)[tid].status == VgTs_Empty) return False;
   return True;
}

// This function is for tools to call.
ThreadId VG_(get_running_tid)(void)
{
   return VG_(running_tid);
}

Bool VG_(is_running_thread)(ThreadId tid)
{
   ThreadState *tst = VG_(get_ThreadState)(tid);

   return 
//      tst->os_state.lwpid == VG_(gettid)() &&	// check we're this tid
      VG_(running_tid) == tid	           &&	// and that we've got the lock
      tst->status == VgTs_Runnable;		// and we're runnable
}

/* Return true if the thread is still alive but in the process of exiting. */
inline Bool VG_(is_exiting)(ThreadId tid)
{
   vg_assert(VG_(is_valid_tid)(tid));
   return VG_(threads)[tid].exitreason != VgSrc_None;
}

/* Return the number of non-dead Threads */
Int VG_(count_living_threads)(void)
{
   Int count = 0;
   ThreadId tid;

   for(tid = 1; tid < VG_N_THREADS; tid++)
      if (VG_(threads)[tid].status != VgTs_Empty &&
	  VG_(threads)[tid].status != VgTs_Zombie)
	 count++;

   return count;
}

/* Given an LWP id (ie, real kernel thread id), find the corresponding
   ThreadId */
ThreadId VG_(get_lwp_tid)(Int lwp)
{
   ThreadId tid;
   
   for(tid = 1; tid < VG_N_THREADS; tid++)
      if (VG_(threads)[tid].status != VgTs_Empty && VG_(threads)[tid].os_state.lwpid == lwp)
	 return tid;

   return VG_INVALID_THREADID;
}

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
