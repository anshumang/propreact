/*
 * This file is part of gpu_use_idle_predictor
 * 
 * gpu_use_idle_predictor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gpu_use_idle_predictor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gpu_use_idle_predictor. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Anshuman Goswami <anshumang@gatech.edu>
 */

#include "CuptiProfiler.h"
#include "Interposer.h"
#include "Predictor.h"
#include "Window.h"

extern CuptiProfiler *p_profiler;
extern Interposer *p_interposer;
extern Predictor *p_predictor;
extern Window *p_window;
extern Trigger *p_trigger;

extern "C"
void XrushCreate()
{
   p_window = new Window();
   p_trigger = new Trigger();
   p_profiler = new CuptiProfiler(p_window);
   p_interposer = new Interposer(p_trigger);
   p_predictor = new Predictor(p_trigger, p_window);
}

extern "C"
void XrushDestroy()
{
   std::cout << "XrushDestroy" << std::endl;
   delete p_profiler;
   delete p_interposer;
   //delete p_predictor;
   delete p_window;
}

extern "C"
void XrushProfile()
{
   p_profiler->read();
}

extern "C"
void XrushLaunch(unsigned long gridX, unsigned long gridY, unsigned long gridZ, unsigned long blockX, unsigned long blockY, unsigned long blockZ)
{
   p_interposer->launch(gridX, gridY, gridZ, blockX, blockY, blockZ);
}
