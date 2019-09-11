KOKKOS_DIRECTORY=/Users/dzpolia/install/kokkos
CXX=g++
CXXFLAGS=-O3 -std=c++11 -g
SHARED_CXXFLAGS=-shared -fPIC
#KOKKOS_FLAGS=-I$(KOKKOS_DIRECTORY)/include -L$(KOKKOS_DIRECTORY)/lib -lkokkos
KOKKOS_FLAGS=-I$(KOKKOS_DIRECTORY)/include -L$(KOKKOS_DIRECTORY)/lib -lkokkos

all: kp_sanitizer_utility.so madebad


kp_sanitizer_utility.so: kp_kernel_timer.cpp kp_kernel_info.h
	$(CXX) $(SHARED_CXXFLAGS) $(CXXFLAGS) -o $@ kp_kernel_timer.cpp

madebad: madebad.cpp kp_sanitizer_utility.so
	$(CXX) $(CXXFLAGS) $(KOKKOS_FLAGS) -fsanitize=address -fsanitize-recover=address madebad.cpp -o madebad


.PHONY: test

test:
	KOKKOS_PROFILE_LIBRARY=./kp_sanitizer_utility.so  ./madebad

clean:
	rm *.so kp_reader
