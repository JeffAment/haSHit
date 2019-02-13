cmake_minimum_required(VERSION 3.13)
project(haSHit)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp CommandBuilderAndExecutor.cpp)

set(Boost_USE_STATIC_LIBS NO) # Set this to YES or NO depending on needs
set(Boost_USE_MULTITHREADED ON)
unset(Boost_INCLUDE_DIR CACHE)
unset(Boost_LIBRARY_DIRS CACHE)
find_package(Boost COMPONENTS filesystem REQUIRED)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
else()
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(OPENSSL_USE_STATIC_LIBS FALSE) # Set this to TRUE or FALSE depending on needs
unset(OPENSSL_INCLUDE_DIR CACHE)
unset(OPENSSL_LIBRARIES CACHE)
find_package(OpenSSL COMPONENTS Crypto REQUIRED)

if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "Could not find OpenSSL!")
else()
    message(STATUS "OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OPENSSL_LIBRARIES: ${OPENSSL_LIBRARIES}")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})