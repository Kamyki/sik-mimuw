# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kamil/dev/SIK/big2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kamil/dev/SIK/big2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tescik.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tescik.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tescik.dir/flags.make

CMakeFiles/tescik.dir/test.c.o: CMakeFiles/tescik.dir/flags.make
CMakeFiles/tescik.dir/test.c.o: ../test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tescik.dir/test.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tescik.dir/test.c.o   -c /Users/kamil/dev/SIK/big2/test.c

CMakeFiles/tescik.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tescik.dir/test.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/test.c > CMakeFiles/tescik.dir/test.c.i

CMakeFiles/tescik.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tescik.dir/test.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/test.c -o CMakeFiles/tescik.dir/test.c.s

CMakeFiles/tescik.dir/test.c.o.requires:

.PHONY : CMakeFiles/tescik.dir/test.c.o.requires

CMakeFiles/tescik.dir/test.c.o.provides: CMakeFiles/tescik.dir/test.c.o.requires
	$(MAKE) -f CMakeFiles/tescik.dir/build.make CMakeFiles/tescik.dir/test.c.o.provides.build
.PHONY : CMakeFiles/tescik.dir/test.c.o.provides

CMakeFiles/tescik.dir/test.c.o.provides.build: CMakeFiles/tescik.dir/test.c.o


# Object files for target tescik
tescik_OBJECTS = \
"CMakeFiles/tescik.dir/test.c.o"

# External object files for target tescik
tescik_EXTERNAL_OBJECTS =

tescik: CMakeFiles/tescik.dir/test.c.o
tescik: CMakeFiles/tescik.dir/build.make
tescik: CMakeFiles/tescik.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tescik"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tescik.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tescik.dir/build: tescik

.PHONY : CMakeFiles/tescik.dir/build

CMakeFiles/tescik.dir/requires: CMakeFiles/tescik.dir/test.c.o.requires

.PHONY : CMakeFiles/tescik.dir/requires

CMakeFiles/tescik.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tescik.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tescik.dir/clean

CMakeFiles/tescik.dir/depend:
	cd /Users/kamil/dev/SIK/big2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kamil/dev/SIK/big2 /Users/kamil/dev/SIK/big2 /Users/kamil/dev/SIK/big2/cmake-build-debug /Users/kamil/dev/SIK/big2/cmake-build-debug /Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles/tescik.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tescik.dir/depend

