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

struct Record
{
    //unsigned long queued; //From CUPTI API record
    unsigned long m_g_x;
    unsigned long m_active;
    unsigned long m_idle;
    Record(unsigned long, unsigned long, unsigned long);
};

typedef std::vector<Record> RecordVec;

struct GlobalWindow
{
  RecordVec m_recs;
  std::mutex m_mutex;
  bool m_stale;
  GlobalWindow();
  ~GlobalWindow();
  void produce(RecordVec r);
  RecordVec consume();
};

#endif
