/*
 * This file is part of gpu_use_idle_predictor
 * 
 * cupti_profiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * cupti_profiler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cupti_profiler. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Anshuman Goswami <anshumang@gatech.edu>
 */

#ifndef _TRIGGER_H
#define _TRIGGER_H

#include <mutex>
#include <condition_variable>
#include <cassert>

struct Grid
{
   unsigned long x;
   unsigned long y;
   unsigned long z;
   Grid();
   Grid(unsigned long, unsigned long, unsigned long);
};

struct Trigger
{
   std::mutex m_lock_1, m_lock_2;
   std::condition_variable m_notify_1, m_notify_2;
   bool m_ready_1, m_ready_2;
   Grid m_grid;
   Trigger();
   ~Trigger();
   void Wait(int);
   void Notify(int);
   void WriteData(Grid g);
   void ReadData(Grid *g);
};

#endif

