# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = E:/CMake/cmake-3.24.1-windows-x86_64/bin/cmake.exe

# The command to remove a file.
RM = E:/CMake/cmake-3.24.1-windows-x86_64/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:/Code/CG-CourseWorks/CourseWork3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:/Code/CG-CourseWorks/CourseWork3/build

# Include any dependencies generated for this target.
include CMakeFiles/CUSTOM_MATH_LIB.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CUSTOM_MATH_LIB.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CUSTOM_MATH_LIB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CUSTOM_MATH_LIB.dir/flags.make

CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj: CMakeFiles/CUSTOM_MATH_LIB.dir/flags.make
CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj: CMakeFiles/CUSTOM_MATH_LIB.dir/includes_CXX.rsp
CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj: E:/Code/CG-CourseWorks/CourseWork3/src/CusMath.cpp
CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj: CMakeFiles/CUSTOM_MATH_LIB.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:/Code/CG-CourseWorks/CourseWork3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj"
	E:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj -MF CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj.d -o CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj -c E:/Code/CG-CourseWorks/CourseWork3/src/CusMath.cpp

CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.i"
	E:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:/Code/CG-CourseWorks/CourseWork3/src/CusMath.cpp > CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.i

CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.s"
	E:/MinGW/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:/Code/CG-CourseWorks/CourseWork3/src/CusMath.cpp -o CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.s

# Object files for target CUSTOM_MATH_LIB
CUSTOM_MATH_LIB_OBJECTS = \
"CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj"

# External object files for target CUSTOM_MATH_LIB
CUSTOM_MATH_LIB_EXTERNAL_OBJECTS =

libCUSTOM_MATH_LIB.a: CMakeFiles/CUSTOM_MATH_LIB.dir/src/CusMath.cpp.obj
libCUSTOM_MATH_LIB.a: CMakeFiles/CUSTOM_MATH_LIB.dir/build.make
libCUSTOM_MATH_LIB.a: CMakeFiles/CUSTOM_MATH_LIB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:/Code/CG-CourseWorks/CourseWork3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libCUSTOM_MATH_LIB.a"
	$(CMAKE_COMMAND) -P CMakeFiles/CUSTOM_MATH_LIB.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CUSTOM_MATH_LIB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CUSTOM_MATH_LIB.dir/build: libCUSTOM_MATH_LIB.a
.PHONY : CMakeFiles/CUSTOM_MATH_LIB.dir/build

CMakeFiles/CUSTOM_MATH_LIB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CUSTOM_MATH_LIB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CUSTOM_MATH_LIB.dir/clean

CMakeFiles/CUSTOM_MATH_LIB.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" E:/Code/CG-CourseWorks/CourseWork3 E:/Code/CG-CourseWorks/CourseWork3 E:/Code/CG-CourseWorks/CourseWork3/build E:/Code/CG-CourseWorks/CourseWork3/build E:/Code/CG-CourseWorks/CourseWork3/build/CMakeFiles/CUSTOM_MATH_LIB.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CUSTOM_MATH_LIB.dir/depend
