cmake_minimum_required(VERSION 2.8)
project( video_ui_UI)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/exe)


# Widgets finds its own dependencies (Qt and opencv).
find_package(Qt5Widgets REQUIRED)
find_package( OpenCV REQUIRED )
find_package(FFmpeg COMPONENTS AVCODEC AVFORMAT SWSCALE AVUTIL AVDEVICE REQUIRED)

if(FFMPEG_FOUND)
    #  FFMPEG_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
    #  FFMPEG_LIBRARIES     - Link these to use the required ffmpeg components.
    #  FFMPEG_DEFINITIONS   - Compiler switches required for using the required ffmpeg components.
    message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS} ")
    message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES} ")
    message("FFMPEG_DEFINITIONS = ${FFMPEG_DEFINITIONS} ")
    include_directories(${FFMPEG_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "FFMPEG not found")
endif()

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Include folders
include_directories(${PROJECT_SOURCE_DIR})

# include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/include )

include_directories(${CMAKE_BINARY_DIR})
# link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/lib )

add_executable( DisplayImage readImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )

add_executable( video_read video_read_test.cpp )
target_link_libraries( video_read ${OpenCV_LIBS} )

add_executable( graphcut graphcut_example.cpp )
target_link_libraries( graphcut ${OpenCV_LIBS} )

# Tell CMake to create the helloworld executable
add_executable(gui
    WIN32
    UI/main.cpp
    UI/CenterWidget.cpp
    UI/CenterWidget.h
    UI/PaintWidget.cpp
    UI/PaintWidget.h
    UI/VideoCut.cpp
    UI/VideoCut.h
    UI/VideoDecoder.cpp
    UI/VideoDecoder.h
    )

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(gui
    ${Qt5Widgets_LIBRARIES}
    ${OpenCV_LIBS}
    ${ffmpeg_libs}
    ${FFMPEG_LIBRARIES}
    )

add_executable( cmd_test
    UI/cmd_test.cpp
    UI/CenterWidget.cpp
    UI/CenterWidget.h
    UI/PaintWidget.cpp
    UI/PaintWidget.h
    UI/VideoCut.cpp
    UI/VideoCut.h
    UI/VideoDecoder.cpp
    UI/VideoDecoder.h
)
target_link_libraries( cmd_test
    ${Qt5Widgets_LIBRARIES}
    ${OpenCV_LIBS}
    ${ffmpeg_libs}
    ${FFMPEG_LIBRARIES}
 )
