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
        src/common/BasicLevenshteinDistance.cc
        src/common/BasicLevenshteinDistance.h
        src/common/HelperMethods.cc
        src/common/HelperMethods.h
        src/common/UnionFind.h
        src/common/Preprocessor.cc
        src/common/Preprocessor.h
        src/common/TextualData.cc
        src/common/TextualData.h
        src/common/TextDistanceInterface.h
        src/ocr/TesseractFinder.cc
        src/ocr/TesseractFinder.h
        src/ocr/OcrProgram.cc
        src/ocr/OcrProgram.h
        src/ocr/AccuracyProgram.cc
        src/ocr/AccuracyProgram.h
        src/parsing/interfaces/FormProcessorInterface.h
        src/parsing/interfaces/RawFormInterface.h
        src/parsing/interfaces/FormModelInterface.h
        src/parsing/interfaces/LabelledTreeNodeInterface.h
        src/parsing/interfaces/TreeFormNodeInterface.h
        src/parsing/interfaces/TreeFormModelInterface.h
        src/parsing/implementation/TreeFormProcessor.cc
        src/parsing/implementation/TreeFormProcessor.h
        src/parsing/implementation/TreeFormModel.cc
        src/parsing/implementation/TreeFormModel.h
        src/parsing/interfaces/TreeFormNodeProcessorInterface.h
        src/parsing/implementation/BasicTreeFormNode.cc
        src/parsing/implementation/BasicTreeFormNode.h
        src/parsing/implementation/TextTreeFormNode.cc
        src/parsing/implementation/TextTreeFormNode.h
        src/parsing/implementation/InputTreeFormNode.cc
        src/parsing/implementation/InputTreeFormNode.h
        src/parsing/implementation/RepeatInputTreeFormNode.cc
        src/parsing/implementation/RepeatInputTreeFormNode.h
        src/parsing/implementation/TableTreeFormNode.cc
        src/parsing/implementation/TableTreeFormNode.h
        src/parsing/implementation
        src/parsing/implementation/TreeFormNodeProcessor.cc
        src/parsing/implementation/TreeFormNodeProcessor.h
        src/parsing/interfaces/RuleInterface.h
        src/parsing/implementation/DivisionRule.cc
        src/parsing/implementation/DivisionRule.h
        src/parsing/implementation/DivisionRuleWithReference.cc
        src/parsing/implementation/DivisionRuleWithReference.h
        src/parsing/checkboxes/checkbox.cc
        src/parsing/checkboxes/checkbox.h
        src/parsing/implementation/BasicForm.cc
        src/parsing/implementation/BasicForm.h
        src/parsing/accuracy_test/InputFieldsAccuracyTest.cc
        src/parsing/accuracy_test/InputFieldsAccuracyTest.h)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


add_executable(k1_parsing src/parsing/main.cc ${SOURCE_FILES})
add_executable(k1_parsing_ocr src/ocr/main.cc ${SOURCE_FILES})

target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(k1_parsing_ocr lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
