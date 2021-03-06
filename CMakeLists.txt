cmake_minimum_required(VERSION 2.8)

SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

add_library(cupti SHARED IMPORTED)
set_property(TARGET cupti PROPERTY IMPORTED_LOCATION "/usr/local/cuda/extras/CUPTI/lib64/libcupti.so")

add_library(nanomsg SHARED IMPORTED)
set_property(TARGET nanomsg PROPERTY IMPORTED_LOCATION "./nanomsg/libnanomsg.so")

project(propreact)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(include ../include /usr/include /usr/local/cuda/extras/CUPTI/include /usr/include /usr/local/cuda/include ./nanomsg/include)

add_library(propreact SHARED src/Xrush.cpp src/CuptiProfiler.cpp src/Interposer.cpp src/Predictor.cpp src/Window.cpp src/Trigger.cpp src/Communicator.cpp src/GlobalWindow.cpp)

target_link_libraries(propreact cupti)
target_link_libraries(propreact nanomsg)

project(propreact-backend)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_BUILD_TYPE "Debug")

include_directories(include ../include /usr/include /usr/local/cuda/include ./nanomsg/include)

add_executable(propreact-backend src/BackendDriver.cpp src/Communicator.cpp src/InterposerBackend.cpp src/RequestPool.cpp src/GlobalWindow.cpp src/Arbiter.cpp)

target_link_libraries(propreact-backend nanomsg)
