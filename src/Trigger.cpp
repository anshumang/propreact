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

#include "Trigger.h"

Trigger *p_trigger = NULL;

Grid::Grid()
  : x(1), y(1), z(1)
{

}

Grid::Grid(unsigned long gx, unsigned long gy, unsigned long gz)
  : x(gx), y(gy), z(gz)
{

}

Trigger::Trigger()
{

}

Trigger::~Trigger()
{

}

void Trigger::Wait(int idx)
{
   if(idx==1)
   {
     std::unique_lock<std::mutex> lk(m_lock_1);
     m_notify_1.wait(lk, [this]{return m_ready_1;});
     m_ready_1 = false;
     lk.unlock();
   }
   else if(idx==2)
   {
     std::unique_lock<std::mutex> lk(m_lock_2);
     m_notify_2.wait(lk, [this]{return m_ready_2;});
     m_ready_2 = false;
     lk.unlock();
   }
   else
   {
     assert(0);//Illegal wait index
   }
}

void Trigger::Notify(int idx)
{
   if(idx==1)
   {
     std::lock_guard<std::mutex> lk(m_lock_1);
     m_ready_1 = true;
     m_notify_1.notify_one();
   }
   else if(idx==2)
   {
     std::lock_guard<std::mutex> lk(m_lock_2);
     m_ready_2 = true;
     m_notify_2.notify_one();
   }
   else
   {
     assert(0);//Illegal notify index
   }
}

void Trigger::ReadData(Grid *g)
{
    g->x = m_grid.x; g->y = m_grid.y; g->z = m_grid.z;
}

void Trigger::WriteData(Grid g)
{
    m_grid.x = g.x; m_grid.y = g.y; m_grid.z = g.z;
}
