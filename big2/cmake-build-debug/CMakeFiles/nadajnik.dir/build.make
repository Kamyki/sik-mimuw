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
include CMakeFiles/nadajnik.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/nadajnik.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nadajnik.dir/flags.make

CMakeFiles/nadajnik.dir/nadajnik.c.o: CMakeFiles/nadajnik.dir/flags.make
CMakeFiles/nadajnik.dir/nadajnik.c.o: ../nadajnik.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/nadajnik.dir/nadajnik.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nadajnik.dir/nadajnik.c.o   -c /Users/kamil/dev/SIK/big2/nadajnik.c

CMakeFiles/nadajnik.dir/nadajnik.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nadajnik.dir/nadajnik.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/nadajnik.c > CMakeFiles/nadajnik.dir/nadajnik.c.i

CMakeFiles/nadajnik.dir/nadajnik.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nadajnik.dir/nadajnik.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/nadajnik.c -o CMakeFiles/nadajnik.dir/nadajnik.c.s

CMakeFiles/nadajnik.dir/nadajnik.c.o.requires:

.PHONY : CMakeFiles/nadajnik.dir/nadajnik.c.o.requires

CMakeFiles/nadajnik.dir/nadajnik.c.o.provides: CMakeFiles/nadajnik.dir/nadajnik.c.o.requires
	$(MAKE) -f CMakeFiles/nadajnik.dir/build.make CMakeFiles/nadajnik.dir/nadajnik.c.o.provides.build
.PHONY : CMakeFiles/nadajnik.dir/nadajnik.c.o.provides

CMakeFiles/nadajnik.dir/nadajnik.c.o.provides.build: CMakeFiles/nadajnik.dir/nadajnik.c.o


CMakeFiles/nadajnik.dir/fifo.c.o: CMakeFiles/nadajnik.dir/flags.make
CMakeFiles/nadajnik.dir/fifo.c.o: ../fifo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/nadajnik.dir/fifo.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nadajnik.dir/fifo.c.o   -c /Users/kamil/dev/SIK/big2/fifo.c

CMakeFiles/nadajnik.dir/fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nadajnik.dir/fifo.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/fifo.c > CMakeFiles/nadajnik.dir/fifo.c.i

CMakeFiles/nadajnik.dir/fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nadajnik.dir/fifo.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/fifo.c -o CMakeFiles/nadajnik.dir/fifo.c.s

CMakeFiles/nadajnik.dir/fifo.c.o.requires:

.PHONY : CMakeFiles/nadajnik.dir/fifo.c.o.requires

CMakeFiles/nadajnik.dir/fifo.c.o.provides: CMakeFiles/nadajnik.dir/fifo.c.o.requires
	$(MAKE) -f CMakeFiles/nadajnik.dir/build.make CMakeFiles/nadajnik.dir/fifo.c.o.provides.build
.PHONY : CMakeFiles/nadajnik.dir/fifo.c.o.provides

CMakeFiles/nadajnik.dir/fifo.c.o.provides.build: CMakeFiles/nadajnik.dir/fifo.c.o


CMakeFiles/nadajnik.dir/set.c.o: CMakeFiles/nadajnik.dir/flags.make
CMakeFiles/nadajnik.dir/set.c.o: ../set.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/nadajnik.dir/set.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nadajnik.dir/set.c.o   -c /Users/kamil/dev/SIK/big2/set.c

CMakeFiles/nadajnik.dir/set.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nadajnik.dir/set.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/set.c > CMakeFiles/nadajnik.dir/set.c.i

CMakeFiles/nadajnik.dir/set.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nadajnik.dir/set.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/set.c -o CMakeFiles/nadajnik.dir/set.c.s

CMakeFiles/nadajnik.dir/set.c.o.requires:

.PHONY : CMakeFiles/nadajnik.dir/set.c.o.requires

CMakeFiles/nadajnik.dir/set.c.o.provides: CMakeFiles/nadajnik.dir/set.c.o.requires
	$(MAKE) -f CMakeFiles/nadajnik.dir/build.make CMakeFiles/nadajnik.dir/set.c.o.provides.build
.PHONY : CMakeFiles/nadajnik.dir/set.c.o.provides

CMakeFiles/nadajnik.dir/set.c.o.provides.build: CMakeFiles/nadajnik.dir/set.c.o


CMakeFiles/nadajnik.dir/argument_parser.c.o: CMakeFiles/nadajnik.dir/flags.make
CMakeFiles/nadajnik.dir/argument_parser.c.o: ../argument_parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/nadajnik.dir/argument_parser.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nadajnik.dir/argument_parser.c.o   -c /Users/kamil/dev/SIK/big2/argument_parser.c

CMakeFiles/nadajnik.dir/argument_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nadajnik.dir/argument_parser.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/argument_parser.c > CMakeFiles/nadajnik.dir/argument_parser.c.i

CMakeFiles/nadajnik.dir/argument_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nadajnik.dir/argument_parser.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/argument_parser.c -o CMakeFiles/nadajnik.dir/argument_parser.c.s

CMakeFiles/nadajnik.dir/argument_parser.c.o.requires:

.PHONY : CMakeFiles/nadajnik.dir/argument_parser.c.o.requires

CMakeFiles/nadajnik.dir/argument_parser.c.o.provides: CMakeFiles/nadajnik.dir/argument_parser.c.o.requires
	$(MAKE) -f CMakeFiles/nadajnik.dir/build.make CMakeFiles/nadajnik.dir/argument_parser.c.o.provides.build
.PHONY : CMakeFiles/nadajnik.dir/argument_parser.c.o.provides

CMakeFiles/nadajnik.dir/argument_parser.c.o.provides.build: CMakeFiles/nadajnik.dir/argument_parser.c.o


CMakeFiles/nadajnik.dir/err.c.o: CMakeFiles/nadajnik.dir/flags.make
CMakeFiles/nadajnik.dir/err.c.o: ../err.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/nadajnik.dir/err.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/nadajnik.dir/err.c.o   -c /Users/kamil/dev/SIK/big2/err.c

CMakeFiles/nadajnik.dir/err.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/nadajnik.dir/err.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kamil/dev/SIK/big2/err.c > CMakeFiles/nadajnik.dir/err.c.i

CMakeFiles/nadajnik.dir/err.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/nadajnik.dir/err.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kamil/dev/SIK/big2/err.c -o CMakeFiles/nadajnik.dir/err.c.s

CMakeFiles/nadajnik.dir/err.c.o.requires:

.PHONY : CMakeFiles/nadajnik.dir/err.c.o.requires

CMakeFiles/nadajnik.dir/err.c.o.provides: CMakeFiles/nadajnik.dir/err.c.o.requires
	$(MAKE) -f CMakeFiles/nadajnik.dir/build.make CMakeFiles/nadajnik.dir/err.c.o.provides.build
.PHONY : CMakeFiles/nadajnik.dir/err.c.o.provides

CMakeFiles/nadajnik.dir/err.c.o.provides.build: CMakeFiles/nadajnik.dir/err.c.o


# Object files for target nadajnik
nadajnik_OBJECTS = \
"CMakeFiles/nadajnik.dir/nadajnik.c.o" \
"CMakeFiles/nadajnik.dir/fifo.c.o" \
"CMakeFiles/nadajnik.dir/set.c.o" \
"CMakeFiles/nadajnik.dir/argument_parser.c.o" \
"CMakeFiles/nadajnik.dir/err.c.o"

# External object files for target nadajnik
nadajnik_EXTERNAL_OBJECTS =

nadajnik: CMakeFiles/nadajnik.dir/nadajnik.c.o
nadajnik: CMakeFiles/nadajnik.dir/fifo.c.o
nadajnik: CMakeFiles/nadajnik.dir/set.c.o
nadajnik: CMakeFiles/nadajnik.dir/argument_parser.c.o
nadajnik: CMakeFiles/nadajnik.dir/err.c.o
nadajnik: CMakeFiles/nadajnik.dir/build.make
nadajnik: CMakeFiles/nadajnik.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable nadajnik"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nadajnik.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nadajnik.dir/build: nadajnik

.PHONY : CMakeFiles/nadajnik.dir/build

CMakeFiles/nadajnik.dir/requires: CMakeFiles/nadajnik.dir/nadajnik.c.o.requires
CMakeFiles/nadajnik.dir/requires: CMakeFiles/nadajnik.dir/fifo.c.o.requires
CMakeFiles/nadajnik.dir/requires: CMakeFiles/nadajnik.dir/set.c.o.requires
CMakeFiles/nadajnik.dir/requires: CMakeFiles/nadajnik.dir/argument_parser.c.o.requires
CMakeFiles/nadajnik.dir/requires: CMakeFiles/nadajnik.dir/err.c.o.requires

.PHONY : CMakeFiles/nadajnik.dir/requires

CMakeFiles/nadajnik.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nadajnik.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nadajnik.dir/clean

CMakeFiles/nadajnik.dir/depend:
	cd /Users/kamil/dev/SIK/big2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kamil/dev/SIK/big2 /Users/kamil/dev/SIK/big2 /Users/kamil/dev/SIK/big2/cmake-build-debug /Users/kamil/dev/SIK/big2/cmake-build-debug /Users/kamil/dev/SIK/big2/cmake-build-debug/CMakeFiles/nadajnik.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/nadajnik.dir/depend

