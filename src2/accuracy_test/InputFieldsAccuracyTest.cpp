//
// Created by shahrukhqasim on 8/28/16.
//

#include "InputFieldsAccuracyTest.h"
#include "../../src/HelperMethods.h"


InputFieldsAccuracyTest::InputFieldsAccuracyTest(const Json::Value &bindings, const Json::Value &groundTruth,
                                                 const Json::Value &modelOutput) : bindings(bindings),
                                                                                   groundTruth(groundTruth),
                                                                                   processorOutput(modelOutput) {}

float InputFieldsAccuracyTest::calculateAccuracy(bool outputCorrect, bool outputInCorrect) {
    readGroundTruth();
    readOutputData();
    readBindingsData();

    int count=0;
    float correct;
    for(auto i:bindingsData) {
        auto iteratorGt=groundTruthData.find(i.first);
        auto iteratorOutput=outputData.find(i.second);

        if(iteratorGt== groundTruthData.end()) {
            std::cerr<<"Ground truth entry not found in ground truth: "<<i.first<<std::endl;
            continue;
        }

        if(iteratorOutput== outputData.end()) {
            std::cerr<<"Output model id not found in output: "<<i.second<<std::endl;
            continue;
        }
        count++;

        if(HelperMethods::nearEqualComparison(iteratorGt->second.value,iteratorOutput->second.value)) {
            if(outputCorrect) {
                std::cout<<"Matched for "<<iteratorOutput->first<<": "<<iteratorGt->second.value+"="<<iteratorOutput->second.value<<std::endl;
            }
            correct++;
        }
        else {
            if(outputInCorrect) {
                std::cout<<"Not matched for "<<iteratorOutput->first<<": "<<iteratorGt->second.value+"!="<<iteratorOutput->second.value<<std::endl;
            }
        }

    }

    if(count!=0) {
        return 100.0*correct/count;
    }

    return -1;
}

void InputFieldsAccuracyTest::readGroundTruth() {
    Json::Value groundTruth = InputFieldsAccuracyTest::groundTruth["Pages"][0]["Fields"];

    for (int i = 0; i < groundTruth.size(); i++) {
        std::string name = groundTruth[i]["Name"].asString();
        std::string value = groundTruth[i]["Value"].asString();
        Json::Value region = groundTruth[i]["Region"];

        int l = region["l"].asInt();
        int t = region["t"].asInt();
        int r = region["r"].asInt();
        int b = region["b"].asInt();

        cv::Rect regionRect(l, t, r - l, b - t);

        groundTruthData[name]={name,value,regionRect};
    }
}

void InputFieldsAccuracyTest::readOutputData() {
    Json::Value processorOutput= InputFieldsAccuracyTest::processorOutput["Pages"][0]["Fields"];

    for (int i = 0; i < processorOutput.size(); i++) {
        std::string name = processorOutput[i]["Id"].asString();
        std::string value = processorOutput[i]["Value"].asString();
        Json::Value region = processorOutput[i]["Region"];

        int l = region["l"].asInt();
        int t = region["t"].asInt();
        int r = region["r"].asInt();
        int b = region["b"].asInt();

        cv::Rect regionRect(l, t, r - l, b - t);

        outputData[name]={name,value,regionRect};

    }
}

void InputFieldsAccuracyTest::readBindingsData() {
    for (int i = 0; i < bindings.size(); i++) {
        std::string model = bindings[i]["Model"].asString();
        std::string bindedGt = bindings[i]["Binded"].asString();

        bindingsData[bindedGt]=model;
    }
}
