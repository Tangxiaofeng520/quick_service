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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/diandian/代码/quick_service

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/diandian/代码/quick_service/build

# Include any dependencies generated for this target.
include CMakeFiles/quick_service.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/quick_service.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/quick_service.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/quick_service.dir/flags.make

CMakeFiles/quick_service.dir/src/qs.cpp.o: CMakeFiles/quick_service.dir/flags.make
CMakeFiles/quick_service.dir/src/qs.cpp.o: /Users/diandian/代码/quick_service/src/qs.cpp
CMakeFiles/quick_service.dir/src/qs.cpp.o: CMakeFiles/quick_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/diandian/代码/quick_service/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/quick_service.dir/src/qs.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/quick_service.dir/src/qs.cpp.o -MF CMakeFiles/quick_service.dir/src/qs.cpp.o.d -o CMakeFiles/quick_service.dir/src/qs.cpp.o -c /Users/diandian/代码/quick_service/src/qs.cpp

CMakeFiles/quick_service.dir/src/qs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/quick_service.dir/src/qs.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/diandian/代码/quick_service/src/qs.cpp > CMakeFiles/quick_service.dir/src/qs.cpp.i

CMakeFiles/quick_service.dir/src/qs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/quick_service.dir/src/qs.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/diandian/代码/quick_service/src/qs.cpp -o CMakeFiles/quick_service.dir/src/qs.cpp.s

CMakeFiles/quick_service.dir/src/worker.cpp.o: CMakeFiles/quick_service.dir/flags.make
CMakeFiles/quick_service.dir/src/worker.cpp.o: /Users/diandian/代码/quick_service/src/worker.cpp
CMakeFiles/quick_service.dir/src/worker.cpp.o: CMakeFiles/quick_service.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/diandian/代码/quick_service/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/quick_service.dir/src/worker.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/quick_service.dir/src/worker.cpp.o -MF CMakeFiles/quick_service.dir/src/worker.cpp.o.d -o CMakeFiles/quick_service.dir/src/worker.cpp.o -c /Users/diandian/代码/quick_service/src/worker.cpp

CMakeFiles/quick_service.dir/src/worker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/quick_service.dir/src/worker.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/diandian/代码/quick_service/src/worker.cpp > CMakeFiles/quick_service.dir/src/worker.cpp.i

CMakeFiles/quick_service.dir/src/worker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/quick_service.dir/src/worker.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/diandian/代码/quick_service/src/worker.cpp -o CMakeFiles/quick_service.dir/src/worker.cpp.s

# Object files for target quick_service
quick_service_OBJECTS = \
"CMakeFiles/quick_service.dir/src/qs.cpp.o" \
"CMakeFiles/quick_service.dir/src/worker.cpp.o"

# External object files for target quick_service
quick_service_EXTERNAL_OBJECTS =

quick_service: CMakeFiles/quick_service.dir/src/qs.cpp.o
quick_service: CMakeFiles/quick_service.dir/src/worker.cpp.o
quick_service: CMakeFiles/quick_service.dir/build.make
quick_service: CMakeFiles/quick_service.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/diandian/代码/quick_service/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable quick_service"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/quick_service.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/quick_service.dir/build: quick_service
.PHONY : CMakeFiles/quick_service.dir/build

CMakeFiles/quick_service.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/quick_service.dir/cmake_clean.cmake
.PHONY : CMakeFiles/quick_service.dir/clean

CMakeFiles/quick_service.dir/depend:
	cd /Users/diandian/代码/quick_service/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/diandian/代码/quick_service /Users/diandian/代码/quick_service /Users/diandian/代码/quick_service/build /Users/diandian/代码/quick_service/build /Users/diandian/代码/quick_service/build/CMakeFiles/quick_service.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/quick_service.dir/depend

