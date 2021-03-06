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

#ifndef _CUPTI_PROFILER_H
#define _CUPTI_PROFILER_H

#include <algorithm>
#include <utility>
#include <vector>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <cupti.h>
#include "Trigger.h"
#include "Window.h"
#include "GlobalWindow.h"

#define CUPTI_CALL(call)                                                \
  do {                                                                  \
    CUptiResult _status = call;                                         \
    if (_status != CUPTI_SUCCESS) {                                     \
      const char *errstr;                                               \
      cuptiGetResultString(_status, &errstr);                           \
      std::cerr << __FILE__ << ":" << __LINE__ << ":" << "error: function " << #call  << "failed with error " << errstr << std::endl; \
                       \
      std::exit(-1);                                                         \
    }                                                                   \
  } while (0)

#define BUF_SIZE (32 * 1024)
#define ALIGN_SIZE (8)
#define ALIGN_BUFFER(buffer, align)                                            \
  (((uintptr_t) (buffer) & ((align)-1)) ? ((buffer) + (align) - ((uintptr_t) (buffer) & ((align)-1))) : (buffer))

struct CuptiProfiler
{
   typedef std::pair<unsigned long, unsigned long> CuptiTuple; 
   typedef std::vector< CuptiTuple > CuptiTupleVector;
   CuptiTupleVector m_tup_vec;
   CuptiTupleVector m_tup_vec_raw;
   typedef Grid ExperimentalKey;
   typedef std::pair<ExperimentalKey, CuptiTuple> ExpKeyCuptiTuple;
   typedef std::vector< ExpKeyCuptiTuple > ExpKeyCuptiTupleVector;
   ExpKeyCuptiTupleVector m_keyval_vec_raw, m_keyval_vec;
   Window* m_win;
   GlobalWindow* m_gwin;
   unsigned long m_start, m_last, m_last_api;
   unsigned long m_tot_records, m_curr_records, m_tot_disjoint_records;
   size_t m_cupti_buffer_size, m_cupti_buffer_pool_limit;
   friend void CUPTIAPI return_buffer(CUcontext ctx, uint32_t stream_id, uint8_t *buffer, size_t size, size_t valid_size);
   CuptiProfiler(Window *, GlobalWindow *);
   ~CuptiProfiler();
   void read();
private:
   void insert(CUpti_Activity *);
   bool cupti_tuple_compare(CuptiTuple& first, CuptiTuple& second);
   void process();
};

#endif
