#include <cstdio>
#include <inttypes.h>
#include <vector>
#include <string>
#include<cuda_runtime.h>


struct SpaceHandle {
  char name[64];
};

extern "C" void kokkosp_init_library(const int loadSeq,
	const uint64_t interfaceVer,
	const uint32_t devInfoCount,
	void* deviceInfo) {

	printf("KokkosP: Debug Synchronizer loaded (sequence is %d, version: %llu)\n", loadSeq, interfaceVer);
}

extern "C" void kokkosp_finalize_library() {
	printf("KokkosP: Kokkos library finalization called.\n");
}

extern "C" void kokkosp_begin_parallel_for(const char* name, const uint32_t devID, uint64_t* kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_begin_parallel_scan(const char* name, const uint32_t devID, uint64_t* kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_end_parallel_scan(const uint64_t kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_begin_parallel_reduce(const char* name, const uint32_t devID, uint64_t* kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  cudaDeviceSynchronize();
}

extern "C" void kokkosp_begin_deep_copy(
    SpaceHandle dst_handle, const char* dst_name, const void* dst_ptr,
    SpaceHandle src_handle, const char* src_name, const void* src_ptr,
    uint64_t size) {
  cudaDeviceSynchronize();
}
