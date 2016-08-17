cmake_minimum_required(VERSION 3.5)
project(k1_parsing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -lpthread")

link_directories(/usr/local/lib/ /usr/lib/)
include_directories(/usr/local/include/)

set(SOURCE_FILES
    csv/CSVparser.cc
    csv/CSVparser.h
    json/jsoncpp.cc
    json/json/json.h
    json/json/json-forwards.h
    src/EditDistance.cc
    src/EditDistance.h
    src/HelperMethods.cc
    src/HelperMethods.h
    src/main.cc
    src/TesseractFinder.cc
    src/TesseractFinder.h
    src/UnionFind.h
    src/Preprocessor.cc
    src/Preprocessor.h
    src/OcrProgram.cc
    src/OcrProgram.h
        src/DataTypes/TextualData.cc
        src/DataTypes/TextualData.h
        src/OcrEvaluation.cc
        src/OcrEvaluation.h
        src/Processor2.cc
    src/Processor2.h 
    src/Mapper.cc 
    src/Mapper.h
        src/DocumentModel.cc
        src/DocumentModel.h
        src/DataTypes/Model/Node.cc
        src/DataTypes/Model/Node.h
        src/DataTypes/Model/TextNode.cc
        src/DataTypes/Model/TextNode.h
        src/DataTypes/Model/InputNode.cc
        src/DataTypes/Model/InputNode.h
        src/DataTypes/Model/TableNode.cc
        src/DataTypes/Model/TableNode.h
        src/DataTypes/Model/RepeatInputNode.cc
        src/DataTypes/Model/RepeatInputNode.h
        src/MappingJob.cpp src/MappingJob.h)


add_executable(k1_parsing ${SOURCE_FILES})
target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
