cmake_minimum_required(VERSION 3.6)
project(sipProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp  src/Markup.cpp  src/XEvent.h src/easylogging++.cc src/easylogging++.h src/config.cpp src/config.h src/eventHandle.cpp src/eventHandle.h)
add_executable(sipProject ${SOURCE_FILES})
target_link_libraries(sipProject eXosip2 osip2 osipparser2)
TARGET_LINK_LIBRARIES(sipProject  pthread)