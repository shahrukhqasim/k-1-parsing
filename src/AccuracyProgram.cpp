//
// Created by shahrukhqasim on 6/25/16.
//

#include "AccuracyProgram.h"

float AccuracyProgram::testAccuracy(){
    int matches=0;

    for(int i=0; i<tesseractOutput.size(); i++){

        for(int j=0; j<expectedOutput.size(); j++){
            if(tesseractOutput[i].compare(expectedOutput[j])) {
                matches++;
                break;

            }
        }

    }

    int s=expectedOutput.size();
    if(s!=0) {
        return matches * 100 / expectedOutput.size();
    }
    else
        return 0;
}


AccuracyProgram::AccuracyProgram(string programOutputFile, string expectedOutputFile) {
    this->programOutputFile=programOutputFile;
    this->expectedOutputFile=expectedOutputFile;
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
        entry.setRect(Rect(l,t,r-l,b-r));
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

    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/programOutput/files.txt");
    ifstream inputStream2("/home/shahrukhqasim/Desktop/bb/All/expectedOutput/files.txt");
    std::string str;
    std::string str2;

    string inputFolder="/home/shahrukhqasim/Desktop/bb/All/programOutput";
    string outputFolder="/home/shahrukhqasim/Desktop/bb/All/expectedOutput";


    for(int i=0;i<50;i++) {
        getline(inputStream, str);
        getline(inputStream2, str2);
        cout<<"Running on "<<i<<endl;

        string programFilePath=inputFolder+"/"+str;
        string expectedFilePath=outputFolder+"/"+str2;

        AccuracyProgram(programFilePath,expectedFilePath).run();
    }
}

void AccuracyProgram::run() {
    Json::Value rootProgram;
    Json::Value rootExpected;

    ifstream programStream(programOutputFile);
    programStream>>rootProgram;

    ifstream expectedStream(expectedOutputFile);
    expectedStream>>rootExpected;

    compare2(rootProgram,rootExpected);
}