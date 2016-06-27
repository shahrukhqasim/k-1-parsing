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

//void plot() {
//    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/expectedOutput/files.txt");
//    ifstream inputStream2("/home/shahrukhqasim/Desktop/bb/All/programInput/files.txt");
//    std::string str;
//    std::string str2;
//
//    string inputFolder="/home/shahrukhqasim/Desktop/bb/All/expectedOutput/";
//    string inputFolderImages="/home/shahrukhqasim/Desktop/bb/All/programInput/";
//    string outputFolder="/home/shahrukhqasim/Desktop/bb/All/plottedDataGiven/";
//
//    while(getline(inputStream, str)) {
//        getline(inputStream2, str2);
//
//        string inputFile=inputFolder+str;
//        cout<<"Running on "<<inputFile<<endl;
//
//        Json::Value root;
//        ifstream jsonStream(inputFile);
//        jsonStream>>root;
//        vector<WordEntry>words;
//        AccuracyProgram::getWords(root,words);
//        cout<<"Words: "<<words.size()<<endl;
//        string inputImage=inputFolderImages+str2;
//        cout<<"with: "<<inputImage<<endl<<endl;
//        Mat image = imread(inputImage, 1);   // Read the file
//        if (!image.data)                      // Check for invalid input
//        {
//            cout << "Could not open or find the image: " << image << std::endl;
//            exit(0);
//        }
//
//
//        Scalar red(0,0,255);
//        for(int i=0;i<words.size();i++) {
//            //cout<<"Drawing rectangle: "<<words[i].getRect()<<endl;
//            rectangle(image,words[i].getRect(),red,3,8,0);
//        }
//
//        imwrite(outputFolder+str2,image);
//    }
//}



int main() {
    //AccuracyProgram::runAccuracyTest();
    OcrProgram::runOcrProgram();
	return 0;
}


