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

#include "Interposer.h"

Interposer *p_interposer = NULL;

Interposer::Interposer(Trigger* trig)
  : m_last(0), m_trig(trig)
{
  std::cout << "Interposer CTOR" << std::endl;
}

Interposer::~Interposer()
{

}

void Interposer::launch(unsigned long gridX, unsigned long gridY, unsigned long gridZ, unsigned long blockX, unsigned long blockY, unsigned long blockZ)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long curr = now.tv_sec*1000000 + now.tv_usec;
    //std::cout << curr - m_last << " " << gridX << " " << gridY << " " << gridZ << " " << blockX << " " << blockY << " " << blockZ << std::endl;
    m_last = curr;
    Grid g(gridX, gridY, gridZ);
    //std::cout << "I " << g.x << std::endl;
    m_trig->WriteData(g);
    m_trig->Notify(1);
    m_trig->Wait(2);
}
