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

#ifndef _PREDICTOR_H
#define _PREDICTOR_H

#include <array>
#include <cstring>
#include <thread>
#include <cstddef>
#include <iostream>
#include <sys/time.h>
#include "Trigger.h"
#include "Window.h"
#include "GlobalWindow.h"

struct Predictor
{
   std::thread m_thr;
   Trigger* m_trig;
   Window* m_win;
   GlobalWindow* m_gwin;
   long m_last, m_start_lib, m_end_lib;
   bool m_lib;
   std::array</*char*/unsigned long, 1024> m_pkt; //sizeof(unsigned long) = 8, 2x8 + 3x8 per record, >40 records
   Predictor(std::string req_url, std::string resp_url, Trigger*, Window*, GlobalWindow*);
   ~Predictor();
   void ProcessTrigger();
   Communicator *m_req_comm, *m_resp_comm;
};

#endif
