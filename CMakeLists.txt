cmake_minimum_required(VERSION 3.5)
project(k1_parsing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -lpthread")

link_directories(/usr/local/lib/ /usr/lib/)
include_directories(/usr/local/include/)

set(SOURCE_FILES
        src/main.cc
#        src2/mainTest.cpp
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
        src/Model/MappingJob.h src2/interfaces/FormProcessorInterface.h src2/interfaces/RawFormInterface.h src2/interfaces/FormModelInterface.h src2/interfaces/LabelledTreeNodeInterface.h src2/interfaces/TreeFormNodeInterface.h src2/interfaces/TreeFormModelInterface.h src2/implementation/TreeFormProcessor.cpp src2/implementation/TreeFormProcessor.h src2/implementation/TreeFormModel.cpp src2/implementation/TreeFormModel.h src2/interfaces/TreeFormNodeProcessorInterface.h src2/implementation/BasicTreeFormNode.cpp src2/implementation/BasicTreeFormNode.h src2/implementation/TextTreeFormNode.cpp src2/implementation/TextTreeFormNode.h src2/implementation/InputTreeFormNode.cpp src2/implementation/InputTreeFormNode.h src2/implementation/RepeatInputTreeFormNode.cpp src2/implementation/RepeatInputTreeFormNode.h src2/implementation/TableTreeFormNode.cpp src2/implementation/TableTreeFormNode.h src2/implementation src2/implementation/TreeFormNodeProcessor.cpp src2/implementation/TreeFormNodeProcessor.h src2/interfaces/RuleInterface.h src2/implementation/DivisionRule.cpp src2/implementation/DivisionRule.h src2/implementation/DivisionRuleWithReference.cpp src2/implementation/DivisionRuleWithReference.h src2/checkboxes/checkbox.cc src2/checkboxes/checkbox.h src2/checkboxes/concomp.cc src2/checkboxes/concomp.h src2/checkboxes/union-find.h src2/implementation/BasicForm.cpp src2/implementation/BasicForm.h src2/accuracy_test/InputFieldsAccuracyTest.cpp src2/accuracy_test/InputFieldsAccuracyTest.h)


add_executable(k1_parsing ${SOURCE_FILES})
target_link_libraries(k1_parsing lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
