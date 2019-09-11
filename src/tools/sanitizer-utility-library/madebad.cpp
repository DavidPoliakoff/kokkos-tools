#include <Kokkos_Core.hpp>
#include <iostream>
#include <Kokkos_LogicalMemorySpace.hpp>

constexpr const char* MirrorSpaceName = "CudaSim";

template<typename ViewType, typename... PolicyTraits>
ViewType makeView(const char* name, int size){
  ViewType view(Kokkos::ViewAllocateWithoutInitializing(name), size);
  Kokkos::RangePolicy<PolicyTraits...>  pol(typename ViewType::execution_space(),0,size);
  Kokkos::parallel_for("HackedViewInitialiation",pol,KOKKOS_LAMBDA(const int index){
    view[index] = 0;
  }); 
  return view;
} 

using namespace Kokkos;
int main(int argc, char* argv[]){
  
  using ScalarType = double;

  Kokkos::RangePolicy<Kokkos::Impl::DeviceId<1>> gpu_policy(Kokkos::Serial(), 0, 8) ;
  Kokkos::RangePolicy<Kokkos::Impl::DeviceId<0>> host_policy(Kokkos::Serial(), 0, 8) ;

  using ViewType = Kokkos::View<ScalarType*>;
  using AltType = Kokkos::View<ScalarType*, Kokkos::LogicalMemorySpace<&MirrorSpaceName,Kokkos::HostSpace>>;
  Kokkos::initialize (argc, argv);
  ViewType out_mat = makeView<ViewType, Kokkos::Impl::DeviceId<0>>("size_8", 8); 
  //AltType this_is_fine = makeView<AltType, Kokkos::Impl::DeviceId<1>>("CudaSim",8);

  Kokkos::parallel_for("host_kernel_one", host_policy, KOKKOS_LAMBDA (const int index){
    out_mat(index) = index * index;
    //std::cout << index << " " << out_mat(index) << std::endl;
  }); 
  //Kokkos::deep_copy(/** */);
  Kokkos::parallel_for("device_kernel_one", gpu_policy, KOKKOS_LAMBDA (const int index){
    out_mat(index) = index * index;
    std::cout << index << " " << out_mat(index) << std::endl;
  }); 

  Kokkos::parallel_for("host_kernel_two", host_policy, KOKKOS_LAMBDA (const int index){
    out_mat(index) = index * index;
    std::cout << index << " " << out_mat(index) << std::endl;
  }); 

}
