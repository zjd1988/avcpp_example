# - Try to find ffmpeg libraries
#     (libavcodec, libavformat, libavutil, libswscale)
# Once done this will define
#
# FFMPEG_FOUND - system has ffmpeg or libav
# FFMPEG_INCLUDE_DIRS - the ffmpeg include directory
# FFMPEG_LIBRARIES - Link these to use ffmpeg
#

if (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)
    # in cache already
    set(FFMPEG_FOUND TRUE)
else (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)

    find_path(FFMPEG_AVCODEC_INCLUDE_DIR NAMES libavcodec/avcodec.h
        PATHS ${_FFMPEG_AVCODEC_INCLUDE_DIRS} ${FFMPEG_SDK_DIR}/include /usr/include /usr/local/include /opt/local/include /sw/include
        PATH_SUFFIXES ffmpeg libav)

    find_library(FFMPEG_LIBAVCODEC NAMES avcodec
        PATHS ${_FFMPEG_AVCODEC_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBAVFORMAT NAMES avformat
        PATHS ${_FFMPEG_AVFORMAT_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBAVUTIL NAMES avutil
        PATHS ${_FFMPEG_AVUTIL_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBSWSCALE NAMES swscale
        PATHS ${_FFMPEG_SWSCALE_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBSWRESAMPLE NAMES swresample
        PATHS ${_FFMPEG_SWRESAMPLE_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBAVDEVICE NAMES avdevice
        PATHS ${_FFMPEG_AVDEVICE_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    find_library(FFMPEG_LIBAVFILTER NAMES avfilter
        PATHS ${_FFMPEG_AVFILTER_LIBRARY_DIRS} ${FFMPEG_SDK_DIR}/lib /usr/lib /usr/local/lib /opt/local/lib)

    if (FFMPEG_LIBAVCODEC AND FFMPEG_LIBAVFORMAT)
        set(FFMPEG_FOUND TRUE)
    endif()

  if (FFMPEG_FOUND)
        set(FFMPEG_INCLUDE_DIRS ${FFMPEG_AVCODEC_INCLUDE_DIR})

        set(FFMPEG_LIBRARIES ${FFMPEG_LIBAVCODEC}
                             ${FFMPEG_LIBAVFORMAT}
                             ${FFMPEG_LIBAVUTIL}
                             ${FFMPEG_LIBSWSCALE}
                             ${FFMPEG_LIBSWRESAMPLE}
                             ${FFMPEG_LIBAVDEVICE}
                             ${FFMPEG_LIBAVFILTER})

        if (NOT FFMPEG_FIND_QUIETLY)
            message(STATUS "Found FFMPEG or Libav: ${FFMPEG_LIBRARIES}, ${FFMPEG_INCLUDE_DIRS}")
        endif (NOT FFMPEG_FIND_QUIETLY)
    else (FFMPEG_FOUND)
        if (FFMPEG_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find libavcodec or libavformat or libavutil")
        endif (FFMPEG_FIND_REQUIRED)
    endif (FFMPEG_FOUND)

endif (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIRS)