cmake_minimum_required(VERSION 3.13)
project(haSHit)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp CommandBuilderAndExecutor.cpp)

find_package(OpenSSL COMPONENTS Crypto REQUIRED)
if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "Could not find OpenSSL!")
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} OpenSSL::Crypto)