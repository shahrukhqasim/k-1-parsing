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
#include "Processor2.h"
#include "Model/ModelBuilder.h"

/**
 * This function prints arguments help
 */
void printHelp() {
    cout<<"Argument 1 should either be -a, -o, -p or -m to run only accuracy test, OCR+accuracy test, processing program or model builder programs respectively respectively."<<endl<<endl;
    cout<<" * In case of first two You need to give path of a directory as an agument which must contin these folders:\n"
                  " * 1. programInput - should contain input images and files.txt which must contains names (not path) of input images files\n"
                  " * 2. expectedOutput - should contain the JSON files of expected output and files.txt which must contain names (not paths) of these JSON files. The order should match with (1)\n"
                  " * 3. programOutput - empty for OCR+Accuracy test. And for accuracy test, should contain JSON files containing output of OCR. There should also be a ocrJsonFiles.txt file in the folder hich specifies the JSON output files.\n"
                  " * 4. plottedDataComparison - an empty folder. Output is written to it. The files that are already there will be overwritten.";
    cout<<endl<<"All files.txt should contain consecutive filenames. First document has to appear at the top and so on"<<endl;
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
    else if(job==string("-p")) {
        Processor2::runProcessorProgram(path);

    }
    else if(job==string("-m")) {
        ModelBuilder::runModelBuilderProgram(path);

    }
    else {
        cout<<"Error in argument 1. It can either be -p or -a"<<endl;
    }


	return 0;
}

int main2() {
    ifstream theFile("/home/shahrukhqasim/Desktop/data.json");
    Json::Value parsedData;
    theFile>>parsedData;

    Mat image=imread("/home/shahrukhqasim/Desktop/image.png",1);

    parsedData=parsedData["Pages"][0]["Fields"];

    vector<Rect>rectangles;

    for(int i=0;i<parsedData.size();i++) {
        Json::Value region=parsedData[i]["Region"];

        int l=region["l"].asInt();
        int t=region["t"].asInt();
        int r=region["r"].asInt();
        int b=region["b"].asInt();

        rectangles.push_back(Rect(l,t,r-l,b-t));

        rectangle(image,Rect(l,t,r-l,b-t) , Scalar(255,0,255), 3, 8,0);
    }

    imwrite("/home/shahrukhqasim/Desktop/ouput.png",image);

    return 0;
}