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

#ifndef _ARBITER_H
#define _ARBITER_H

#include <thread>
#include <chrono>
#include <map>
#include <set>
#include <utility>
#include "RequestPool.h"
#include "GlobalWindow.h"

struct Arbiter
{
  int m_num_tenants;
  Arbiter(RequestPool *, GlobalWindow *);
  ~Arbiter();
  void start();
  void join();
  void process();
  std::thread m_thr;
  RequestPool *m_req_pool;
  GlobalWindow *m_gwin;
};

#endif
