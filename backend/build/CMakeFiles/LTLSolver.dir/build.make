# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/swarnim/Cambridge/Projects/SAT-Solver/backend

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/swarnim/Cambridge/Projects/SAT-Solver/backend/build

# Include any dependencies generated for this target.
include CMakeFiles/LTLSolver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/LTLSolver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/LTLSolver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LTLSolver.dir/flags.make

CMakeFiles/LTLSolver.dir/src/main.cpp.o: CMakeFiles/LTLSolver.dir/flags.make
CMakeFiles/LTLSolver.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/LTLSolver.dir/src/main.cpp.o: CMakeFiles/LTLSolver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/swarnim/Cambridge/Projects/SAT-Solver/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LTLSolver.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/LTLSolver.dir/src/main.cpp.o -MF CMakeFiles/LTLSolver.dir/src/main.cpp.o.d -o CMakeFiles/LTLSolver.dir/src/main.cpp.o -c /home/swarnim/Cambridge/Projects/SAT-Solver/backend/src/main.cpp

CMakeFiles/LTLSolver.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LTLSolver.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/swarnim/Cambridge/Projects/SAT-Solver/backend/src/main.cpp > CMakeFiles/LTLSolver.dir/src/main.cpp.i

CMakeFiles/LTLSolver.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LTLSolver.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/swarnim/Cambridge/Projects/SAT-Solver/backend/src/main.cpp -o CMakeFiles/LTLSolver.dir/src/main.cpp.s

# Object files for target LTLSolver
LTLSolver_OBJECTS = \
"CMakeFiles/LTLSolver.dir/src/main.cpp.o"

# External object files for target LTLSolver
LTLSolver_EXTERNAL_OBJECTS =

LTLSolver: CMakeFiles/LTLSolver.dir/src/main.cpp.o
LTLSolver: CMakeFiles/LTLSolver.dir/build.make
LTLSolver: CMakeFiles/LTLSolver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/swarnim/Cambridge/Projects/SAT-Solver/backend/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable LTLSolver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LTLSolver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LTLSolver.dir/build: LTLSolver
.PHONY : CMakeFiles/LTLSolver.dir/build

CMakeFiles/LTLSolver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LTLSolver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LTLSolver.dir/clean

CMakeFiles/LTLSolver.dir/depend:
	cd /home/swarnim/Cambridge/Projects/SAT-Solver/backend/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/swarnim/Cambridge/Projects/SAT-Solver/backend /home/swarnim/Cambridge/Projects/SAT-Solver/backend /home/swarnim/Cambridge/Projects/SAT-Solver/backend/build /home/swarnim/Cambridge/Projects/SAT-Solver/backend/build /home/swarnim/Cambridge/Projects/SAT-Solver/backend/build/CMakeFiles/LTLSolver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LTLSolver.dir/depend

