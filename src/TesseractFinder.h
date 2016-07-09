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



/**
 * Contains an OCR result entry.
 * TODO: change it to TextualData class
 */
struct OcrResult {
    Point p1;
    Point p2;
    string text;
};

/**
 * This class represents a tesseract OCR job
 */
class TesseractFinder {
private:
    /**
     * The path of the input image
     */
    string imagePath;


    /**
     * To extract results from a tesseract job
     *
     * @param[in] api represents pointer to the TesseractBaseAPI from which recognition was done
     */
    void iterate(tesseract::TessBaseAPI *api);


    /**
     * Run OCR
     */
    void recognizeText();

    vector<OcrResult> data;

    static const int MODE_FULL=0;
    static const int MODE_BOXES=1;
    int mode;

    /**
     * Segment boxes, if it is a segmented image
     */
    vector<Rect>boxes;

public:
    /**
     * If it is an unsegmented image
     *
     * @param[in] imagePath represents the input image file path on which OCR will run
     *
     */
    TesseractFinder(const string &imagePath);

    /**
     * If it is a segmented image
     *
     * @param[in] imagePath represents the input image path on which OCR will run
     *
     * boxes contain the segments
     */
    TesseractFinder(const string &imagePath, const vector<Rect>& boxes);

    /**
     * Run OCR
     */
    void run();

    /**
     * To retrieve results
     *
     * @return the reconized data
     */
    vector<OcrResult> getRecognizedData();

};


#endif
