cmake_minimum_required(VERSION 3.5.0)

project(imu)

# set(CMAKE_EXE_LINKER_FLAGS " -no-pie")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

aux_source_directory(. DIR_IMU_SRC)

add_library(imu ${DIR_IMU_SRC})

include_directories(${PROJECT_SOURCE_DIR}/lib/xspublic)

link_directories(${PROJECT_SOURCE_DIR}/lib/xspublic/xscommon)
link_directories(${PROJECT_SOURCE_DIR}/lib/xspublic/xscontroller)
link_directories(${PROJECT_SOURCE_DIR}/lib/xspublic/xstypes)

target_link_libraries(
	imu	
	xscontroller
	xscommon
	xstypes
	rt
	dl
)