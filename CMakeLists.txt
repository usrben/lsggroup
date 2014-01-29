cmake_minimum_required(VERSION 2.8)
project(lsggroup)

add_definitions(-std=c++11 -ggdb -O0 -Wreturn-type)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.55.0 COMPONENTS filesystem thread system program_options) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS -DBOOST_FILESYSTEM_NO_DEPRECATED)
    add_executable(lsggroup main.cpp)
    add_subdirectory(google)
    target_link_libraries(lsggroup usrben_google)
endif()


