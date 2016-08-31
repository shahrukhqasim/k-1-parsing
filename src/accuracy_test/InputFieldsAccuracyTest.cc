
#include "InputFieldsAccuracyTest.h"
#include "../HelperMethods.h"
#include <regex>


InputFieldsAccuracyTest::InputFieldsAccuracyTest(const Json::Value &bindings, const Json::Value &groundTruth,
                                                 const Json::Value &modelOutput) : bindings(bindings),
                                                                                   groundTruth(groundTruth),
                                                                                   processorOutput(modelOutput) {}

float InputFieldsAccuracyTest::calculateAccuracy(bool outputErrors, bool outputCorrect, bool outputInCorrect) {
    readGroundTruth();
    readOutputData();
    readBindingsData();

    int count = 0;
    float correct=0;
    for (auto i:bindingsData) {
        auto iteratorGt = groundTruthData.find(i.first);
        auto iteratorOutput = outputData.find(i.second);

        if (iteratorGt == groundTruthData.end()) {
            if (outputErrors)
                std::cerr << "Ground truth entry not found in ground truth: " << i.first << std::endl;


//            if(imageSet) {
//                cv::rectangle(drawImage, iteratorGt->second.region, cv::Scalar(0, 0, 255), 3, 8, 0);
//            }

            continue;
        }

        std::string expectedValue=iteratorGt->second.value;
        std::string myValue;

        bool repeatInputId = false;
        std::regex regexRepeatInput(".*[:][0-9-]+");
        if (iteratorOutput == outputData.end()) {
            if (std::regex_match(i.second, regexRepeatInput)) {
                repeatInputId = true;
            } else {
                if (outputErrors)
                    std::cerr << "Output model id not found in output: " << i.second << std::endl;


//                if (imageSet) {
//                    cv::rectangle(drawImage, iteratorGt->second.region, cv::Scalar(0, 0, 255), 3, 8, 0);
//                }

                continue;
            }
        }

        if(repeatInputId)
            myValue="";
        else
            myValue=iteratorOutput->second.value;

        count++;

        if (HelperMethods::nearEqualComparison(expectedValue,myValue)) {
            if (outputCorrect) {
                std::cout << "Matched for " << i.second << ": " << expectedValue + "="
                          << myValue << std::endl;
            }

            if (imageSet) {
                cv::rectangle(drawImage, iteratorGt->second.region, cv::Scalar(0, 255, 0), 3, 8, 0);
            }

            correct++;
        } else {
            if (outputInCorrect) {
                std::cout << "Not matched for " << i.second << ": " << expectedValue + "!="
                          << myValue << std::endl;
            }

            if (imageSet) {
                cv::rectangle(drawImage, iteratorGt->second.region, cv::Scalar(0, 0, 255), 3, 8, 0);
            }
        }
    }

    if (count != 0) {
        return 100.0 * correct / count;
    }

    return -1;
}

void InputFieldsAccuracyTest::readGroundTruth() {
    Json::Value groundTruth = InputFieldsAccuracyTest::groundTruth["Pages"][0]["Fields"];

    for (unsigned int i = 0; i < groundTruth.size(); i++) {
        std::string name = groundTruth[i]["Name"].asString();
        std::string value = groundTruth[i]["Value"].asString();
        Json::Value region = groundTruth[i]["Region"];

        int l = region["l"].asInt();
        int t = region["t"].asInt();
        int r = region["r"].asInt();
        int b = region["b"].asInt();

        cv::Rect regionRect(l, t, r - l, b - t);

        groundTruthData[name] = {name, value, regionRect};
    }
}

void InputFieldsAccuracyTest::readOutputData() {
    Json::Value processorOutput = InputFieldsAccuracyTest::processorOutput["Pages"][0]["Fields"];

    for (unsigned int i = 0; i < processorOutput.size(); i++) {
        std::string name = processorOutput[i]["Id"].asString();
        std::string value = processorOutput[i]["Value"].asString();
        Json::Value region = processorOutput[i]["Region"];

        int l = region["l"].asInt();
        int t = region["t"].asInt();
        int r = region["r"].asInt();
        int b = region["b"].asInt();

        cv::Rect regionRect(l, t, r - l, b - t);

        outputData[name] = {name, value, regionRect};

    }
}

void InputFieldsAccuracyTest::readBindingsData() {
    for (unsigned int i = 0; i < bindings.size(); i++) {
        std::string model = bindings[i]["Model"].asString();
        std::string bindedGt = bindings[i]["Binded"].asString();

        bindingsData[bindedGt] = model;
    }
}

InputFieldsAccuracyTest::InputFieldsAccuracyTest(const Json::Value &bindings, const Json::Value &groundTruth,
                                                 const Json::Value &processorOutput, const cv::Mat &drawImage)
        : bindings(bindings), groundTruth(groundTruth), processorOutput(processorOutput) {
    InputFieldsAccuracyTest::drawImage = drawImage.clone();
    imageSet = true;
}

cv::Mat InputFieldsAccuracyTest::getErrorsImage() const {
    return drawImage.clone();
}
