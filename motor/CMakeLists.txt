include_directories(
        ../thirdparty/SOEM/soem
        ../thirdparty/SOEM/osal
        ../thirdparty/SOEM/osal/linux
        ../thirdparty/SOEM/oshw/linux
        ./
)

set(SOURCES motor_control.c
        motor_control.h
        math_ops.c
        math_ops.h
        transmit.c
        transmit.h
        config.h
        config_main.h
        config_main.cpp
        Loop.hpp
)
add_library(motor ${SOURCES})
target_link_libraries(motor soem)
