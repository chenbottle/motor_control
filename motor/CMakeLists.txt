include_directories(
        ../thirdparty/soem
        ../thirdparty/osal
        ../thirdparty/osal/linux
        ../thirdparty/oshw/linux
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
