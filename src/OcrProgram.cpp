//
// Created by shahrukhqasim on 6/24/16.
//

#include "OcrProgram.h"
#include "AccuracyProgram.h"

OcrProgram::OcrProgram(string inputFileName, string inputFolder, string outputFolder) {
    this->inputFileName=inputFileName;
    this->inputFolder=inputFolder;
    this->outputFolder=outputFolder;
}

void OcrProgram::loadImage(Mat &image, string fileName, int mode) {
    Mat image2 = imread(fileName, mode);   // Read the file
    if (!image2.data)                      // Check for invalid input
    {
        cout << "Could not open or find the image: " << fileName << std::endl;
        exit(0);
    }
    Preprocessor::binarizeShafait(image2,image,100,0.5);

}


void OcrProgram::run() {
    loadImage(originalImage,inputFolder+"/"+inputFileName,0);
    doSegmentation();
    cleanImageAndWriteToDisk();
    runOcr();
    outputToJson();
    outputResult();
}

void OcrProgram::doSegmentation() {
    Mat image=originalImage.clone();

    Preprocessor::invertImage(image);
    Preprocessor::conCompFast(image,segments,0.5,0.5,1024,4);
    Preprocessor::invertImage(image);
}

void OcrProgram::runOcr() {
    // First run OCR on the segments
    TesseractFinder finder1(outputFolder+"/"+cleanedImageFileName,segments);
    finder1.setIteratorLevel(tesseract::RIL_WORD);
    finder1.run();

    Mat imageWithoutSegments=cleanedImage.clone();

    // Then remove the segments and run OCR again
    Scalar color(255, 255, 255);
    for (int i = 0; i < segments.size(); i++) {
        Rect box = segments[i];
        rectangle(imageWithoutSegments, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), color, CV_FILLED, 8,0);
    }

    string withoutSegmentsImageFileName=HelperMethods::removeFileExtension(inputFileName)+"_withoutSegments.png";
    HelperMethods::outputImage(imageWithoutSegments,outputFolder+"/"+withoutSegmentsImageFileName);

    TesseractFinder finder2(outputFolder+"/"+withoutSegmentsImageFileName);
    finder1.setIteratorLevel(tesseract::RIL_WORD);
    finder2.run();

    // Now merge the results of both runs
    data=finder1.getRecognizedData();
    vector<OcrResult>data2=finder2.getRecognizedData();
    data.insert(data.end(), data2.begin(), data2.end());

}

void OcrProgram::cleanImageAndWriteToDisk() {
    vector<Rect>boxes;
    Preprocessor::conCompFast(originalImage,boxes,1,1,0,8);

    cleanedImage=Mat(originalImage.rows,originalImage.cols, originalImage.type());

    Scalar color(255,255,255);
    rectangle(cleanedImage,Rect(0,0,cleanedImage.cols,cleanedImage.rows),color,CV_FILLED,8,0);

    for(int i=0;i<boxes.size();i++) {
        Rect box=boxes[i];
        if(!(box.width>10*box.height || box.width*10>originalImage.cols||box.height*10>originalImage.rows)) {
            Mat maskImage(originalImage.rows,originalImage.cols,originalImage.type());
            maskImage.setTo(0);
            rectangle(maskImage,box,255,CV_FILLED,8,0);
            originalImage.copyTo(cleanedImage,maskImage);
        }
    }

    cleanedImageFileName=HelperMethods::removeFileExtension(inputFileName)+"_cleaned.png";

    HelperMethods::outputImage(cleanedImage,outputFolder+"/"+cleanedImageFileName);
}

void OcrProgram::outputToJson() {
    Json::Value root;
    Json::Value pages;
    pages["PageNumber"]=1;
    pages["Width"]=originalImage.cols;
    pages["Height"]=originalImage.rows;

    Json::Value words;

    for(int i=0;i<data.size();i++) {
        OcrResult wordEntry=data[i];
        string s=wordEntry.text;
        Json::Value word;
        Json::Value rectangle;
        rectangle["l"]=wordEntry.p1.x;
        rectangle["t"]=wordEntry.p1.y;
        rectangle["r"]=wordEntry.p2.x;
        rectangle["b"]=wordEntry.p2.y;
        word["Value"]=s;
        word["Region"]=rectangle;

        pages["Words"][i]=word;
    }

    root["Pages"][0]=pages;

    fstream outputFile(outputFolder+"/"+HelperMethods::removeFileExtension(inputFileName)+".json",ios::out);
    outputFile<<root;
    outputFile.close();
}

void OcrProgram::outputResult() {


    Mat image;
    cvtColor(originalImage,image,CV_GRAY2RGB);

    for (int i = 0; i < data.size(); i++) {

        Point p1 = data[i].p1;
        Point p2 = data[i].p2;
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                              rng.uniform(0, 255));

        rectangle(image, p1, p2, color, 3, 8, 0);
    }

    HelperMethods::outputImage(image,outputFolder+"/"+inputFileName);
}

void OcrProgram::oneToOneMatch() {
	// TODO: Change path or something
	csv::Parser file = csv::Parser(inputFolder+"/model.csv");

	string mappedFieldName;
	float left;
	float top;
	float width;
	float height;

	for (int i = 0; i < file.rowCount(); i++) {
		mappedFieldName = file[i]["MappedFieldName"];
		left = stof(file[i]["Left"]);
		top = stof(file[i]["Top"]);
		width = stof(file[i]["Width"]);
		height = stof(file[i]["Height"]);

		cout << "Read a line: " << mappedFieldName << endl;

		Point tll(left * 2.777, (792 - top) * 2.777);
		Point brr(tll.x + width * 2.777, tll.y + height * 2.777);

        documentModelData.push_back( { tll, brr, mappedFieldName });
	}
}

void OcrProgram::runOcrProgram() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/programInput/files.txt");
    ifstream expectedJsonStream("/home/shahrukhqasim/Desktop/bb/All/expectedOutput/files.txt");

    std::string str;
    std::string str2;

    // TODO: To run extensive testing on single image
    while(getline(inputStream, str)) {
        getline(expectedJsonStream, str2);
        cout<<"Running on "<<str<<endl;

        OcrProgram(str,"/home/shahrukhqasim/Desktop/bb/All/programInput","/home/shahrukhqasim/Desktop/bb/All/programOutput").run();
        string programFilePath="/home/shahrukhqasim/Desktop/bb/All/programOutput/"+HelperMethods::removeFileExtension(str)+".json";
        string expectedFilePath="/home/shahrukhqasim/Desktop/bb/All/expectedOutput/"+str2;
        string inputImagePath="/home/shahrukhqasim/Desktop/bb/All/programInput/"+str;
        string plotImagePath="/home/shahrukhqasim/Desktop/bb/All/plottedDataComparison/"+str;
        AccuracyProgram(programFilePath,expectedFilePath,inputImagePath,plotImagePath).run();

        cout<<"Run complete on "<<str<<endl;
    }
}