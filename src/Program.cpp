//
// Created by shahrukhqasim on 6/24/16.
//

#include "Program.h"

Program::Program(string inputFileName, string inputFolder, string outputFolder) {
    this->inputFileName=inputFileName;
    this->inputFolder=inputFolder;
    this->outputFolder=outputFolder;
}

void Program::loadImage(Mat &image, string fileName, int mode) {
    Mat image2 = imread(fileName, mode);   // Read the file
    if (!image2.data)                      // Check for invalid input
    {
        cout << "Could not open or find the image: " << fileName << std::endl;
        exit(0);
    }
    Preprocessor::binarizeShafait(image2,image,100,0.5);

}


void Program::run() {
    loadImage(originalImage,inputFolder+"/"+inputFileName,0);
    doSegmentation();
    cleanImageAndWriteToDisk();
    runOcr();
    outputResult();
}

void Program::doSegmentation() {
    Mat image=originalImage.clone();

    Preprocessor::invertImage(image);
    Preprocessor::conCompFast(image,segments,0.5,0.5,1024,4);
    Preprocessor::invertImage(image);
}

void Program::runOcr() {
    // First run OCR on the segments
    TesseractFinder finder1(outputFolder+"/"+cleanedImageFileName,segments);
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
    finder2.run();

    // Now merge the results of both runs
    data=finder1.getRecognizedData();
    vector<OcrResult>data2=finder2.getRecognizedData();
    data.insert(data.end(), data2.begin(), data2.end());

}

void Program::cleanImageAndWriteToDisk() {
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

void Program::outputResult() {
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