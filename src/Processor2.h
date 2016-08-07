//
// Created by shahrukhqasim on 7/28/16.
//


#include <iostream>

#include "stdio.h"
#include <vector>
#include <string>
#include <fstream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "../csv/CSVparser.hpp"
#include "HelperMethods.h"
#include "Preprocessor.h"
#include "TextualData.h"
#include <functional>
#include "Model/Node.h"

#include "Model/InputNode.h"

#ifndef K_1_PARSING2_PROCESSOR2_H
#define K_1_PARSING2_PROCESSOR2_H


struct GroundTruth {
    Rect rect;
    string value;
    bool taken=false;
public:
    GroundTruth(const Rect &rect, const string &value);
};


/**
 * This class represents the processing of text extracted from a form job.
 */
class Processor2 {
private:
    string imageFilePath;
    string textFilePath;
    string groundTruthFilePath;
    string outputFolder;
    string outputFileName;
    string modelFilePath;


    /**
     * 3 channel colored matrix of the input form
     */
    Mat image;

    /**
     * OCR words
     */
    vector<TextualData> words;


    /**
     * OCR merged words
     */
    vector<TextualData> mergedWords;

    /**
     * Represent the line data of Part 1 text
     */
    vector<TextualData>part1Data;

    /**
     * Represent the line data of Part 2 text
     */
    vector<TextualData>part2Data;

    /**
     * Represent the line data of Part 3 text
     */
    vector<TextualData>part3Data;

    /**
     * Represent the line data of header 1 text
     */
    vector<TextualData>header1Data;

    /**
     * Represent the line data of header 1 text
     */
    vector<TextualData>header2Data;

    shared_ptr<Node>documentNode;


    /**
     * Represents the x coordinate where the form was divided into two parts
     */
    int xDivisionCoordinate;

    vector<pair<string,Rect>>mappedGround;
    vector<pair<string,Rect>>mappedGroundP1;
    vector<pair<string,Rect>>mappedGroundP2;
    vector<pair<string,Rect>>mappedGroundP3;
    vector<pair<string,Rect>>mappedGroundH1;
    vector<pair<string,Rect>>mappedGroundH2;

    unordered_map<string,shared_ptr<GroundTruth>>groundTruth;

    RNG rng;

public:
    /**
     * Construct a processing job
     *
     * @param[in] imageFilePath represents the image of the form
     * @param[in] textFilePath represents the json file path of the OCR result
     * @param[in] groundTruthFilePath represents the groundTruth values
     * @param[in] modelFilePath represents the groundTruth values
     * @param[in] outputFolder represents the folder where to write results. File name will be picked from the last argument
     * @param[in] outputFileName represents the file name on basis on which, to write the output
     */
    Processor2(const string &imageFilePath, const string &textFilePath, const string &groundTruthFilePath, const string&modelFilePath,
              const string &outputFolder, const string &outputFileName);

    /**
     * Executes the job
     */
    float execute();

    /**
     * Reads field values from the JSON file.
     *
     * @param[in] root represents the JSON of the field values data
     * @param[out] outputVector represents the vector in which the data will be stored as return
     */
    static void getFieldValues(Json::Value root, vector<TextualData> &outputVector);

    /**
     * Searches in data to find the closest match to textToFind
     *
     * @param[in] data represents the text to search in
     * @param[in] textToFind represents the text which we have to search
     *
     * @return -1 if data is empty. Index in vector if found
     */
    static int findMinTextIndex(const vector<TextualData>&data,const string & textToFind);

    /**
     * Draws boxes on image
     *
     * @param[out] image is the image matrix to draw on
     * @param[in] data is the textual data to draw
     * @param[in] color is the color in which to draw
     *
     */
    static void drawBoxes(Mat &image, const vector<TextualData> &data, const Scalar &color);

    /**
     * Merges words from words into elemBoxes
     *
     * @param[in] words represent the input words
     * @param[out] elemBoxes represent the resultant elements
     */
    void mergeWordBoxes(const vector<TextualData>&words,vector<TextualData>&elemBoxes);

    /**
     * Build this program parameters from command line argument directory and run it on batch data
     */
    static void runProcessorProgram(string parentPath);

private:
    /**
     * Reads the data from files into memory as specified by constructor parameters
     */
    void readData();

    /**
     * Divide the data into 3 parts. Part 1 Part 2 and Part 3
     */
    void divideIntoParts();

    /**
     * Process Header 1
     */
    void processHeader1();

    /**
     * Process Header 2
     */
    void processHeader2();

    /**
     * Process Part 1
     */
    void processPart1();

    /**
     * Process and extract part 2 fields
     */
    void processPart2();

    /**
     * Process and extract part 3 fields
     */
    void processPart3();

    TextualData extractTextualDataType1(string key);
    TextualData extractTextualDataType2(string key);
    TextualData extractTextualDataType3(string key);

    string findTextWithRules(vector<std::function<bool(const TextualData&d)>>rules, const vector<TextualData>&data);

    string findTextWithRulesOnlyRightMost(vector<std::function<bool(const TextualData &d)>> rules,
                                          const vector<TextualData> &data);


    void outputDataToJson();
    void recursiveInputFieldsToJson(shared_ptr<Node>node);
    Json::Value outputJson;
    int lastIndexJson=0;

    void outputBindingLine(shared_ptr<Node>node,Rect region);


    int accuracyTests=0;
    int testsPassed=0;
    void testAccuracy(shared_ptr<InputNode>node);

public:

    /**
     * Checks if one rect is below another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is below b and false otherwise
     */
    static bool isBelow(const Rect& a, const Rect& b);


    /**
     * Checks if one rect is above another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is above b and false otherwise
     */
    static bool isAbove(const Rect& a, const Rect& b);



    /**
     * Checks if one rect is left to another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is left to b and false otherwise
     */
    static bool isLeftTo(const Rect& a, const Rect& b);



    /**
     * Checks if one rect is right to another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is right to b and false otherwise
     */
    static bool isRightTo(const Rect& a, const Rect& b);


};


#endif //K_1_PARSING2_PROCESSOR2_H
