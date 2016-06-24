//
// Created by Talha Rehman on 6/24/16.
//
#include "OcrProgram.h"

#ifndef K_1_PARSING2_WORDENTRY_H
#define K_1_PARSING2_WORDENTRY_H


// This class represents a word defined by its string and bounding box
class WordEntry {

private:
    Rect boundingBox;
    string word;

public:

    bool compare(WordEntry &expectedOutput);
    Rect getRect();
    string getWord();
    void setRect(Rect input);
    void setString(string input);

};

#endif //K_1_PARSING2_WORDENTRY_H
