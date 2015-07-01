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

#include "Window.h"

Window *p_window = NULL;

Window::Window()
{

}

Window::~Window()
{

}

void Window::WriteDataIdle(ExperimentalKey k, unsigned long val)
{
      auto search = m_table_idle.find(k);
      if(search == m_table_idle.end())
      {
          //std::cout << "[Write] "<< k.x << " " << k.y << " " << k.z << " key not present, adding it and adding value " << val << std::endl;
          /*if queue not created for this key, create one, add to it and insert it to the table*/
          MinIdleQueue q;
          q.push(val);
          m_table_idle.emplace(std::make_pair(k, q));
          return;
      }
      /*if queue already present, add to it and insert it to the table*/
      MinIdleQueue q = search->second;
      //std::cout << "[Write] " << k.x << " " << k.y << " " << k.z << " key present, adding it and adding value " << val << " top at " << q.top() << " with size of  " << q.size() << std::endl;
      q.push(val);
      m_table_idle.erase(search);
      m_table_idle.emplace(std::make_pair(k, q));

/*DEBUG*/
#if 0
      search = m_table_idle.find(k);
      if(search == m_table_idle.end())
      {
          /*if key not present, assume no idle period available*/
          std::cout << "[Write-Read] " << k.x << " " << k.y << " " << k.z << " key not present with val " << val << " -- this should not happen !" << std::endl;
          return;
      }
      q = search->second;
      unsigned long valr = q.top();
      std::cout << "[Write-Read] " << k.x << " " << k.y << " " << k.z << " key present and value is " << valr << " with size of " << q.size() << std::endl;
#endif
/*DEBUG*/
}

void Window::WriteDataActive(ExperimentalKey k, unsigned long val)
{
      auto search = m_table_active.find(k);
      if(search == m_table_active.end())
      {
          //std::cout << "[Write] "<< k.x << " " << k.y << " " << k.z << " key not present, adding it and adding value " << val << std::endl;
          /*if queue not created for this key, create one, add to it and insert it to the table*/
          MinActiveQueue q;
          q.push(val);
          m_table_active.emplace(std::make_pair(k, q));
          return;
      }
      /*if queue already present, add to it and insert it to the table*/
      MinActiveQueue q = search->second;
      //std::cout << "[Write] " << k.x << " " << k.y << " " << k.z << " key present, adding it and adding value " << val << " top at " << q.top() << " with size of  " << q.size() << std::endl;
      q.push(val);
      m_table_active.erase(search);
      m_table_active.emplace(std::make_pair(k, q));
}

unsigned long Window::ReadDataIdle(ExperimentalKey k)
{
      auto search = m_table_idle.find(k);
      if(search == m_table_idle.end())
      {
          /*if key not present, assume no idle period available*/
          return (unsigned long)0;
      }
      MinIdleQueue q = search->second;
      unsigned long val = q.top();
      //std::cout << "[Read] " << k.x << " " << k.y << " " << k.z << " key present and value is " << val << " with size of " << q.size() << std::endl;
      return val;
}

unsigned long Window::ReadDataActive(ExperimentalKey k)
{
      auto search = m_table_active.find(k);
      if(search == m_table_active.end())
      {
          /*if key not present, assume max active period*/
          return (unsigned long)ULONG_MAX;
      }
      MinActiveQueue q = search->second;
      unsigned long val = q.top();
      //std::cout << "[Read] " << k.x << " " << k.y << " " << k.z << " key present and value is " << val << " with size of " << q.size() << std::endl;
      return val;
}

void Window::Acquire()
{
   m_mutex.lock();
}

void Window::Release()
{
   m_mutex.unlock();
}
