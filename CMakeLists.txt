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
    src/TextualData.cc
    src/TextualData.h
    src/AccuracyProgram.cc
    src/AccuracyProgram.h
        src/Processor2.cc
    src/Processor2.h 
    src/Mapper.cc 
    src/Mapper.h
        src/Model/ModelParser.cc
        src/Model/ModelParser.h
    src/Model/Node.cc 
    src/Model/Node.h 
    src/Model/TextNode.cc 
    src/Model/TextNode.h 
    src/Model/InputNode.cc 
    src/Model/InputNode.h 
    src/Model/TableNode.cc 
    src/Model/TableNode.h 
    src/Model/RepeatInputNode.cc 
    src/Model/RepeatInputNode.h 
    src/Model/MappingJob.cc 
    src/Model/MappingJob.h)


add_executable(k1_parsing ${SOURCE_FILES})
target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
