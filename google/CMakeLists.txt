add_library(usrben_google oauth2.cpp directory_members.cpp contacts.cpp)
target_link_libraries(usrben_google ${Boost_LIBRARIES} cppnetlib-uri cppnetlib-client-connections ssl crypto pthread)
