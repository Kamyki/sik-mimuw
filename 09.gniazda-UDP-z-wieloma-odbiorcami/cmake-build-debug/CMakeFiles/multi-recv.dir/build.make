# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_SOURCE_DIR = /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/multi-recv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/multi-recv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/multi-recv.dir/flags.make

CMakeFiles/multi-recv.dir/err.c.o: CMakeFiles/multi-recv.dir/flags.make
CMakeFiles/multi-recv.dir/err.c.o: ../err.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/multi-recv.dir/err.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/multi-recv.dir/err.c.o   -c /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/err.c

CMakeFiles/multi-recv.dir/err.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/multi-recv.dir/err.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/err.c > CMakeFiles/multi-recv.dir/err.c.i

CMakeFiles/multi-recv.dir/err.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/multi-recv.dir/err.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/err.c -o CMakeFiles/multi-recv.dir/err.c.s

CMakeFiles/multi-recv.dir/err.c.o.requires:

.PHONY : CMakeFiles/multi-recv.dir/err.c.o.requires

CMakeFiles/multi-recv.dir/err.c.o.provides: CMakeFiles/multi-recv.dir/err.c.o.requires
	$(MAKE) -f CMakeFiles/multi-recv.dir/build.make CMakeFiles/multi-recv.dir/err.c.o.provides.build
.PHONY : CMakeFiles/multi-recv.dir/err.c.o.provides

CMakeFiles/multi-recv.dir/err.c.o.provides.build: CMakeFiles/multi-recv.dir/err.c.o


CMakeFiles/multi-recv.dir/multi-recv.c.o: CMakeFiles/multi-recv.dir/flags.make
CMakeFiles/multi-recv.dir/multi-recv.c.o: ../multi-recv.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/multi-recv.dir/multi-recv.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/multi-recv.dir/multi-recv.c.o   -c /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/multi-recv.c

CMakeFiles/multi-recv.dir/multi-recv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/multi-recv.dir/multi-recv.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/multi-recv.c > CMakeFiles/multi-recv.dir/multi-recv.c.i

CMakeFiles/multi-recv.dir/multi-recv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/multi-recv.dir/multi-recv.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/multi-recv.c -o CMakeFiles/multi-recv.dir/multi-recv.c.s

CMakeFiles/multi-recv.dir/multi-recv.c.o.requires:

.PHONY : CMakeFiles/multi-recv.dir/multi-recv.c.o.requires

CMakeFiles/multi-recv.dir/multi-recv.c.o.provides: CMakeFiles/multi-recv.dir/multi-recv.c.o.requires
	$(MAKE) -f CMakeFiles/multi-recv.dir/build.make CMakeFiles/multi-recv.dir/multi-recv.c.o.provides.build
.PHONY : CMakeFiles/multi-recv.dir/multi-recv.c.o.provides

CMakeFiles/multi-recv.dir/multi-recv.c.o.provides.build: CMakeFiles/multi-recv.dir/multi-recv.c.o


# Object files for target multi-recv
multi__recv_OBJECTS = \
"CMakeFiles/multi-recv.dir/err.c.o" \
"CMakeFiles/multi-recv.dir/multi-recv.c.o"

# External object files for target multi-recv
multi__recv_EXTERNAL_OBJECTS =

multi-recv: CMakeFiles/multi-recv.dir/err.c.o
multi-recv: CMakeFiles/multi-recv.dir/multi-recv.c.o
multi-recv: CMakeFiles/multi-recv.dir/build.make
multi-recv: CMakeFiles/multi-recv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable multi-recv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multi-recv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/multi-recv.dir/build: multi-recv

.PHONY : CMakeFiles/multi-recv.dir/build

CMakeFiles/multi-recv.dir/requires: CMakeFiles/multi-recv.dir/err.c.o.requires
CMakeFiles/multi-recv.dir/requires: CMakeFiles/multi-recv.dir/multi-recv.c.o.requires

.PHONY : CMakeFiles/multi-recv.dir/requires

CMakeFiles/multi-recv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/multi-recv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/multi-recv.dir/clean

CMakeFiles/multi-recv.dir/depend:
	cd /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug /Users/kamil/dev/SIK/09.gniazda-UDP-z-wieloma-odbiorcami/cmake-build-debug/CMakeFiles/multi-recv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/multi-recv.dir/depend
