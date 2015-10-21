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
  : m_stale(true), m_tenants(2)
{
   m_mutex.lock();
   //std::cout << "aka GlobalWindow CTOR " << m_recs.size() << std::endl;
   for(int i=0; i<m_tenants; i++)
   {
      ActiveIdlePair dummyp(0,0);
      IdleMinQueue minq;
      minq.push(dummyp);
      ActiveMaxQueue maxq;
      maxq.push(dummyp);
      IdleMap imap;
      imap.insert(std::make_pair(0, minq));
      ActiveMap amap;
      amap.insert(std::make_pair(0, maxq));
      m_tenants_idle_map_vector.push_back(imap);
      m_tenants_active_map_vector.push_back(amap);
   }
   //std::cout << m_tenants_idle_map_vector.size() << " " << m_tenants_active_map_vector.size() << std::endl;
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
   for(int i=0; i<r.size(); i++)
   {
      //std::cout << m_recs[i].m_g_x << " " << m_recs[i].m_active << " " << m_recs[i].m_idle << std::endl;
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
      for(int i=0; i<r.size(); i++)
      {
         //std::cout << r[i].m_g_x << " " << r[i].m_active << " " << r[i].m_idle << std::endl;
         //std::cout << m_recs[i].m_g_x << " " << m_recs[i].m_active << " " << m_recs[i].m_idle << std::endl;
      }
   }
   m_mutex.unlock();
   return r;
}

void GlobalWindow::update_idle(ExperimentalKey k, unsigned long va, unsigned long vi, unsigned int t)
{
   auto searchm = m_tenants_idle_map_vector[t];
   auto searchmq = searchm.find(k);
   if(searchmq == searchm.end())
   {
      IdleMinQueue minq;
      minq.push(std::make_pair(va, vi));
      m_tenants_idle_map_vector[t].insert(std::make_pair(k,minq));
      auto searchmqq = m_tenants_idle_map_vector[t].find(k);
      std::cout << "[not found] update_idle after " << m_tenants_idle_map_vector[t].size() << " " << searchmqq->second.size() << std::endl;
      return;
   }
   IdleMinQueue minq(searchmq->second);
   minq.push(std::make_pair(va, vi));
   //std::cout << "[found] size of IdleMinQueue to insert " << searchmq->second.size() << " " << minq.size() << std::endl;
   m_tenants_idle_map_vector[t].erase(/*searchmq*/k);
   //std::cout << "[found] erase " << m_tenants_idle_map_vector[t].size() << std::endl;
   m_tenants_idle_map_vector[t].emplace(std::make_pair(k, minq));
   //std::cout << "[found] insert " << m_tenants_idle_map_vector[t].size() << std::endl;
   auto searchmqq = m_tenants_idle_map_vector[t].find(k);
   std::cout << "[found] update_idle after " << m_tenants_idle_map_vector[t].size() << " " << searchmqq->second.size() << std::endl;
}

void GlobalWindow::update_active(ExperimentalKey k, unsigned long va, unsigned long vi, unsigned int t)
{
   m_mutex.lock();
   auto searchm = m_tenants_active_map_vector[t];
   auto searchmq = searchm.find(k);
   if(searchmq == searchm.end())
   {
      ActiveMaxQueue maxq;
      maxq.push(std::make_pair(va, vi));
      m_tenants_active_map_vector[t].insert(std::make_pair(k,maxq));
      auto searchmqq = m_tenants_active_map_vector[t].find(k);
      //std::cout << "[not found] update_active after " << m_tenants_active_map_vector[t].size() << " " << searchmqq->second.size() << std::endl;
      return;
   }
   ActiveMaxQueue maxq(searchmq->second);
   maxq.push(std::make_pair(va, vi));
   m_tenants_active_map_vector[t].erase(/*searchmq*/k);
   m_tenants_active_map_vector[t].emplace(std::make_pair(k, maxq));
   auto searchmqq = m_tenants_active_map_vector[t].find(k);
   //std::cout << "[found] update_active after " << m_tenants_active_map_vector[t].size() << " " << searchmqq->second.size() << std::endl;
   m_mutex.unlock();
}

unsigned long GlobalWindow::retrieve_idle(unsigned int t, ExperimentalKey k)
{
    m_mutex.lock();
    auto searchm = m_tenants_idle_map_vector[t];
    auto searchmq = searchm.find(k);
    m_mutex.unlock();
    return 0;
}

unsigned long GlobalWindow::retrieve_active(unsigned int t, ExperimentalKey k)
{
    m_mutex.lock();
    auto searchm = m_tenants_active_map_vector[t];
    auto searchmq = searchm.find(k);
    m_mutex.unlock();
    return 0;
}

