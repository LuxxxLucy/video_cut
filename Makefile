# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lucy/Desktop/doing/Graphics/video_cut

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lucy/Desktop/doing/Graphics/video_cut

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.8.1/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.8.1/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/lucy/Desktop/doing/Graphics/video_cut/CMakeFiles /Users/lucy/Desktop/doing/Graphics/video_cut/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/lucy/Desktop/doing/Graphics/video_cut/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named exe/graphcut

# Build rule for target.
exe/graphcut: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 exe/graphcut
.PHONY : exe/graphcut

# fast build rule for target.
exe/graphcut/fast:
	$(MAKE) -f CMakeFiles/exe/graphcut.dir/build.make CMakeFiles/exe/graphcut.dir/build
.PHONY : exe/graphcut/fast

#=============================================================================
# Target rules for targets named exe/DisplayImage

# Build rule for target.
exe/DisplayImage: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 exe/DisplayImage
.PHONY : exe/DisplayImage

# fast build rule for target.
exe/DisplayImage/fast:
	$(MAKE) -f CMakeFiles/exe/DisplayImage.dir/build.make CMakeFiles/exe/DisplayImage.dir/build
.PHONY : exe/DisplayImage/fast

graphcut_example.o: graphcut_example.cpp.o

.PHONY : graphcut_example.o

# target to build an object file
graphcut_example.cpp.o:
	$(MAKE) -f CMakeFiles/exe/graphcut.dir/build.make CMakeFiles/exe/graphcut.dir/graphcut_example.cpp.o
.PHONY : graphcut_example.cpp.o

graphcut_example.i: graphcut_example.cpp.i

.PHONY : graphcut_example.i

# target to preprocess a source file
graphcut_example.cpp.i:
	$(MAKE) -f CMakeFiles/exe/graphcut.dir/build.make CMakeFiles/exe/graphcut.dir/graphcut_example.cpp.i
.PHONY : graphcut_example.cpp.i

graphcut_example.s: graphcut_example.cpp.s

.PHONY : graphcut_example.s

# target to generate assembly for a file
graphcut_example.cpp.s:
	$(MAKE) -f CMakeFiles/exe/graphcut.dir/build.make CMakeFiles/exe/graphcut.dir/graphcut_example.cpp.s
.PHONY : graphcut_example.cpp.s

preprocess.o: preprocess.cpp.o

.PHONY : preprocess.o

# target to build an object file
preprocess.cpp.o:
	$(MAKE) -f CMakeFiles/exe/DisplayImage.dir/build.make CMakeFiles/exe/DisplayImage.dir/preprocess.cpp.o
.PHONY : preprocess.cpp.o

preprocess.i: preprocess.cpp.i

.PHONY : preprocess.i

# target to preprocess a source file
preprocess.cpp.i:
	$(MAKE) -f CMakeFiles/exe/DisplayImage.dir/build.make CMakeFiles/exe/DisplayImage.dir/preprocess.cpp.i
.PHONY : preprocess.cpp.i

preprocess.s: preprocess.cpp.s

.PHONY : preprocess.s

# target to generate assembly for a file
preprocess.cpp.s:
	$(MAKE) -f CMakeFiles/exe/DisplayImage.dir/build.make CMakeFiles/exe/DisplayImage.dir/preprocess.cpp.s
.PHONY : preprocess.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... exe/graphcut"
	@echo "... exe/DisplayImage"
	@echo "... graphcut_example.o"
	@echo "... graphcut_example.i"
	@echo "... graphcut_example.s"
	@echo "... preprocess.o"
	@echo "... preprocess.i"
	@echo "... preprocess.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

