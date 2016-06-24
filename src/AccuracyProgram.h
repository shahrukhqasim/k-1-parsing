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

    vector<WordEntry> tesseractOutput;
    vector<WordEntry> expectedOutput;
public:
    AccuracyProgram(string programOutputFile, string expectedOutputFile);
    void run();
    static void runAccuracyTest();

private:
    void getWords(Json::Value root,vector<WordEntry>&outputVector);
    void compare2(Json::Value rootProgram,Json::Value rootExpected);
    float testAccuracy();
};


#endif //K_1_PARSING2_ACCURACYPROGRAM_H
