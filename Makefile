# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/shahrukhqasim/workspace/k-1-parsing2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shahrukhqasim/workspace/k-1-parsing2

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/shahrukhqasim/workspace/k-1-parsing2/CMakeFiles /home/shahrukhqasim/workspace/k-1-parsing2/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/shahrukhqasim/workspace/k-1-parsing2/CMakeFiles 0
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
# Target rules for targets named k_1_parsing2

# Build rule for target.
k_1_parsing2: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 k_1_parsing2
.PHONY : k_1_parsing2

# fast build rule for target.
k_1_parsing2/fast:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/build
.PHONY : k_1_parsing2/fast

csv/CSVparser.o: csv/CSVparser.cpp.o

.PHONY : csv/CSVparser.o

# target to build an object file
csv/CSVparser.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/csv/CSVparser.cpp.o
.PHONY : csv/CSVparser.cpp.o

csv/CSVparser.i: csv/CSVparser.cpp.i

.PHONY : csv/CSVparser.i

# target to preprocess a source file
csv/CSVparser.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/csv/CSVparser.cpp.i
.PHONY : csv/CSVparser.cpp.i

csv/CSVparser.s: csv/CSVparser.cpp.s

.PHONY : csv/CSVparser.s

# target to generate assembly for a file
csv/CSVparser.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/csv/CSVparser.cpp.s
.PHONY : csv/CSVparser.cpp.s

json/jsoncpp.o: json/jsoncpp.cpp.o

.PHONY : json/jsoncpp.o

# target to build an object file
json/jsoncpp.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/json/jsoncpp.cpp.o
.PHONY : json/jsoncpp.cpp.o

json/jsoncpp.i: json/jsoncpp.cpp.i

.PHONY : json/jsoncpp.i

# target to preprocess a source file
json/jsoncpp.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/json/jsoncpp.cpp.i
.PHONY : json/jsoncpp.cpp.i

json/jsoncpp.s: json/jsoncpp.cpp.s

.PHONY : json/jsoncpp.s

# target to generate assembly for a file
json/jsoncpp.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/json/jsoncpp.cpp.s
.PHONY : json/jsoncpp.cpp.s

src/AccuracyProgram.o: src/AccuracyProgram.cpp.o

.PHONY : src/AccuracyProgram.o

# target to build an object file
src/AccuracyProgram.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/AccuracyProgram.cpp.o
.PHONY : src/AccuracyProgram.cpp.o

src/AccuracyProgram.i: src/AccuracyProgram.cpp.i

.PHONY : src/AccuracyProgram.i

# target to preprocess a source file
src/AccuracyProgram.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/AccuracyProgram.cpp.i
.PHONY : src/AccuracyProgram.cpp.i

src/AccuracyProgram.s: src/AccuracyProgram.cpp.s

.PHONY : src/AccuracyProgram.s

# target to generate assembly for a file
src/AccuracyProgram.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/AccuracyProgram.cpp.s
.PHONY : src/AccuracyProgram.cpp.s

src/EditDistance.o: src/EditDistance.cc.o

.PHONY : src/EditDistance.o

# target to build an object file
src/EditDistance.cc.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/EditDistance.cc.o
.PHONY : src/EditDistance.cc.o

src/EditDistance.i: src/EditDistance.cc.i

.PHONY : src/EditDistance.i

# target to preprocess a source file
src/EditDistance.cc.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/EditDistance.cc.i
.PHONY : src/EditDistance.cc.i

src/EditDistance.s: src/EditDistance.cc.s

.PHONY : src/EditDistance.s

# target to generate assembly for a file
src/EditDistance.cc.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/EditDistance.cc.s
.PHONY : src/EditDistance.cc.s

src/HelperMethods.o: src/HelperMethods.cpp.o

.PHONY : src/HelperMethods.o

# target to build an object file
src/HelperMethods.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/HelperMethods.cpp.o
.PHONY : src/HelperMethods.cpp.o

src/HelperMethods.i: src/HelperMethods.cpp.i

.PHONY : src/HelperMethods.i

# target to preprocess a source file
src/HelperMethods.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/HelperMethods.cpp.i
.PHONY : src/HelperMethods.cpp.i

src/HelperMethods.s: src/HelperMethods.cpp.s

.PHONY : src/HelperMethods.s

# target to generate assembly for a file
src/HelperMethods.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/HelperMethods.cpp.s
.PHONY : src/HelperMethods.cpp.s

src/LineBreaker.o: src/LineBreaker.cpp.o

.PHONY : src/LineBreaker.o

# target to build an object file
src/LineBreaker.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/LineBreaker.cpp.o
.PHONY : src/LineBreaker.cpp.o

src/LineBreaker.i: src/LineBreaker.cpp.i

.PHONY : src/LineBreaker.i

# target to preprocess a source file
src/LineBreaker.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/LineBreaker.cpp.i
.PHONY : src/LineBreaker.cpp.i

src/LineBreaker.s: src/LineBreaker.cpp.s

.PHONY : src/LineBreaker.s

# target to generate assembly for a file
src/LineBreaker.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/LineBreaker.cpp.s
.PHONY : src/LineBreaker.cpp.s

src/Mapper.o: src/Mapper.cpp.o

.PHONY : src/Mapper.o

# target to build an object file
src/Mapper.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Mapper.cpp.o
.PHONY : src/Mapper.cpp.o

src/Mapper.i: src/Mapper.cpp.i

.PHONY : src/Mapper.i

# target to preprocess a source file
src/Mapper.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Mapper.cpp.i
.PHONY : src/Mapper.cpp.i

src/Mapper.s: src/Mapper.cpp.s

.PHONY : src/Mapper.s

# target to generate assembly for a file
src/Mapper.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Mapper.cpp.s
.PHONY : src/Mapper.cpp.s

src/Model/InputNode.o: src/Model/InputNode.cpp.o

.PHONY : src/Model/InputNode.o

# target to build an object file
src/Model/InputNode.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/InputNode.cpp.o
.PHONY : src/Model/InputNode.cpp.o

src/Model/InputNode.i: src/Model/InputNode.cpp.i

.PHONY : src/Model/InputNode.i

# target to preprocess a source file
src/Model/InputNode.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/InputNode.cpp.i
.PHONY : src/Model/InputNode.cpp.i

src/Model/InputNode.s: src/Model/InputNode.cpp.s

.PHONY : src/Model/InputNode.s

# target to generate assembly for a file
src/Model/InputNode.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/InputNode.cpp.s
.PHONY : src/Model/InputNode.cpp.s

src/Model/ModelBuilder.o: src/Model/ModelBuilder.cpp.o

.PHONY : src/Model/ModelBuilder.o

# target to build an object file
src/Model/ModelBuilder.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/ModelBuilder.cpp.o
.PHONY : src/Model/ModelBuilder.cpp.o

src/Model/ModelBuilder.i: src/Model/ModelBuilder.cpp.i

.PHONY : src/Model/ModelBuilder.i

# target to preprocess a source file
src/Model/ModelBuilder.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/ModelBuilder.cpp.i
.PHONY : src/Model/ModelBuilder.cpp.i

src/Model/ModelBuilder.s: src/Model/ModelBuilder.cpp.s

.PHONY : src/Model/ModelBuilder.s

# target to generate assembly for a file
src/Model/ModelBuilder.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/ModelBuilder.cpp.s
.PHONY : src/Model/ModelBuilder.cpp.s

src/Model/Node.o: src/Model/Node.cpp.o

.PHONY : src/Model/Node.o

# target to build an object file
src/Model/Node.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/Node.cpp.o
.PHONY : src/Model/Node.cpp.o

src/Model/Node.i: src/Model/Node.cpp.i

.PHONY : src/Model/Node.i

# target to preprocess a source file
src/Model/Node.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/Node.cpp.i
.PHONY : src/Model/Node.cpp.i

src/Model/Node.s: src/Model/Node.cpp.s

.PHONY : src/Model/Node.s

# target to generate assembly for a file
src/Model/Node.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/Node.cpp.s
.PHONY : src/Model/Node.cpp.s

src/Model/TableNode.o: src/Model/TableNode.cpp.o

.PHONY : src/Model/TableNode.o

# target to build an object file
src/Model/TableNode.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TableNode.cpp.o
.PHONY : src/Model/TableNode.cpp.o

src/Model/TableNode.i: src/Model/TableNode.cpp.i

.PHONY : src/Model/TableNode.i

# target to preprocess a source file
src/Model/TableNode.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TableNode.cpp.i
.PHONY : src/Model/TableNode.cpp.i

src/Model/TableNode.s: src/Model/TableNode.cpp.s

.PHONY : src/Model/TableNode.s

# target to generate assembly for a file
src/Model/TableNode.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TableNode.cpp.s
.PHONY : src/Model/TableNode.cpp.s

src/Model/TextNode.o: src/Model/TextNode.cpp.o

.PHONY : src/Model/TextNode.o

# target to build an object file
src/Model/TextNode.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TextNode.cpp.o
.PHONY : src/Model/TextNode.cpp.o

src/Model/TextNode.i: src/Model/TextNode.cpp.i

.PHONY : src/Model/TextNode.i

# target to preprocess a source file
src/Model/TextNode.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TextNode.cpp.i
.PHONY : src/Model/TextNode.cpp.i

src/Model/TextNode.s: src/Model/TextNode.cpp.s

.PHONY : src/Model/TextNode.s

# target to generate assembly for a file
src/Model/TextNode.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Model/TextNode.cpp.s
.PHONY : src/Model/TextNode.cpp.s

src/OcrProgram.o: src/OcrProgram.cpp.o

.PHONY : src/OcrProgram.o

# target to build an object file
src/OcrProgram.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/OcrProgram.cpp.o
.PHONY : src/OcrProgram.cpp.o

src/OcrProgram.i: src/OcrProgram.cpp.i

.PHONY : src/OcrProgram.i

# target to preprocess a source file
src/OcrProgram.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/OcrProgram.cpp.i
.PHONY : src/OcrProgram.cpp.i

src/OcrProgram.s: src/OcrProgram.cpp.s

.PHONY : src/OcrProgram.s

# target to generate assembly for a file
src/OcrProgram.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/OcrProgram.cpp.s
.PHONY : src/OcrProgram.cpp.s

src/Preprocessor.o: src/Preprocessor.cpp.o

.PHONY : src/Preprocessor.o

# target to build an object file
src/Preprocessor.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Preprocessor.cpp.o
.PHONY : src/Preprocessor.cpp.o

src/Preprocessor.i: src/Preprocessor.cpp.i

.PHONY : src/Preprocessor.i

# target to preprocess a source file
src/Preprocessor.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Preprocessor.cpp.i
.PHONY : src/Preprocessor.cpp.i

src/Preprocessor.s: src/Preprocessor.cpp.s

.PHONY : src/Preprocessor.s

# target to generate assembly for a file
src/Preprocessor.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Preprocessor.cpp.s
.PHONY : src/Preprocessor.cpp.s

src/Processor.o: src/Processor.cpp.o

.PHONY : src/Processor.o

# target to build an object file
src/Processor.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor.cpp.o
.PHONY : src/Processor.cpp.o

src/Processor.i: src/Processor.cpp.i

.PHONY : src/Processor.i

# target to preprocess a source file
src/Processor.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor.cpp.i
.PHONY : src/Processor.cpp.i

src/Processor.s: src/Processor.cpp.s

.PHONY : src/Processor.s

# target to generate assembly for a file
src/Processor.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor.cpp.s
.PHONY : src/Processor.cpp.s

src/Processor2.o: src/Processor2.cpp.o

.PHONY : src/Processor2.o

# target to build an object file
src/Processor2.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor2.cpp.o
.PHONY : src/Processor2.cpp.o

src/Processor2.i: src/Processor2.cpp.i

.PHONY : src/Processor2.i

# target to preprocess a source file
src/Processor2.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor2.cpp.i
.PHONY : src/Processor2.cpp.i

src/Processor2.s: src/Processor2.cpp.s

.PHONY : src/Processor2.s

# target to generate assembly for a file
src/Processor2.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/Processor2.cpp.s
.PHONY : src/Processor2.cpp.s

src/TesseractFinder.o: src/TesseractFinder.cc.o

.PHONY : src/TesseractFinder.o

# target to build an object file
src/TesseractFinder.cc.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TesseractFinder.cc.o
.PHONY : src/TesseractFinder.cc.o

src/TesseractFinder.i: src/TesseractFinder.cc.i

.PHONY : src/TesseractFinder.i

# target to preprocess a source file
src/TesseractFinder.cc.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TesseractFinder.cc.i
.PHONY : src/TesseractFinder.cc.i

src/TesseractFinder.s: src/TesseractFinder.cc.s

.PHONY : src/TesseractFinder.s

# target to generate assembly for a file
src/TesseractFinder.cc.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TesseractFinder.cc.s
.PHONY : src/TesseractFinder.cc.s

src/TextualData.o: src/TextualData.cpp.o

.PHONY : src/TextualData.o

# target to build an object file
src/TextualData.cpp.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TextualData.cpp.o
.PHONY : src/TextualData.cpp.o

src/TextualData.i: src/TextualData.cpp.i

.PHONY : src/TextualData.i

# target to preprocess a source file
src/TextualData.cpp.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TextualData.cpp.i
.PHONY : src/TextualData.cpp.i

src/TextualData.s: src/TextualData.cpp.s

.PHONY : src/TextualData.s

# target to generate assembly for a file
src/TextualData.cpp.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/TextualData.cpp.s
.PHONY : src/TextualData.cpp.s

src/main.o: src/main.cc.o

.PHONY : src/main.o

# target to build an object file
src/main.cc.o:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/main.cc.o
.PHONY : src/main.cc.o

src/main.i: src/main.cc.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cc.i:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/main.cc.i
.PHONY : src/main.cc.i

src/main.s: src/main.cc.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cc.s:
	$(MAKE) -f CMakeFiles/k_1_parsing2.dir/build.make CMakeFiles/k_1_parsing2.dir/src/main.cc.s
.PHONY : src/main.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... k_1_parsing2"
	@echo "... csv/CSVparser.o"
	@echo "... csv/CSVparser.i"
	@echo "... csv/CSVparser.s"
	@echo "... json/jsoncpp.o"
	@echo "... json/jsoncpp.i"
	@echo "... json/jsoncpp.s"
	@echo "... src/AccuracyProgram.o"
	@echo "... src/AccuracyProgram.i"
	@echo "... src/AccuracyProgram.s"
	@echo "... src/EditDistance.o"
	@echo "... src/EditDistance.i"
	@echo "... src/EditDistance.s"
	@echo "... src/HelperMethods.o"
	@echo "... src/HelperMethods.i"
	@echo "... src/HelperMethods.s"
	@echo "... src/LineBreaker.o"
	@echo "... src/LineBreaker.i"
	@echo "... src/LineBreaker.s"
	@echo "... src/Mapper.o"
	@echo "... src/Mapper.i"
	@echo "... src/Mapper.s"
	@echo "... src/Model/InputNode.o"
	@echo "... src/Model/InputNode.i"
	@echo "... src/Model/InputNode.s"
	@echo "... src/Model/ModelBuilder.o"
	@echo "... src/Model/ModelBuilder.i"
	@echo "... src/Model/ModelBuilder.s"
	@echo "... src/Model/Node.o"
	@echo "... src/Model/Node.i"
	@echo "... src/Model/Node.s"
	@echo "... src/Model/TableNode.o"
	@echo "... src/Model/TableNode.i"
	@echo "... src/Model/TableNode.s"
	@echo "... src/Model/TextNode.o"
	@echo "... src/Model/TextNode.i"
	@echo "... src/Model/TextNode.s"
	@echo "... src/OcrProgram.o"
	@echo "... src/OcrProgram.i"
	@echo "... src/OcrProgram.s"
	@echo "... src/Preprocessor.o"
	@echo "... src/Preprocessor.i"
	@echo "... src/Preprocessor.s"
	@echo "... src/Processor.o"
	@echo "... src/Processor.i"
	@echo "... src/Processor.s"
	@echo "... src/Processor2.o"
	@echo "... src/Processor2.i"
	@echo "... src/Processor2.s"
	@echo "... src/TesseractFinder.o"
	@echo "... src/TesseractFinder.i"
	@echo "... src/TesseractFinder.s"
	@echo "... src/TextualData.o"
	@echo "... src/TextualData.i"
	@echo "... src/TextualData.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
