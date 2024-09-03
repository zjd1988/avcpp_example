cmake_minimum_required(VERSION 3.10)
project(avcpp_example)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message("build for ${CMAKE_BUILD_TYPE}")

# set default target platform
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(TARGET_PLATFORM x64-win)
endif()
if(NOT TARGET_PLATFORM)
    set(TARGET_PLATFORM x64)
endif()

message(STATUS "CMAKE_BUILD_TYPE:            ${CMAKE_BUILD_TYPE}")
message(STATUS "TARGET_PLATFORM:             ${TARGET_PLATFORM}")
message(STATUS "CMAKE_SYSTEM_NAME:           ${CMAKE_SYSTEM_NAME}")

# set runtime opt when msvc
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(MSVC_RUNTIME_FLAGS
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL)
    foreach(flag ${MSVC_RUNTIME_FLAGS})
        string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
        set(${flag} "${${flag}}" CACHE STRING "msvc compiler flags" FORCE)
    endforeach()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions(-DNOMINMAX)
else()
    set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} dl)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    else()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
else()
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    else()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    endif()
endif()
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")

# find ffmpeg
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    include(cmake/find_ffmpeg_windows.cmake)
    find_FFmpeg(${FFMPEG_SDK_DIR})
else()
    include(cmake/find_ffmpeg_linux.cmake)
endif()
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${FFMPEG_INCLUDE_DIRS})
set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} ${FFMPEG_LIBRARIES})

# spdlog
set(SPDLOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/spdlog/include)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${SPDLOG_INCLUDE_DIRS})

# cxxopts
set(CXXOPTS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/cxxopts/include/)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${CXXOPTS_INCLUDE_DIRS})

# ghc filesystem
set(GHC_FILESYSTEM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/filesystem/include)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${GHC_FILESYSTEM_INCLUDE_DIRS})

# OpenCV 4..0
if(USE_OPENCV_MOBILE)
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        add_definitions(-DOpenCV_STATIC)
        set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/opencv-mobile/${TARGET_PLATFORM}/x64/vc14/staticlib)
    else()
        set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/opencv-mobile/${TARGET_PLATFORM}/lib/cmake/opencv4)
    endif()
    find_package(OpenCV REQUIRED)
    set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} ${OpenCV_LIBS})
    else()
        set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} ${OpenCV_LIBS} gomp pthread)
    endif()
    message(STATUS "opencv mobile libs: ${OpenCV_LIBS}")
    message(STATUS "opencv mobile include dirs: ${OpenCV_INCLUDE_DIRS}")
else()
    set(OPENCV_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/opencv/${TARGET_PLATFORM}/include)
    set(OPENCV_LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/opencv/${TARGET_PLATFORM}/lib)
    set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})
    set(PROJECT_LINK_DIRS ${PROJECT_LINK_DIRS} ${OPENCV_LIB_DIRS})
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
        set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} opencv_world490)
    else()
        set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} opencv_world)
    endif()
    message(STATUS "opencv world include dirs: ${OPENCV_INCLUDE_DIRS}")
    message(STATUS "opencv world lib dirs: ${OPENCV_LIB_DIRS}")
endif()

# use avcpp when ffmpeg used
set(AVCPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/avcpp/${TARGET_PLATFORM}/include)
set(AVCPP_LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/avcpp/${TARGET_PLATFORM}/lib)
set(PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${AVCPP_INCLUDE_DIRS})
set(PROJECT_LINK_DIRS ${PROJECT_LINK_DIRS} ${AVCPP_LIB_DIRS})
set(PROJECT_LINK_LIBS ${PROJECT_LINK_LIBS} avcpp)

# common src files
file(GLOB COMMON_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp)

# config src files
file(GLOB CONFIG_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/config/*.cpp)

# metadata src files
file(GLOB METADATA_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/metadata/*.cpp)

# reader src files
file(GLOB READER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/reader/*.cpp)

# final project src files
set(PROJECT_SRC_FILES ${COMMON_SRC_FILES}
                      ${CONFIG_SRC_FILES}
                      ${METADATA_SRC_FILES}
                      ${READER_SRC_FILES})

# target so
add_library(${CMAKE_PROJECT_NAME} SHARED ${PROJECT_SRC_FILES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PROJECT_INCLUDE_DIRS})
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_LINK_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_LINK_LIBS})

# TARGET video_reader
add_executable(video_reader ${CMAKE_CURRENT_SOURCE_DIR}/example/video_reader.cpp)
target_include_directories(video_reader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PROJECT_INCLUDE_DIRS})

target_link_directories(video_reader PRIVATE ${PROJECT_LINK_DIRS})
target_link_libraries(video_reader PRIVATE ${CMAKE_PROJECT_NAME} ${PROJECT_LINK_LIBS})
