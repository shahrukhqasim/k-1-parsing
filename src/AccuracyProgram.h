//
// Created by Shah Rukh Qasim on 6/25/16.
//
#include "OcrProgram.h"
#include "WordEntry.h"


#ifndef K_1_PARSING2_ACCURACYPROGRAM_H
#define K_1_PARSING2_ACCURACYPROGRAM_H


class AccuracyProgram {
private:
    string programOutputFile;
    string expectedOutputFile;

    string inputFile;
    string comparisonFile;

    Mat theImage;

    vector<WordEntry> tesseractOutput;
    vector<WordEntry> expectedOutput;
public:
    AccuracyProgram(string programOutputFile, string expectedOutputFilem, string inputFile, string comparisonFile);
    void run();
    static void runAccuracyTest();
    static void getWords(Json::Value root,vector<WordEntry>&outputVector);
private:
    void compare2(Json::Value rootProgram,Json::Value rootExpected);
    float testAccuracy();
};


#endif //K_1_PARSING2_ACCURACYPROGRAM_H
