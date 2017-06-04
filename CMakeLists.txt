cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
add_executable( exe/DisplayImage readImage.cpp )
target_link_libraries( exe/DisplayImage ${OpenCV_LIBS} )

add_executable( exe/graphcut graphcut_example.cpp )
target_link_libraries( exe/graphcut ${OpenCV_LIBS} )

# OBJS = preprocess
#
# CC	= g++
#
# CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/Cellar/opencv/2.4.13.2/include/
# export CPLUS_INCLUDE_PATH
#
# # CFLAGS	= -g -Wall -ansi -pedantic
# CFLAGS	= $(pkg-config --cflags --libs opencv)
#
# all:  preprocess preprocess.o
#
# preprocess:
# 	$(CC) $(CFLAGS) preprocess.cpp -o preprocess.o
#
#
# preprocess.o:
# 	$(CC) $(CFLAGS) -c preprocess.cpp -o preprocess.o
# # g++ $(pkg-config --cflags --libs opencv) preprocess.cpp -o preprocess
