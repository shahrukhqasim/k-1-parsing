//
// Created by shahrukhqasim on 6/25/16.
//

#include "AccuracyProgram.h"

float AccuracyProgram::testAccuracy(){
    vector<WordEntry> expected2=vector<WordEntry>(expectedOutput);

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


//     Then draw all the not found elements of expected output in red
    Scalar red(0,0,255);
    for(int i=0;i<expectedOutput.size();i++) {
        rectangle(theImage,expectedOutput[i].getRect(),red,3,8,0);
    }

    // Then draw all the not found elements ocr output in blue
    Scalar blue(255,0,0);
    for(int i=0;i<tesseractOutput.size();i++) {
        rectangle(theImage,tesseractOutput[i].getRect(),blue,3,8,0);
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
void AccuracyProgram::cleanWords(vector<WordEntry>& root) {
    vector<WordEntry>words2;
    for(int i=0;i<root.size();i++) {
        WordEntry word=root[i];
        string w=word.getWord();
        bool isVerified=false;
        for(int i=0;i<w.size();i++) {
            if(HelperMethods::isAlphaNumericNotSpace(w[i])) {
                isVerified = true;
                break;
            }
        }
        if(isVerified) {
            words2.push_back(word);
        }
    }
    root.clear();
    root.insert(root.begin(),words2.begin(),words2.end());
}


void AccuracyProgram::compare2(Json::Value rootProgram,Json::Value rootExpected) {
    getWords(rootProgram,tesseractOutput);
    getWords(rootExpected,expectedOutput);

    if(expectedOutput.size()==0) {
        successful=false;
    }
    else {
        successful=true;
    }


    cleanWords(expectedOutput);
    cout<<tesseractOutput.size()<<" "<<expectedOutput.size()<<endl;

    accuracy=testAccuracy();
    cout<<"Accuracy is "<<accuracy<<endl;
}

void AccuracyProgram::runAccuracyTest() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bbboth/programOutput/ocrJsonFiles.txt");
    ifstream inputStream2("/home/shahrukhqasim/Desktop/bbboth/expectedOutput/files.txt");
    ifstream inputStream3("/home/shahrukhqasim/Desktop/bbboth/programInput/files.txt");
    std::string str;
    std::string str2;
    std::string str3;

    string inputFolder="/home/shahrukhqasim/Desktop/bbboth/exportedFromTesseract";
    string outputFolder="/home/shahrukhqasim/Desktop/bbboth/expectedOutput";


    const string plotOutputFolder="/home/shahrukhqasim/Desktop/bbboth/plottedDataComparison/";


    int num=0;
    float sum=0;
    // TODO: To run extensive testing on single image
    while(getline(inputStream2, str2)) {
        getline(inputStream, str);
        getline(inputStream3, str3);

        string inputImagePath="/home/shahrukhqasim/Desktop/bbboth/programInput/"+str3;
        string plotImagePath=plotOutputFolder+str3;
        string programFilePath=inputFolder+"/"+str2;
        string expectedFilePath=outputFolder+"/"+str2;

        cout<<"Running on "<<programFilePath<<endl;
//        cout<<"Running on "<<expectedFilePath<<endl<<endl;
        AccuracyProgram program(programFilePath,expectedFilePath,inputImagePath,plotImagePath);
        float accuracy=program.run();
        if(program.successful) {
            sum += accuracy;
            num++;
        }
    }
    cout<<"Average accuracy="<<sum/num<<" from "<<num<<" forms"<<endl;
}

float AccuracyProgram::run() {
    Json::Value rootProgram;
    Json::Value rootExpected;

    ifstream programStream(programOutputFile);
    programStream>>rootProgram;

    ifstream expectedStream(expectedOutputFile);
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

    return accuracy;
}

void AccuracyProgram::ferjadTest() {
    Json::Value expected444;
    ifstream s22("/home/shahrukhqasim/Desktop/ferjads/programOutput/1.json");
    s22>>expected444;



    csv::Parser file = csv::Parser(
            "/home/shahrukhqasim/Desktop/ferjads/programOutput/1.csv");

    string word;
    float left;
    float top;
    float right;
    float bottom;

    AccuracyProgram p1("","","/home/shahrukhqasim/Desktop/ferjads/programOutput/1.png","/home/shahrukhqasim/Desktop/ferjads/programOutput/comparison1.png");

    p1.getWords(expected444,p1.expectedOutput);

    for (int i = 0; i < file.rowCount(); i++) {
        word= file[i]["word"];
        left = stof(file[i]["left"]);
        top = stof(file[i]["top"]);
        right = stof(file[i]["right"]);
        bottom = stof(file[i]["bottom"]);

        WordEntry entry;
        entry.setRect(Rect(Point(left,top),Point(right,bottom)));
        entry.setString(word);

        p1.tesseractOutput.push_back(entry);
    }


    Mat image=imread(p1.inputFile,0);
    if(!image.data) {

        cerr<<"Could not read the input image file";
        exit(0);
    }
    cvtColor(image,p1.theImage,CV_GRAY2RGB);

    cleanWords(p1.expectedOutput);
    cleanWords(p1.tesseractOutput);
    cout<<p1.testAccuracy();
    HelperMethods::outputImage(p1.theImage,p1.comparisonFile);
}