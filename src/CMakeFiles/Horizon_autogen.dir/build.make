# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tombutcher/Projects/Horizon-DAW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tombutcher/Projects/Horizon-DAW

# Utility rule file for Horizon_autogen.

# Include the progress variables for this target.
include src/CMakeFiles/Horizon_autogen.dir/progress.make

src/CMakeFiles/Horizon_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/tombutcher/Projects/Horizon-DAW/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target Horizon"
	cd /Users/tombutcher/Projects/Horizon-DAW/src && /usr/local/Cellar/cmake/3.15.5/bin/cmake -E cmake_autogen /Users/tombutcher/Projects/Horizon-DAW/src/CMakeFiles/Horizon_autogen.dir/AutogenInfo.cmake ""

Horizon_autogen: src/CMakeFiles/Horizon_autogen
Horizon_autogen: src/CMakeFiles/Horizon_autogen.dir/build.make

.PHONY : Horizon_autogen

# Rule to build all files generated by this target.
src/CMakeFiles/Horizon_autogen.dir/build: Horizon_autogen

.PHONY : src/CMakeFiles/Horizon_autogen.dir/build

src/CMakeFiles/Horizon_autogen.dir/clean:
	cd /Users/tombutcher/Projects/Horizon-DAW/src && $(CMAKE_COMMAND) -P CMakeFiles/Horizon_autogen.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/Horizon_autogen.dir/clean

src/CMakeFiles/Horizon_autogen.dir/depend:
	cd /Users/tombutcher/Projects/Horizon-DAW && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tombutcher/Projects/Horizon-DAW /Users/tombutcher/Projects/Horizon-DAW/src /Users/tombutcher/Projects/Horizon-DAW /Users/tombutcher/Projects/Horizon-DAW/src /Users/tombutcher/Projects/Horizon-DAW/src/CMakeFiles/Horizon_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/Horizon_autogen.dir/depend
