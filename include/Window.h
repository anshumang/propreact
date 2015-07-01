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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <queue>
#include <map>
#include <mutex>
#include <cstddef>
#include <climits>
#include <iostream>
#include "Trigger.h"

struct Window
{
   typedef std::priority_queue<unsigned long, std::vector<unsigned long>, std::greater<unsigned long> > MinIdleQueue;
   typedef std::priority_queue<unsigned long> MaxActiveQueue;
   typedef std::priority_queue<unsigned long, std::vector<unsigned long>, std::greater<unsigned long> > MinActiveQueue;
   typedef Grid ExperimentalKey;
   struct ExperimentalKeyCmpFtor
   {
      bool operator()(ExperimentalKey a, ExperimentalKey b)
      {
         return a.x < b.x;
      }
   };
   typedef std::map <ExperimentalKey, MinIdleQueue, ExperimentalKeyCmpFtor> KeyIdleQueueTable;
   //typedef std::map <ExperimentalKey, MaxActiveQueue, ExperimentalKeyCmpFtor> KeyActiveQueueTable;
   typedef std::map <ExperimentalKey, MinActiveQueue, ExperimentalKeyCmpFtor> KeyActiveQueueTable;
   std::mutex m_mutex;
   KeyIdleQueueTable m_table_idle;
   KeyActiveQueueTable m_table_active;
   Window();
   ~Window();
   void WriteDataIdle(ExperimentalKey k, unsigned long val);
   void WriteDataActive(ExperimentalKey k, unsigned long val);
   unsigned long ReadDataIdle(ExperimentalKey k);
   unsigned long ReadDataActive(ExperimentalKey k);
   void Acquire();
   void Release();
};

#endif
