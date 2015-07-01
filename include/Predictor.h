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

#include <thread>
#include <cstddef>
#include <iostream>
#include <sys/time.h>
#include "Trigger.h"
#include "Window.h"

struct Predictor
{
   std::thread m_thr;
   Trigger* m_trig;
   Window* m_win;
   long m_last, m_start_lib, m_end_lib;
   bool m_lib;
   Predictor(Trigger*, Window*);
   ~Predictor();
   void ProcessTrigger();
};

#endif
