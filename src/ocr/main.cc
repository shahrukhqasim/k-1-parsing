#include <iostream>

//#include "stdio.h"
#include <vector>
#include <tesseract/baseapi.h>
#include "opencv2/core/core.hpp"

#include <regex>

using namespace std;
using namespace tesseract;
using namespace cv;

#include "../ocr/AccuracyProgram.h"

/**
 * This function prints arguments help
 */
void printHelp() {
    cout<<"Argument 1 should either be -a or -o to run only accuracy test or OCR+accuracy test respectively."<<endl<<endl;
    cout<<" * You need to give path of a directory as second argument which must contin these folders:\n"
                  "\t * 1. programInput - should contain input images and files.txt which must contains names (not path) of input images files\n"
                  "\t * 2. expectedOutput - should contain the JSON files of expected output and files.txt which must contain names (not paths) of these JSON files. The order should match with (1)\n"
                  "\t * 3. programOutput - empty for OCR+Accuracy test. And for accuracy test, should contain JSON files containing output of OCR. There should also be a ocrJsonFiles.txt file in the folder hich specifies the JSON output files.\n"
                  "\t * 4. plottedDataComparison - an empty folder. Output is written to it. The files that are already there will be overwritten.\n\n";
    cout<<endl<<endl<<"All files.txt should contain consecutive filenames. First document has to appear at the top and so on"<<endl;
    // TODO: Show prompt for processing program and and model builder program
}


/**
 * This function is the main entry point of the program which takes command line arguments
 * @param[in] argc number of arguments which include program binary path
 * @param[in] argv the argument strings themselves
 *
 * @return insignificant. Will return 0 in most cases.
 */
int main(int argc, char**argv) {
    if(argc!=3) {
        cout<<"Error in arguments."<<endl;
        printHelp();
        exit(0);
    }

    // Firt argument is always -o or -a. If it is -o, we have to run OCR Job+Accuracy Job
    string job=argv[1];
    // Second argument is the path to the parent directory. Which must contain various folders
    string path=argv[2];


    // Compare the job and run the program accordingly
    if(job==string("-a")) {
        AccuracyProgram::runAccuracyTest(path);
    }
    else if(job==string("-o")) {

        OcrProgram::runOcrProgram(path);

    }
    else {
        cout<<"Error in argument 1. It can either be -a, -o, -p or -m"<<endl;
    }


	return 0;
}
