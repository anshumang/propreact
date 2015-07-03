/*
 * This file is part of propreact
 * 
 * propreact is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * propreact is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with propreact. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Anshuman Goswami <anshumang@gatech.edu>
 */

#include "GlobalWindow.h"

GlobalWindow *p_global_window = NULL;

Record::Record(unsigned long gx, unsigned long active, unsigned long idle)
   : m_g_x(gx), m_active(active), m_idle(idle)
{
}

GlobalWindow::GlobalWindow()
  : m_stale(true)
{
   m_mutex.lock();
   std::cout << "aka GlobalWindow CTOR " << m_recs.size() << std::endl;
   m_mutex.unlock();
}

GlobalWindow::~GlobalWindow()
{

}

void GlobalWindow::produce(RecordVec r)
{
   //std::cout << "aka GlobalWindow produce " << m_recs.size() << " " << r.size() << std::endl;
   m_mutex.lock();
   if(r.size() > m_recs.size())
   {
      int idx=0;
      for (; idx<m_recs.size(); idx++)
      {
           m_recs[idx] = r[idx];
      }
      for(; idx<r.size(); idx++)
      {
           m_recs.push_back(r[idx]);
      }
   }
   else
   {
      int idx=0, lim=m_recs.size();
      for (; idx<r.size(); idx++)
      {
           m_recs[idx] = r[idx];
      }
      for(; idx<lim; idx++)
      {
           m_recs.pop_back();
      }
   }
   m_stale = false;
   m_mutex.unlock();
}

RecordVec GlobalWindow::consume()
{
   RecordVec r;
   //std::cout << "aka GlobalWindow consume " << m_recs.size() << std::endl;
   m_mutex.lock();
   if(!m_stale)
   {
      std::copy(m_recs.begin(), m_recs.end(), std::back_inserter(r));
      m_stale = true;
   }
   m_mutex.unlock();
   return r;
}
