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

#ifndef _REQUEST_POOL_H
#define _REQUEST_POOL_H

#include <utility>
#include <map>
#include <array>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <iostream>

typedef unsigned long ExperimentalKey;

struct RequestPool
{
    RequestPool();
    ~RequestPool();
    unsigned int m_num_tenants;
    typedef std::pair<unsigned int, unsigned long> TenantIdExperimentalKeyPair;
    typedef std::map<unsigned int, unsigned long> TenantIdExperimentalKeyMap; 
    std::array<bool, 2> m_tenant_ready;
    std::array<std::mutex, 2> m_tenant_mutex;
    std::array<std::condition_variable, 2> m_tenant_notify;
    std::mutex m_mutex; 
    TenantIdExperimentalKeyMap m_tenant_requests;
    void update(TenantIdExperimentalKeyPair p);
    ExperimentalKey retrieve(unsigned int t);
    void wait(int);
    void release(int);
};

#endif
