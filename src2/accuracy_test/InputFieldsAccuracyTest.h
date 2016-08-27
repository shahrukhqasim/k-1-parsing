//
// Created by shahrukhqasim on 8/28/16.
//

#ifndef K1_PARSING_INPUTFIELDSACCURACYTEST_H
#define K1_PARSING_INPUTFIELDSACCURACYTEST_H


#include <opencv2/core/mat.hpp>
#include <unordered_map>
#include <iostream>
#include "../../json/json/json.h"

class InputFieldsAccuracyTest {
    struct AccuracyData{
        std::string name;
        std::string value;
        cv::Rect region;
    };

    std::unordered_map<std::string,AccuracyData> groundTruthData;
    std::unordered_map<std::string,AccuracyData> outputData;
    std::unordered_map<std::string,std::string> bindingsData;

    Json::Value bindings;
    Json::Value groundTruth;
    Json::Value processorOutput;

    void readGroundTruth();
    void readOutputData();
    void readBindingsData();
public:
    InputFieldsAccuracyTest(const Json::Value &bindings, const Json::Value &groundTruth,
                            const Json::Value &modelOutput);
    float calculateAccuracy(bool outputCorrect=false, bool outputInCorrect=false);
};


#endif //K1_PARSING_INPUTFIELDSACCURACYTEST_H
