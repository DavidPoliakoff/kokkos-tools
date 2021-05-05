//@HEADER
// ************************************************************************
//
//                        Kokkos v. 3.0
//       Copyright (2020) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY NTESS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NTESS OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact David Poliakoff (dzpolia@sandia.gov)
//
// ************************************************************************
//@HEADER

#include <stdio.h>
#include <inttypes.h>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <stack>
#include <string>
#include <iostream>
#include <craypat.h>

#define ALLOWS_INT64_LOOKUP

using craypat_range_identifier = int;
using craypat_region_stack_identifier = int;

static std::unordered_map<const char*, craypat_range_identifier> range_map;
static std::stack<craypat_region_stack_identifier> region_range_stack;
craypat_range_identifier get_new_range_identifier(const char *name) {
    // TODO: insert real implementation here
    //static craypat_region_stack_identifier id;
    //auto fnd = range_map.find(name);
    //if(fnd==range_map.end()){
    //  fnd->second = ++id;
    //}
    PAT_MAGICREGION_BEGIN(name);
    return 0;
    //return fnd->second;
}

void end_the_thing(craypat_range_identifier id) {
    // TODO: insert real implementation here
    PAT_MAGICREGION_END();
}

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
    printf("-----------------------------------------------------------\n");
    printf("KokkosP: CrayPAT Analyzer Connector (sequence is %d, version: %llu)\n",
           loadSeq, interfaceVer);
    printf("-----------------------------------------------------------\n");

}

extern "C" void kokkosp_finalize_library() {
    printf("-----------------------------------------------------------\n");
    printf("KokkosP: Finalization of CrayPAT Connector. Complete.\n");
    printf("-----------------------------------------------------------\n");

}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
#ifdef ALLOWS_INT64_LOOKUP
    *kID = get_new_range_identifier(name);
#else
    range_map[kID] = get_new_range_identifier(name);
#endif
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {

#ifdef ALLOWS_INT64_LOOKUP
    end_the_thing(kID);
#else
    end_the_thing(range_map[kID]);
#endif

}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
#ifdef ALLOWS_INT64_LOOKUP
    *kID = get_new_range_identifier(name);
#else
    range_map[kID] = get_new_range_identifier(name);
#endif
}

extern "C" void kokkosp_end_parallel_scan(const uint64_t kID) {
#ifdef ALLOWS_INT64_LOOKUP
    end_the_thing(kID);
#else
    end_the_thing(range_map[kID]);
#endif
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
#ifdef ALLOWS_INT64_LOOKUP
    *kID = get_new_range_identifier(name);
#else
    range_map[kID] = get_new_range_identifier(name);
#endif
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
#ifdef ALLOWS_INT64_LOOKUP
    end_the_thing(kID);
#else
    end_the_thing(range_map[kID]);
#endif
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
    region_range_stack.push(get_new_range_identifier(regionName));
}

extern "C" void kokkosp_pop_profile_region() {
    if (region_range_stack.empty()) {
        std::cerr
                << "KokkosP: Error - popped region with no active regions pushed. "
                << std::endl;
    } else {
        auto stack_top = region_range_stack.top();
        end_the_thing(stack_top);
        region_range_stack.pop();
    }
}
