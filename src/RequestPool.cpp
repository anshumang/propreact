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

#include "RequestPool.h"

RequestPool::RequestPool()
  :m_num_tenants(2)
{

}

RequestPool::~RequestPool()
{

}

void RequestPool::update(TenantIdExperimentalKeyPair p)
{
    //for(auto search = m_tenant_requests.begin(); search != m_tenant_requests.end(); search++)
    //{
       //assert(search.first != p.first); //Request and response are synchronous, there shouldn't be pending requests when a new request arrives from a tenant
    //}
    m_mutex.lock();
    assert(m_tenant_requests.find(p.first) == m_tenant_requests.end());
    m_tenant_requests.insert(std::make_pair(p.first, p.second));
    if (m_tenant_requests.find(p.first) != m_tenant_requests.end())
    {
      std::cerr << "RequestPool::update OK" << std::endl;
    }
    m_mutex.unlock();
}

ExperimentalKey RequestPool::retrieve(unsigned int t)
{
    //std::cerr << "RequestPool::retrieve entering" << std::endl;
    ExperimentalKey k=0; 
    m_mutex.lock();
    if (m_tenant_requests.find(t) != m_tenant_requests.end())
    {
       k = m_tenant_requests[t];
       std::cerr << "RequestPool::retrieve OK" << std::endl;
    }
    m_tenant_requests.erase(t);
    m_mutex.unlock();
    //std::cerr << "RequestPool::retrieve exiting" << std::endl;
    return k;
}

void RequestPool::wait(int t)
{
    std::cerr << "RequestPool::wait entering" << std::endl;
    std::unique_lock<std::mutex> lk(m_tenant_mutex[t]);
    m_tenant_notify[t].wait(lk, [this, t]{return m_tenant_ready[t];});
    m_tenant_ready[t] = false;
    lk.unlock();
    std::cerr << "RequestPool::wait exiting" << std::endl;
}

void RequestPool::release(int t)
{
    //std::cerr << "RequestPool::release entering" << std::endl;
    std::lock_guard<std::mutex> lk(m_tenant_mutex[t]);
    m_tenant_ready[t] = true;
    m_tenant_notify[t].notify_one();
    //std::cerr << "RequestPool::release exiting" << std::endl;
}
