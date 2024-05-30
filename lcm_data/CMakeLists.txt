project(listener)

cmake_minimum_required(VERSION 3.5)
find_package(lcm REQUIRED)
include(${LCM_USE_FILE})

include_directories(../lcm-types/cpp)

aux_source_directory(. SOURCE)
add_executable(listener ${SOURCE})
target_link_libraries(${PROJECT_NAME}
  lcm
)
