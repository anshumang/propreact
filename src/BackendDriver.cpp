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

#include "RequestPool.h"
#include "InterposerBackend.h"
#include "GlobalWindow.h"
#include "Arbiter.h"

int main(int argc, const char **argv)
{
    RequestPool reqpool;
    GlobalWindow gwin;
    Arbiter arb(&reqpool, &gwin);
    arb.start();
    InterposerBackend tenant0("ipc:///tmp/propreact0b.ipc", "ipc:///tmp/propreact0a.ipc", 0, &reqpool, &gwin);
    tenant0.start();
    InterposerBackend tenant1("ipc:///tmp/propreact1b.ipc", "ipc:///tmp/propreact1a.ipc", 1, &reqpool, &gwin);
    tenant1.start();
    tenant0.join();
    return 0;
}

