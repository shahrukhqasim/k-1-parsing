cmake_minimum_required(VERSION 3.5)
project(k_1_parsing2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -lpthread")

link_directories(/usr/local/lib/ /usr/lib/)
include_directories(/usr/local/include/)

set(SOURCE_FILES
    csv/CSVparser.cpp
    csv/CSVparser.hpp
    json/jsoncpp.cpp
    json/json/json.h
    json/json/json-forwards.h
    src/EditDistance.cc
    src/EditDistance.h
    src/HelperMethods.cpp
    src/HelperMethods.h
    src/main.cc
    src/TesseractFinder.cc
    src/TesseractFinder.h
    src/union-find.h src/Preprocessor.cpp src/Preprocessor.h src/OcrProgram.cpp src/OcrProgram.h src/WordEntry.cpp src/WordEntry.h src/AccuracyProgram.cpp src/AccuracyProgram.h)

add_executable(k_1_parsing2 ${SOURCE_FILES})
target_link_libraries(k_1_parsing2 lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)