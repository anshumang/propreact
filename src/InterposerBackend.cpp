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

#include "InterposerBackend.h"

InterposerBackend::InterposerBackend(std::string req_url, std::string resp_url, int tenant_id, RequestPool *req_pool, GlobalWindow *gwin)
   : m_tenant_id(tenant_id), m_req_pool(req_pool), m_gwin(gwin)
{
    m_req_comm = new Communicator(req_url, RECEIVER);
    m_req_comm->bind();
    m_resp_comm = new Communicator(resp_url, SENDER);
    m_resp_comm->connect();
}

InterposerBackend::~InterposerBackend()
{
    delete m_req_comm;
    delete m_resp_comm;
}

void InterposerBackend::join()
{
    m_thr.join();
}

void InterposerBackend::start()
{
    m_thr = std::thread(&InterposerBackend::process, this);
}

void InterposerBackend::process()
{
   while(true)
   {
       void *buf = NULL;
       //std::cout << "IB : Before receive" << std::endl;
       int bytes = m_req_comm->receive(&buf);
       struct timeval now, later;
       gettimeofday(&now, NULL);
       unsigned long *pkt = (unsigned long*)buf;
       //std::cout << pkt[0]/sizeof(unsigned long) << std::endl;
       //std::cout << pkt[1] << std::endl;
       m_req_pool->update(std::make_pair(m_tenant_id, pkt[1]));
       for(int i=0; i<(pkt[0]/sizeof(unsigned long)-2)/3; i++)
       {
          //std::cout << *((unsigned long *)buff+i) << std::endl;
          //std::cout << pkt[2+3*i] << " " << pkt[3*i+3] << " " << pkt[3*i+4] << std::endl;
          if(m_tenant_id == 0)
          {
             m_gwin->update_idle(pkt[2+3*i], pkt[3*i+3], pkt[3*i+4], m_tenant_id);
          }
          else
          {
             m_gwin->update_active(pkt[2+3*i], pkt[3*i+3], pkt[3*i+4], m_tenant_id);
          }
       }
       //std::cout << "IB : After receive" << std::endl;
       m_req_pool->wait(m_tenant_id);
       assert(bytes >= 0);
       m_req_comm->freemsg(buf);
       std::string resp("RESPONSE");
       assert((bytes = m_resp_comm->send(resp.c_str(), resp.length()+1)) >= 0);
       gettimeofday(&later, NULL);
       std::cerr << (later.tv_sec - now.tv_sec)*1000000 + (later.tv_usec - now.tv_usec) << std::endl;
   }
}
