# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master

# Include any dependencies generated for this target.
include examples/CMakeFiles/http-server.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/http-server.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/http-server.dir/flags.make

examples/CMakeFiles/http-server.dir/http_server.cpp.o: examples/CMakeFiles/http-server.dir/flags.make
examples/CMakeFiles/http-server.dir/http_server.cpp.o: examples/http_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/http-server.dir/http_server.cpp.o"
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/http-server.dir/http_server.cpp.o -c /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples/http_server.cpp

examples/CMakeFiles/http-server.dir/http_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http-server.dir/http_server.cpp.i"
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples/http_server.cpp > CMakeFiles/http-server.dir/http_server.cpp.i

examples/CMakeFiles/http-server.dir/http_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http-server.dir/http_server.cpp.s"
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples/http_server.cpp -o CMakeFiles/http-server.dir/http_server.cpp.s

examples/CMakeFiles/http-server.dir/http_server.cpp.o.requires:

.PHONY : examples/CMakeFiles/http-server.dir/http_server.cpp.o.requires

examples/CMakeFiles/http-server.dir/http_server.cpp.o.provides: examples/CMakeFiles/http-server.dir/http_server.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/http-server.dir/build.make examples/CMakeFiles/http-server.dir/http_server.cpp.o.provides.build
.PHONY : examples/CMakeFiles/http-server.dir/http_server.cpp.o.provides

examples/CMakeFiles/http-server.dir/http_server.cpp.o.provides.build: examples/CMakeFiles/http-server.dir/http_server.cpp.o


# Object files for target http-server
http__server_OBJECTS = \
"CMakeFiles/http-server.dir/http_server.cpp.o"

# External object files for target http-server
http__server_EXTERNAL_OBJECTS =

examples/http-server: examples/CMakeFiles/http-server.dir/http_server.cpp.o
examples/http-server: examples/CMakeFiles/http-server.dir/build.make
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_coroutine.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_context.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_thread.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_program_options.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libboost_system.a
examples/http-server: /usr/lib/x86_64-linux-gnu/libpthread.so
examples/http-server: examples/CMakeFiles/http-server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable http-server"
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http-server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/http-server.dir/build: examples/http-server

.PHONY : examples/CMakeFiles/http-server.dir/build

examples/CMakeFiles/http-server.dir/requires: examples/CMakeFiles/http-server.dir/http_server.cpp.o.requires

.PHONY : examples/CMakeFiles/http-server.dir/requires

examples/CMakeFiles/http-server.dir/clean:
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples && $(CMAKE_COMMAND) -P CMakeFiles/http-server.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/http-server.dir/clean

examples/CMakeFiles/http-server.dir/depend:
	cd /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/examples/CMakeFiles/http-server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/http-server.dir/depend
