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
    float run();
    static void runAccuracyTest();
    static void getWords(Json::Value root,vector<WordEntry>&outputVector);
    static void cleanWords(vector<WordEntry>& root);
    static void ferjadTest();
private:
    void compare2(Json::Value rootProgram,Json::Value rootExpected);
    float testAccuracy();
    float accuracy;
    bool successful;
};


#endif //K_1_PARSING2_ACCURACYPROGRAM_H
