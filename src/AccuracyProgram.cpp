//
// Created by shahrukhqasim on 6/25/16.
//

#include "AccuracyProgram.h"

float AccuracyProgram::testAccuracy(){
    int size=expectedOutput.size();
    int matches=0;

    // Those elements which are found are entered into this vector
    vector<WordEntry>found;

    // Those elements from tesseract output which are matched to anything
    vector<WordEntry>ocrMatched;

    for(int i=0; i<tesseractOutput.size(); i++){
        for(int j=0; j<expectedOutput.size(); j++){
            if(tesseractOutput[i].compare(expectedOutput[j])) {
                // Push the found element into found vector
                found.push_back(expectedOutput[j]);

                // Remove it from expected output. It cannot occur twice
                expectedOutput.erase(expectedOutput.begin()+j);

                matches++;
                break;

            }
        }

        // Those elements from tesseract output which are not matched to anything
        vector<WordEntry>notMatched;
        for(int i=0;i<tesseractOutput.size();i++) {
            bool matched=false;
            for(int j=0;j<ocrMatched.size();j++) {
                if(tesseractOutput[i].equals(ocrMatched[j])) {
                    matched=true;
                    break;
                }
            }
            if(!matched) {
                notMatched.push_back(tesseractOutput[i]);
            }
        }


        // Now expectedOutput contains not found elements and found vector contains found elements
        // Moreover ocrMatched vector contains those elements which were not matched to anything
        // when compared

        // First draw all the found elements in green
        Scalar green(0,255,0);
        for(int i=0;i<found.size();i++) {
            rectangle(theImage,found[i].getRect(),green,3,8,0);
        }


        // Then draw all the not found elements of expected output in red
        Scalar red(0,0,255);
        for(int i=0;i<found.size();i++) {
            rectangle(theImage,expectedOutput[i].getRect(),red,3,8,0);
        }

        // Then draw all the not found elements ocr output in blue
        Scalar blue(255,0,0);
        for(int i=0;i<tesseractOutput.size();i++) {
            rectangle(theImage,tesseractOutput[i].getRect(),blue,3,8,0);
        }


    }

    if(size!=0) {
        return matches * 100.0 / size;
    }
    else
        return 0;
}


AccuracyProgram::AccuracyProgram(string programOutputFile, string expectedOutputFilename, string inputFile, string comparisonFile) {
    this->programOutputFile=programOutputFile;
    this->expectedOutputFile=expectedOutputFilename;
    this->inputFile=inputFile;
    this->comparisonFile=comparisonFile;
}

void AccuracyProgram::getWords(Json::Value root,vector<WordEntry>&outputVector) {
    root=root["Pages"][0];
    Json::Value words=root["Words"];
    for(int i=0;i<words.size();i++) {
        Json::Value word=words[i];
        string value = word["Value"].asString();
        Json::Value rectangle=word["Region"];

        int t=rectangle["t"].asInt();
        int l=rectangle["l"].asInt();
        int b=rectangle["b"].asInt();
        int r=rectangle["r"].asInt();

        WordEntry entry;
        entry.setRect(Rect(l,t,r-l,b-t));
        entry.setString(value);
        outputVector.push_back(entry);
    }

}


void AccuracyProgram::compare2(Json::Value rootProgram,Json::Value rootExpected) {
    getWords(rootProgram,tesseractOutput);
    getWords(rootExpected,expectedOutput);

    float accuracy=testAccuracy();
    cout<<"Accuracy is "<<accuracy<<endl;
}

void AccuracyProgram::runAccuracyTest() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/programOutput/ocrJsonFiles.txt");
    ifstream inputStream2("/home/shahrukhqasim/Desktop/bb/All/expectedOutput/files.txt");
    ifstream inputStream3("/home/shahrukhqasim/Desktop/bb/All/programInput/files.txt");
    std::string str;
    std::string str2;
    std::string str3;

    string inputFolder="/home/shahrukhqasim/Desktop/bb/All/programOutput";
    string outputFolder="/home/shahrukhqasim/Desktop/bb/All/expectedOutput";


    const string plotOutputFolder="/home/shahrukhqasim/Desktop/bb/All/plottedDataComparison/";


    // TODO: To run extensive testing on single image
    while(getline(inputStream, str)) {
        getline(inputStream2, str2);
        getline(inputStream3, str3);

        string inputImagePath="/home/shahrukhqasim/Desktop/bb/All/programInput/"+str3;
        string plotImagePath=plotOutputFolder+str3;
        string programFilePath=inputFolder+"/"+str;
        string expectedFilePath=outputFolder+"/"+str2;

        cout<<"Running on "<<programFilePath<<endl;
        AccuracyProgram(programFilePath,expectedFilePath,inputImagePath,plotImagePath).run();
    }
}

void AccuracyProgram::run() {
    Json::Value rootProgram;
    Json::Value rootExpected;

    ifstream programStream(programOutputFile);
    programStream>>rootProgram;

    ifstream expectedStream(expectedOutputFile);
    cout<<"B: "<<expectedOutputFile<<endl;
    expectedStream>>rootExpected;

    cout<<inputFile<<endl;
    Mat image=imread(inputFile,0);
    if(!image.data) {

        cerr<<"Could not read the input image file";
        exit(0);
    }
    cvtColor(image,theImage,CV_GRAY2RGB);

    compare2(rootProgram,rootExpected);

    HelperMethods::outputImage(theImage,comparisonFile);
}