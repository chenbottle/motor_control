cmake_minimum_required(VERSION 3.5)
project(MasterStack)

add_subdirectory(thirdparty)
add_subdirectory(motor)
set(CMAKE_EXE_LINKER_FLAGS " -no-pie")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${Boost_INCLUDE_DIR})
include_directories(
        thirdparty/SOEM/soem
        thirdparty/SOEM/osal
        thirdparty/SOEM/osal/linux
        thirdparty/SOEM/oshw/linux
        thirdparty/XSense_Raw
        thirdparty/XSense_Raw/lib/xspublic
        motor
        ./src
        ./
)

set(CMAKE_CXX_STANDARD 17)

link_directories(thirdparty/XSense_Raw/lib/xspublic/xscommon)
link_directories(thirdparty/XSense_Raw/lib/xspublic/xscontroller)
link_directories(thirdparty/XSense_Raw/lib/xspublic/xstypes)

aux_source_directory(./src exec_file)
add_executable(master_stack_test ${exec_file})
target_link_libraries(master_stack_test 
    -pthread   
    motor 
    imu
    soem)
