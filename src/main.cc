#include <iostream>

#include "stdio.h"
#include <vector>
#include <tesseract/baseapi.h>
#include "opencv2/core/core.hpp"

#include <regex>

using namespace std;
using namespace tesseract;
using namespace cv;

#include "AccuracyProgram.h"


/**
 * You need to give path of a directory as an agument which must contin these folders:
 * 1. programInput - should contain input images and files.txt which must contains names (not path) of input images files
 * 2. expectedOutput - should contain the JSON files of expected output and files.txt which must contain names (not paths) of these JSON files. The order should match with (1)
 * 3. programOutput - an empty folder. Output is written to it. The files that are already there will be overwritten.
 * 4. plottedDataComparison - an empty folder. Output is written to it. The files that are already there will be overwritten.
 */

int main(int argc, char**argv) {
    if(argc!=2) {
        cerr<<"Wrong number of arguments."<<endl;
        return 0;
    }
    cout<<argv[1]<<endl;

    string path=argv[1];
    if(path[path.length()-1]!='/')
        path=path+'/';

    OcrProgram::runOcrProgram(path);


	return 0;
}


