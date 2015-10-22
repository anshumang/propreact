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

#ifndef _INTERPOSER_BACKEND_H
#define _INTERPOSER_BACKEND_H

#include <thread>
#include <iostream>
#include <sys/time.h>
#include "Communicator.h"
#include "RequestPool.h"
#include "GlobalWindow.h"

struct InterposerBackend
{
    InterposerBackend(std::string req_url, std::string resp_url, int tenant_id, RequestPool *req_pool, GlobalWindow *gwin);
    ~InterposerBackend();
    void start();
    void join();
    void process();
    std::thread m_thr;
    Communicator *m_req_comm, *m_resp_comm;
    RequestPool *m_req_pool;
    int m_tenant_id;
    GlobalWindow *m_gwin;
};

#endif
