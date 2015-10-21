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

#include "Arbiter.h"

Arbiter::Arbiter(RequestPool *req_pool, GlobalWindow *gwin)
    : m_req_pool(req_pool), m_gwin(gwin), m_num_tenants(2)
{

}

Arbiter::~Arbiter()
{

}

void Arbiter::join()
{
    m_thr.join();
}

void Arbiter::start()
{
    m_thr = std::thread(&Arbiter::process, this);
}

void Arbiter::process()
{
    while(true)
    {
        std::map<unsigned int, ExperimentalKey> tenants_with_requests;
        for(int t=0; t<m_num_tenants; t++)
        {
            ExperimentalKey k = m_req_pool->retrieve(t);
            if(k>0){
              tenants_with_requests.insert(std::make_pair(t,k));
            }
        }
        if(tenants_with_requests.size()>0){
          std::cerr << "Arbiter::process found " << tenants_with_requests.size() << " requests" << std::endl;
        }
        std::set<unsigned int> tenants_to_be_responded;
        /*for now, simply adds the lookup cost*/
        for(auto search=tenants_with_requests.begin(); 
            search!=tenants_with_requests.end();
            search++)
        {
             if(search->first == 0)
             {
                 m_gwin->retrieve_idle(search->first, search->second);
                 tenants_to_be_responded.insert(search->first);
             }
             else
             {
                 m_gwin->retrieve_active(search->first, search->second);
                 tenants_to_be_responded.insert(search->first);
             }
        }
        if(tenants_to_be_responded.size()>0){
          std::cerr << "Arbiter::process found " << tenants_to_be_responded.size() << " responses" << std::endl;
        }
        for(auto search=tenants_to_be_responded.begin();
            search!=tenants_to_be_responded.end();
            search++)
        {
             m_req_pool->release(*search);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
