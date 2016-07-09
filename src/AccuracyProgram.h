//
// Created by Shah Rukh Qasim on 6/25/16.
//
#include "OcrProgram.h"
#include "TextualData.h"


#ifndef K_1_PARSING2_ACCURACYPROGRAM_H
#define K_1_PARSING2_ACCURACYPROGRAM_H


class AccuracyProgram {
private:
    /**
     * Contains the complete path of the OCR program output JSON file
     */
    string programOutputFile;

    /**
     * Contains the complete path of the grouth truth JSON file
     */
    string expectedOutputFile;

    /**
     * Contains complete path of the input image file (on which OCR was run)
     */
    string inputFile;

    /**
     * Contains complete path of the plotted image file (output)
     */
    string comparisonFile;

    /**
     * The input image is loaded into memory for plotting purposes.
     * Image is loaded from inputFile path
     */
    Mat theImage;

    /**
     * Data vector of OCR result
     */
    vector<TextualData> ocrOutput;

    /**
     * Data vector of the ground truth
     */
    vector<TextualData> expectedOutput;
public:
    /**
     * This is a static function which runs an accuracy test on the path structure provided.
     *
     * @param[in] path The path to the root directory which can either end with / or not
     */
    static void runAccuracyTest(string path);

    /**
     * This is the constructor of the AccuracyTest case
     *
     * @param[in] programOutputFile must contain the complete path (or relative) of the OCR result (JSON)
     * @param[in] expectedOutputFile must contain the complete path (or relative) of the ground truth (JSON)
     * @param[in] inputFile must contain the complete path (or relative) of input image on which the OCR was run (for plotting purposes)
     * @param[in] comparisonFile must contain the complete path (or relative) of the comparison file to be generated
     */
    AccuracyProgram(string programOutputFile, string expectedOutputFilem, string inputFile, string comparisonFile);

    /**
     * This runs the test case (data is already provided in the constructor)
     *
     * @return the percentage accuracy of the test
     */
    float run();

    /**
     * This function is static because it does not depend on any class level variables. root
     * must contain the JSON of the text (string and bounding box) on the agreed upon
     * format of the grouth truth
     *
     * @param[in] root must containt he JSON
     * @param[out] outputVector is the output vector which will store the results
     *
     */
    static void getWords(Json::Value root,vector<TextualData>&outputVector);

    /**
     * This function is static because it does not depend on any class level variables. It cleans
     * data from any non-alpha numeric data
     *
     * @param[in] root must contain the vector to the data
     */
    static void cleanWords(vector<TextualData>& root);
private:
    /**
     * This function compares two data jsons. It then calls testAccuracy function for the real job
     *
     * @param[in] rootProgram must contain the OCR result
     * @param[in[ rootExpected must contain the ground truth
     */
    void compare2(Json::Value rootProgram,Json::Value rootExpected);


    /**
     * This function runs the comparison test. It takes input data fed in to class level vectors
     *
     * @return the accuracy percentage value of the test
     */
    float testAccuracy();

    /**
     * The resultant accuracy is stored in this variable
     */
    float accuracy;

    /**
     * Contains if ground truth is empty or not
     */
    bool successful;
};


#endif //K_1_PARSING2_ACCURACYPROGRAM_H
