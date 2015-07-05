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

#ifndef _GLOBAL_WINDOW_H
#define _GLOBAL_WINDOW_H

#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <queue>
#include <map>
#include <deque>
#include <utility>

struct Record
{
    //unsigned long queued; //From CUPTI API record
    unsigned long m_g_x;
    unsigned long m_active;
    unsigned long m_idle;
    Record(unsigned long, unsigned long, unsigned long);
};

typedef std::vector<Record> RecordVec;
typedef std::pair<unsigned long, unsigned long> ActiveIdlePair;

struct CompareIdleMin
{
public:
    bool operator()(ActiveIdlePair n1, ActiveIdlePair n2) {
        return n1.second<n2.second;
    }
};

struct CompareActiveMax
{
public:
    bool operator()(ActiveIdlePair n1, ActiveIdlePair n2) {
        return n1.first>n1.first;
    }
};
struct GlobalWindow
{
  RecordVec m_recs;
  std::mutex m_mutex;
  bool m_stale;
  unsigned int m_tenants;
  typedef unsigned long ExperimentalKey;
  typedef std::priority_queue<ActiveIdlePair, std::deque<ActiveIdlePair>, CompareIdleMin> IdleMinQueue; 
  typedef std::priority_queue<ActiveIdlePair, std::deque<ActiveIdlePair>, CompareActiveMax> ActiveMaxQueue; 
  typedef std::map<ExperimentalKey, IdleMinQueue> IdleMap;
  typedef std::map<ExperimentalKey, ActiveMaxQueue> ActiveMap;
  //IdleMap m_tenant_idle_map; //May need a vector for all tenants' idle map
  typedef std::vector<IdleMap> IdleMapVector;
  IdleMapVector m_tenants_idle_map_vector; //for now, only for tenant 0 (e.g., simulation)
  typedef std::vector<ActiveMap> ActiveMapVector;
  ActiveMapVector m_tenants_active_map_vector;
  GlobalWindow();
  ~GlobalWindow();
  void produce(RecordVec r);
  RecordVec consume();
  void update_idle(ExperimentalKey k, unsigned long va, unsigned long vi, unsigned int t);
  void update_active(ExperimentalKey k, unsigned long va, unsigned long vi, unsigned int t);
  unsigned long retrieve_idle(ExperimentalKey k, unsigned int t);
  unsigned long retrieve_active(ExperimentalKey k, unsigned int t);
};

#endif
