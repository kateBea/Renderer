﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.3.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.3.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\zanet\Desktop\Renderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\zanet\Desktop\Renderer\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\window.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles\window.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\window.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\window.dir\flags.make

CMakeFiles\window.dir\test\WindowTest.cpp.obj: CMakeFiles\window.dir\flags.make
CMakeFiles\window.dir\test\WindowTest.cpp.obj: C:\Users\zanet\Desktop\Renderer\test\WindowTest.cpp
CMakeFiles\window.dir\test\WindowTest.cpp.obj: CMakeFiles\window.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\zanet\Desktop\Renderer\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/window.dir/test/WindowTest.cpp.obj"
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\window.dir\test\WindowTest.cpp.obj.d --working-dir=C:\Users\zanet\Desktop\Renderer\cmake-build-debug --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~2\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\HostX86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\window.dir\test\WindowTest.cpp.obj /FdCMakeFiles\window.dir\ /FS -c C:\Users\zanet\Desktop\Renderer\test\WindowTest.cpp
<<

CMakeFiles\window.dir\test\WindowTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/window.dir/test/WindowTest.cpp.i"
	C:\PROGRA~1\MICROS~2\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\HostX86\x86\cl.exe > CMakeFiles\window.dir\test\WindowTest.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\zanet\Desktop\Renderer\test\WindowTest.cpp
<<

CMakeFiles\window.dir\test\WindowTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/window.dir/test/WindowTest.cpp.s"
	C:\PROGRA~1\MICROS~2\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\HostX86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\window.dir\test\WindowTest.cpp.s /c C:\Users\zanet\Desktop\Renderer\test\WindowTest.cpp
<<

# Object files for target window
window_OBJECTS = \
"CMakeFiles\window.dir\test\WindowTest.cpp.obj"

# External object files for target window
window_EXTERNAL_OBJECTS =

window.exe: CMakeFiles\window.dir\test\WindowTest.cpp.obj
window.exe: CMakeFiles\window.dir\build.make
window.exe: library\glfw\src\glfw3.lib
window.exe: lib\libglew32d.lib
window.exe: CMakeFiles\window.dir\objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\zanet\Desktop\Renderer\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable window.exe"
	"C:\Program Files\JetBrains\CLion 2022.3.2\bin\cmake\win\x64\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\window.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100226~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100226~1.0\x86\mt.exe --manifests -- C:\PROGRA~1\MICROS~2\2022\COMMUN~1\VC\Tools\MSVC\1433~1.316\bin\HostX86\x86\link.exe /nologo @CMakeFiles\window.dir\objects1 @<<
 /out:window.exe /implib:window.lib /pdb:C:\Users\zanet\Desktop\Renderer\cmake-build-debug\window.pdb /version:0.0 /machine:X86 /debug /INCREMENTAL /subsystem:console  library\glfw\src\glfw3.lib lib\libglew32d.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\window.dir\build: window.exe
.PHONY : CMakeFiles\window.dir\build

CMakeFiles\window.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\window.dir\cmake_clean.cmake
.PHONY : CMakeFiles\window.dir\clean

CMakeFiles\window.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\zanet\Desktop\Renderer C:\Users\zanet\Desktop\Renderer C:\Users\zanet\Desktop\Renderer\cmake-build-debug C:\Users\zanet\Desktop\Renderer\cmake-build-debug C:\Users\zanet\Desktop\Renderer\cmake-build-debug\CMakeFiles\window.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\window.dir\depend

