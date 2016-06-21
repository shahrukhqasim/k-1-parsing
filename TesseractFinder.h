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
  void recognizeText();
  vector<KeyData> data;

public:
  TesseractFinder(const string&imagePath);
  void run();
  vector<KeyData> getRecognizedData();

};


#endif
