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

#include "Communicator.h"

Communicator::Communicator(std::string& url, Purpose recv_or_send)
   :m_url(url)
{
    if(recv_or_send == SENDER)
    {
       assert((m_sock = nn_socket(AF_SP, NN_PUSH)) >= 0);
       std::cout << "SENDER " << url.c_str() << " " << m_sock << std::endl;
    }
    else if(recv_or_send == RECEIVER)
    {
       assert((m_sock = nn_socket(AF_SP, NN_PULL)) >= 0);
       std::cout << "RECEIVER " << url.c_str() << " " << m_sock << std::endl;
    }
    else
    {
       assert(0 && "Has to be a SENDER or a receiver!");
    }
}

Communicator::~Communicator()
{
    /*if(m_sock == 1)
    {
      std::cout << "SHUTDOWN " << m_sock << std::endl;
      assert(nn_shutdown(m_sock, 0) >= 0);
    }*/
}

int Communicator::connect()
{
    int ret = nn_connect(m_sock, m_url.c_str());
    assert(ret >= 0);
    std::cout << "CONNECT " << m_url.c_str() << " " << m_sock << std::endl;
    
    return ret;
}

int Communicator::bind()
{
    int ret = nn_bind (m_sock, m_url.c_str());
    assert(ret >= 0);
    std::cout << "BIND " << m_url.c_str() << " " << m_sock << std::endl;

    return ret;
}

int Communicator::receive(void *buf)
{
   std::cout << "RECEIVE start " << m_sock << std::endl;
   int bytes = nn_recv(m_sock, buf, NN_MSG, 0);
   assert(bytes >= 0);
   std::cout << "RECEIVE end " << m_sock << " " << bytes << std::endl;
   return bytes;
}

int Communicator::freemsg(void *buf)
{
   int ret = nn_freemsg(buf);
   assert(ret >= 0);
   return ret;
}

int Communicator::send(const void *buf, size_t size)
{
   std::cout << "SEND start " << m_sock << std::endl;
   int bytes = nn_send(m_sock, buf, size, 0);
   assert(bytes >= 0);
   std::cout << "SEND end " << m_sock << " " << bytes << std::endl;
   return bytes;
}
