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

#ifndef _COMMUNICATOR_H
#define _COMMUNICATOR_H

#include <string>
#include <cassert>
#include <iostream>
#include <nn.h>
#include <pipeline.h>

enum Purpose
{
   SENDER,
   RECEIVER
};

struct Communicator
{
   Communicator(std::string& url, Purpose recv_or_send);
   ~Communicator();
   int connect();
   int bind();
   int send(const void *, size_t);
   int receive(void *);
   int freemsg(void *);
   int m_sock;
   std::string m_url;
};

#endif
