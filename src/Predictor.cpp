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

#include "Predictor.h"

Predictor *p_predictor = NULL;

Predictor::Predictor(std::string req_url, std::string resp_url, Trigger* trig, Window *win, GlobalWindow *gwin)
  : m_trig(trig), m_win(win), m_last(0), m_lib(false), m_start_lib(0), m_end_lib(0), m_gwin(gwin)
{
   m_thr = std::thread(&Predictor::ProcessTrigger, this);
   m_req_comm = new Communicator(req_url, SENDER);
   m_req_comm->connect();
   m_resp_comm = new Communicator(resp_url, RECEIVER);
   m_resp_comm->bind();
}

Predictor::~Predictor()
{
    delete m_req_comm;
    delete m_resp_comm;
}

void Predictor::ProcessTrigger()
{
   struct timeval now;
   while(true)
   {
      m_trig->Wait(1);
      //std::cout << "New trigger received" << std::endl;;
      Grid g;
      m_trig->ReadData(&g);
      //std::cout << g.x << " " << g.y << " " << g.z << std::endl;
/*Notify after response from daemon, may choose to not contact daemon and then, Interposer released quicker*/
      //m_trig->Notify(2);
      if((g.x==0)&&(g.y==0)&&(!m_lib))
      {
         /*start library kernels active on GPU*/
         //std::cout << "XS " << g.x << " " << g.y << " " << g.z << std::endl;
	 gettimeofday(&now, NULL);
	 long now_t = now.tv_sec*1000000 + now.tv_usec;
         m_start_lib = now_t;
         m_trig->Notify(2);
         continue;
      }
      else if((g.x==1)&&(g.y==0)&&(!m_lib))
      {
         /*end library kernels active on GPU*/
         //std::cout << "XE " << g.x << " " << g.y << " " << g.z << std::endl;
	 gettimeofday(&now, NULL);
	 long now_t = now.tv_sec*1000000 + now.tv_usec;
         m_end_lib = now_t;
         m_lib = true;
         //unsigned long idle = m_win->ReadDataIdle(g);
         //std::cout << "X 0 " << m_end_lib - m_start_lib << " " << idle << std::endl;
         m_trig->Notify(2);
         continue; 
      }

      if(m_lib)
      {
         //std::cout << "XES " << g.x << " " << g.y << " " << g.z << std::endl;
	 gettimeofday(&now, NULL);
	 long now_t = now.tv_sec*1000000 + now.tv_usec;
         unsigned long idle = now_t - m_end_lib;
         //std::cout << m_end_lib - m_start_lib << " " << idle << std::endl;
         Grid gk(0, 0, 0);
         //m_win->WriteDataIdle(gk, idle);
         m_lib = false;
      }
      
      unsigned long active = m_win->ReadDataActive(g);
      unsigned long idle = m_win->ReadDataIdle(g);
      RecordVec r = m_gwin->consume();
      //std::cout << r.size() << std::endl;
      //std::cout << "From Window --- " << g.x << " " << g.y << " " << g.z << " " << idle << std::endl;
      //std::cout << now_t - m_last << " " << idle << std::endl;
      //m_last = now_t + idle;
      /*PREDICTOR OUTPUT*/
      //std::cout << active << "\n" << idle << std::endl;

      //m_last = active+idle;
      std::string req("REQUEST");
      //std::cout << "I : Before send " << sizeof(req) << std::endl;
      int bytes=0;
      //char *buff = new char[(2+(3*r.size()))*sizeof(unsigned long)];
      char *buff = (char *)&m_pkt[0];
      unsigned long pkt_size=(2+(3*r.size()))*sizeof(unsigned long);
      std::memcpy(buff, &pkt_size, sizeof(unsigned long));
      std::memcpy(buff+sizeof(unsigned long), &g.x, sizeof(unsigned long));
      //std::cout << m_pkt[0]/sizeof(unsigned long) << std::endl;
      //std::cout << m_pkt[1] << std::endl;
      for(int i=0; i<r.size(); i++)
      {
         //std::cout << r[i].m_g_x << " " << r[i].m_active << " " << r[i].m_idle << std::endl;
         std::memcpy(buff+(2+3*i)*sizeof(unsigned long), &r[i].m_g_x, sizeof(unsigned long));
         std::memcpy(buff+(3+3*i)*sizeof(unsigned long), &r[i].m_active, sizeof(unsigned long));
         std::memcpy(buff+(4+3*i)*sizeof(unsigned long), &r[i].m_idle, sizeof(unsigned long));
         //std::cout << m_pkt[2+3*i] << " " << m_pkt[3+3*i] << " " << m_pkt[4+3*i] << std::endl;
      }
      //assert((bytes = m_req_comm->send(req.c_str(), req.length()+1))>=0);    
      //std::cout << "--- " << pkt_size/sizeof(unsigned long) << " " << r.size() << " " << (pkt_size/sizeof(unsigned long)-2)/3 << std::endl;
      std::cout << m_pkt[0]/sizeof(unsigned long) << std::endl;
      std::cout << m_pkt[1] << std::endl;
      for(int i=0; i<(pkt_size/sizeof(unsigned long)-2)/3; i++)
      {
          //std::cout << *((unsigned long *)buff+i) << std::endl;
          std::cout << m_pkt[2+3*i] << " " << m_pkt[3*i+3] << " " << m_pkt[3*i+4] << std::endl;
      }
#if 1
      assert((bytes = m_req_comm->send(buff, pkt_size))>=0);    
      while(true)
      {
         void *buf = NULL;
         int bytes = m_resp_comm->receive(&buf);
         std::cout << "I : aka After receive" << std::endl;
         assert(bytes >= 0);
         m_req_comm->freemsg(buf);
         m_trig->Notify(2);
         break;
      }
#endif
      //delete buff;
      //m_trig->Notify(2);
   }
}
