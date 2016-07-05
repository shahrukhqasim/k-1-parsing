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

#include "../json/json/json.h"

using namespace std;
using namespace tesseract;
using namespace cv;

#ifndef K_1_PARSING2_PROGRAM_H
#define K_1_PARSING2_PROGRAM_H

struct DocumentModelData {
	Point p1;
	Point p2;
	string exactKeyText;
};

struct SmallDistance
{
	inline bool operator() (const Point& struct1, const Point& struct2)
	{
		return (struct1.y-struct1.x > struct2.y-struct2.x);
	}
};

struct XFirst
{
	inline bool operator() (const Point& struct1, const Point& struct2)
	{
		return (struct1.x < struct2.x);
	}
};


class OcrProgram {
    RNG rng=RNG(12345);
    string inputFileName;
    string inputFolder;
    string outputFolder;
    Mat cleanedImage;
    string cleanedImageFileName;
    vector<OcrResult>data;
    vector<DocumentModelData>documentModelData;

    Mat originalImage;

    void loadImage(Mat&image, string fileName, int mode);
    void doSegmentation();
    void cleanImageAndWriteToDisk();
    void runOcr();
	void outputToJson();
    void outputResult();
    void oneToOneMatch();


    vector<Rect>segments;
public:
    void run();
    OcrProgram(string inputFileName, string inputFolder, string outputFolder);
	static void runOcrProgram(string path);
};


#endif //K_1_PARSING2_PROGRAM_H
