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
        src/EditDistanceFinder.cc
        src/EditDistanceFinder.h
    src/HelperMethods.cc
    src/HelperMethods.h
    src/main.cc
    src/TesseractFinder.cc
    src/TesseractFinder.h
    src/UnionFind.h
        src/PreProcessingHelperMethods.cc
        src/PreProcessingHelperMethods.h
    src/OcrProgram.cc
    src/OcrProgram.h
        src/data_types/TextualData.cc
        src/data_types/TextualData.h
        src/OcrEvaluation.cc
        src/OcrEvaluation.h
        src/FieldsExtracter.cc
        src/FieldsExtracter.h
        src/NodesMapper.cc
        src/NodesMapper.h
        src/DocumentModel.cc
        src/DocumentModel.h
        src/data_types/model/Node.cc
        src/data_types/model/Node.h
        src/data_types/model/TextNode.cc
        src/data_types/model/TextNode.h
        src/data_types/model/InputNode.cc
        src/data_types/model/InputNode.h
        src/data_types/model/TableNode.cc
        src/data_types/model/TableNode.h
        src/data_types/model/RepeatInputNode.cc
        src/data_types/model/RepeatInputNode.h
        src/MappingJob.cpp src/MappingJob.h)


add_executable(k1_parsing ${SOURCE_FILES})
target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
