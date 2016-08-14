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

#include "../csv/CSVparser.h"
#include "HelperMethods.h"
#include "Preprocessor.h"
#include "TextualData.h"
#include <functional>

#ifndef K_1_PARSING2_PROCESSOR_H
#define K_1_PARSING2_PROCESSOR_H

/**
 * This class represents the processing of text extracted from a form job.
 */
class Processor {
private:
    std::string imageFilePath;
    std::string textFilePath;
    std::string groundTruthFilePath;
    std::string outputFolder;
    std::string outputFileName;

    /**
     * 3 channel colored matrix of the input form
     */
    cv::Mat image;

    /**
     * OCR words
     */
    std::vector<TextualData> words;


    /**
     * OCR merged words
     */
    std::vector<TextualData> mergedWords;

    /**
     * Ground truth fields values
     */
    std::vector<TextualData> fields;


    /**
     * Represent the line data of Part 1 text
     */
    std::vector<TextualData>part1Data;

    /**
     * Represent the line data of Part 2 text
     */
    std::vector<TextualData>part2Data;

    /**
     * Represent the line data of Part 3 text
     */
    std::vector<TextualData>part3Data;


    /**
     * Represents the x coordinate where the form was divided into two parts
     */
    int xDivisionCoordinate;

public:
    /**
     * Construct a processing job
     *
     * @param[in] imageFilePath represents the image of the form
     * @param[in] textFilePath represents the json file path of the OCR result
     * @param[in] groundTruthFilePath represents the groundTruth values
     * @param[in] outputFolder represents the folder where to write results. File name will be picked from the last argument
     * @param[in] outputFileName represents the file name on basis on which, to write the output
     */
    Processor(const std::string &imageFilePath, const std::string &textFilePath, const std::string &groundTruthFilePath,
              const std::string &outputFolder, const std::string &outputFileName);

    /**
     * Executes the job
     */
    void execute();

    /**
     * Reads field values from the JSON file.
     *
     * @param[in] root represents the JSON of the field values data
     * @param[out] outputVector represents the std::vector in which the data will be stored as return
     */
    static void getFieldValues(Json::Value root, std::vector<TextualData> &outputVector);

    /**
     * Searches in data to find the closest match to textToFind
     *
     * @param[in] data represents the text to search in
     * @param[in] textToFind represents the text which we have to search
     *
     * @return -1 if data is empty. Index in std::vector if found
     */
    static int findMinTextIndex(const std::vector<TextualData>&data,const std::string & textToFind);

    /**
     * Draws boxes on image
     *
     * @param[out] image is the image matrix to draw on
     * @param[in] data is the textual data to draw
     * @param[in] color is the color in which to draw
     *
     */
    static void drawBoxes(cv::Mat &image, const std::vector<TextualData> &data, const cv::Scalar &color);

    /**
     * Merges words from words into elemBoxes
     *
     * @param[in] words represent the input words
     * @param[out] elemBoxes represent the resultant elements
     */
    void mergeWordBoxes(const std::vector<TextualData>&words,std::vector<TextualData>&elemBoxes);

    /**
     * Build this program parameters from command line argument directory and run it on batch data
     */
    static void runProcessorProgram(std::string parentPath);

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

    TextualData extractTextualDataType1(std::string key);
    TextualData extractTextualDataType2(std::string key);
    TextualData extractTextualDataType3(std::string key);

    std::string findTextWithRules(std::vector<std::function<bool(const TextualData&d)>>rules, const std::vector<TextualData>&data);

    std::string findTextWithRulesOnlyRightMost(std::vector<std::function<bool(const TextualData &d)>> rules,
                                          const std::vector<TextualData> &data);


    /**
     * Checks if one rect is below another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is below b and false otherwise
     */
    static bool isBelow(const cv::Rect& a, const cv::Rect& b);


    /**
     * Checks if one rect is above another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is above b and false otherwise
     */
    static bool isAbove(const cv::Rect& a, const cv::Rect& b);



    /**
     * Checks if one rect is left to another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is left to b and false otherwise
     */
    static bool isLeftTo(const cv::Rect& a, const cv::Rect& b);



    /**
     * Checks if one rect is right to another
     *
     * @param[in] a represents the test rectangle
     * @param[in] b represents the rectangle to compare agains
     *
     * @return true if a is right to b and false otherwise
     */
    static bool isRightTo(const cv::Rect& a, const cv::Rect& b);


};


#endif //K_1_PARSING2_PROCESSOR_H
