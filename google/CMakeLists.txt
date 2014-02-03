add_library(usrben_google oauth2.cpp directory/members.cpp directory/directory.cpp)
target_link_libraries(usrben_google ${Boost_LIBRARIES} cppnetlib-uri cppnetlib-client-connections ssl crypto pthread)
