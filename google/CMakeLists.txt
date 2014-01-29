#add_definitions(-std=c++11 -ggdb -O0 -Wreturn-type)
#
#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#
#find_package(Boost 1.55.0 COMPONENTS filesystem thread system ) 
#
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS}) 
#    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS -DBOOST_FILESYSTEM_NO_DEPRECATED)
#    add_library(usrben_google oauth2.cpp directory/members.cpp)
#    target_link_libraries(usrben_google ${Boost_LIBRARIES} ssl crypto pthread)
#endif()

add_library(usrben_google oauth2.cpp directory/members.cpp)
target_link_libraries(usrben_google ${Boost_LIBRARIES} cppnetlib-uri cppnetlib-client-connections ssl crypto pthread)
