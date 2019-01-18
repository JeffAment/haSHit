cmake_minimum_required(VERSION 3.13)
project(haSHit)

set(CMAKE_CXX_STANDARD 14)
link_libraries(crypto) #for openssl/md5.h

add_executable(haSHit main.cpp CommandBuilderAndExecutor.cpp CommandBuilderAndExecutor.h main.h)

# For Boost
find_package(Boost COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")
target_link_libraries(haSHit Boost::filesystem)
# End Boost