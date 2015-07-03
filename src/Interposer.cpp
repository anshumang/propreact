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

#include "Interposer.h"

Interposer *p_interposer = NULL;

Interposer::Interposer(/*std::string req_url, std::string resp_url,*/ Trigger* trig)
  : m_last(0), m_trig(trig)
{
    std::cout << "Interposer CTOR" << std::endl;
/*
    m_req_comm = new Communicator(req_url, SENDER);
    m_req_comm->connect();
    m_resp_comm = new Communicator(resp_url, RECEIVER);
    m_resp_comm->bind();
*/
}

Interposer::~Interposer()
{
    //delete m_req_comm;
    //delete m_resp_comm;
}

void Interposer::launch(unsigned long gridX, unsigned long gridY, unsigned long gridZ, unsigned long blockX, unsigned long blockY, unsigned long blockZ)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long curr = now.tv_sec*1000000 + now.tv_usec;
    //std::cout << curr - m_last << " " << gridX << " " << gridY << " " << gridZ << " " << blockX << " " << blockY << " " << blockZ << std::endl;
    m_last = curr;
    Grid g(gridX, gridY, gridZ);
    //std::cout << "I " << g.x << std::endl;
    m_trig->WriteData(g);
    m_trig->Notify(1);
    m_trig->Wait(2);
#if 0
    std::string req("REQUEST");
    //std::cout << "I : Before send " << sizeof(req) << std::endl;
    int bytes=0;
    assert((bytes = m_req_comm->send(req.c_str(), req.length()+1))>=0);    
    //std::cout << "I : After send" << std::endl;
    while(true)
    {
       void *buf = NULL;
       int bytes = m_resp_comm->receive(&buf);
       assert(bytes >= 0);
       m_req_comm->freemsg(buf);
       break;
    }
#endif
}
