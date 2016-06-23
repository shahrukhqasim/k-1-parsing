#ifndef TESSERACT_FINDER_
#define TESSERACT_FINDER_

#include <iostream>
#include "stdio.h"
#include <string>
#include <fstream>
// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <leptonica/allheaders.h>
#include <leptonica/allheaders.h>
// OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <leptonica/allheaders.h>
#include <vector>

using namespace std;
using namespace cv;



struct KeyData {
    Point p1;
    Point p2;
    string text;
};

class TesseractFinder {
private:
    string imagePath;


    void iterate(tesseract::TessBaseAPI *api);
    void recognizeText();

    vector<KeyData> data;

    static const int MODE_FULL=0;
    static const int MODE_BOXES=1;
    int mode;

    vector<Rect>boxes;

public:
    TesseractFinder(const string &imagePath);
    TesseractFinder(const string &imagePath, const vector<Rect>& boxes);

    void run();

    vector<KeyData> getRecognizedData();

};


#endif
