cmake_minimum_required(VERSION 3.5)
project(k1_parsing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -lpthread")

link_directories(/usr/local/lib/ /usr/lib/)
include_directories(/usr/local/include/)

set(SOURCE_FILES
        src/main.cc
        csv/CSVparser.cc
        csv/CSVparser.h
        json/jsoncpp.cc
        json/json/json.h
        json/json/json-forwards.h
        src/EditDistance.cc
        src/EditDistance.h
        src/HelperMethods.cc
        src/HelperMethods.h
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
        src/interfaces/FormProcessorInterface.h
        src/interfaces/RawFormInterface.h
        src/interfaces/FormModelInterface.h
        src/interfaces/LabelledTreeNodeInterface.h
        src/interfaces/TreeFormNodeInterface.h
        src/interfaces/TreeFormModelInterface.h
        src/implementation/TreeFormProcessor.cc
        src/implementation/TreeFormProcessor.h
        src/implementation/TreeFormModel.cc
        src/implementation/TreeFormModel.h
        src/interfaces/TreeFormNodeProcessorInterface.h
        src/implementation/BasicTreeFormNode.cc
        src/implementation/BasicTreeFormNode.h
        src/implementation/TextTreeFormNode.cc
        src/implementation/TextTreeFormNode.h
        src/implementation/InputTreeFormNode.cc
        src/implementation/InputTreeFormNode.h
        src/implementation/RepeatInputTreeFormNode.cc
        src/implementation/RepeatInputTreeFormNode.h
        src/implementation/TableTreeFormNode.cc
        src/implementation/TableTreeFormNode.h
        src/implementation
        src/implementation/TreeFormNodeProcessor.cc
        src/implementation/TreeFormNodeProcessor.h
        src/interfaces/RuleInterface.h
        src/implementation/DivisionRule.cc
        src/implementation/DivisionRule.h
        src/implementation/DivisionRuleWithReference.cc
        src/implementation/DivisionRuleWithReference.h
        src/checkboxes/checkbox.cc
        src/checkboxes/checkbox.h
        src/implementation/BasicForm.cc
        src/implementation/BasicForm.h
        src/accuracy_test/InputFieldsAccuracyTest.cc
        src/accuracy_test/InputFieldsAccuracyTest.h)



add_executable(k1_parsing ${SOURCE_FILES})
target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
