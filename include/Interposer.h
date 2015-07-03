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

#ifndef _INTERPOSER_H
#define _INTERPOSER_H

#include <cstddef>
#include <iostream>
#include <sys/time.h>
#include "Trigger.h"

struct Interposer
{
   unsigned long m_last;
   Trigger* m_trig;
   Interposer(/*std::string req_url, std::string resp_url,*/ Trigger* trig);
   ~Interposer();
   void launch(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
   //Communicator *m_req_comm, *m_resp_comm;
};

#endif

