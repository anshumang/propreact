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
    assert(m_tenant_requests.find(p.first) == m_tenant_requests.end());
    m_tenant_requests.insert(std::make_pair(p.first, p.second));
}

ExperimentalKey RequestPool::retrieve(unsigned int t)
{
    ExperimentalKey k=0; 
    if (m_tenant_requests.find(t) != m_tenant_requests.end())
    {
       k = m_tenant_requests[t];
    }
    m_tenant_requests.erase(t);
    return k;
}
