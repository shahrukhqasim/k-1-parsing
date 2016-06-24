//
// Created by Shah Rukh Qasim on 6/24/16.
//

#include <iostream>

#include "stdio.h"
#include <vector>
#include <string>
#include <fstream>
// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <leptonica/allheaders.h>

#include "TesseractFinder.h"
#include "EditDistance.h"

#include "../csv/CSVparser.hpp"
#include "HelperMethods.h"
#include "Preprocessor.h"

using namespace std;
using namespace tesseract;
using namespace cv;

#ifndef K_1_PARSING2_PROGRAM_H
#define K_1_PARSING2_PROGRAM_H


class Program {
    RNG rng=RNG(12345);
    string inputFileName;
    string inputFolder;
    string outputFolder;
    Mat cleanedImage;
    string cleanedImageFileName;
    vector<OcrResult>data;

    Mat originalImage;

    void loadImage(Mat&image, string fileName, int mode);
    void doSegmentation();
    void cleanImageAndWriteToDisk();
    void runOcr();
    void outputResult();

    vector<Rect>segments;
public:
    void run();
    Program(string inputFileName, string inputFolder, string outputFolder);
};


#endif //K_1_PARSING2_PROGRAM_H
