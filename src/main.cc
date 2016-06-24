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
#include "OcrProgram.h"

using namespace std;
using namespace tesseract;
using namespace cv;

#include "AccuracyProgram.h"


int main() {
    AccuracyProgram::runAccuracyTest();

//    string inputFolder="../../input";
//    string outputFolder="../../output";
//    for(int i=0;i<=10;i++) {
//        string fileName=to_string(i);
//        cout<<"Running on "<<fileName<<endl;
//        fileName+=".png";
//
//        OcrProgram runner(fileName,inputFolder,outputFolder);
//        runner.run();
//        cout<<"Run complete on "<<fileName<<endl;
//    }
	return 0;
}


