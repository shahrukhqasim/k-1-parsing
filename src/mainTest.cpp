////
//// Created by shahrukhqasim on 7/11/16.
////
//
//#include <iostream>
//
//#include "stdio.h"
//#include <vector>
//#include <tesseract/baseapi.h>
//#include "opencv2/core/core.hpp"
//
//#include <regex>
//
//using namespace std;
//using namespace tesseract;
//using namespace cv;
//
//#include "AccuracyProgram.h"
//
//
//
//int run(string imagePath, string textPath, string outputDirectoryPath, string workFile) {
//    ifstream jsonStream(textPath);
//    Json::Value jsonWords;
//    jsonStream >> jsonWords;
//    vector<TextualData> words;
//
//    Mat image = imread(imagePath, 1);
//
//
////    rectangle(image, Rect(0, 0, image.cols - 1, image.rows - 1), Scalar(255, 255, 255,255), CV_FILLED, 8, 0);
//
//    AccuracyProgram::getWords(jsonWords, words);
//
//
//
//    vector<TextualData>elemBoxes;
//
//    cout << "Running "<<workFile<<": " << words.size() << endl;
//    mergeWordBoxes(words,elemBoxes);
//
//
//
//    Mat vProjection(1,image.cols,CV_16U);
//    vProjection=0;
//    for(int i=0;i<elemBoxes.size();i++) {
//        Rect r=elemBoxes[i].getRect();
//        for(int j=r.x;j<r.x+r.width;j++)
//            vProjection.at<short>(0,j)+=r.height;
//    }
//
//    for(int i=0;i<vProjection.cols;i++) {
////        line(image,Point(i,0),Point(i,vProjection.at<short>(0,i)),colorBlue);
////        cout<<vProjection.at<short>(0,i)<<" ";
//    }
//
//    int quarter=vProjection.cols/6;
//
//    int shallowProjectionIndex=quarter;
//    for(int i=quarter*2;i<quarter*4;i++) {
//        if(vProjection.at<short>(0,i)<vProjection.at<short>(0,shallowProjectionIndex))
//            shallowProjectionIndex=i;
//    }
//
//    // Draw the separation line
////    line(image,Point(shallowProjectionIndex,0),Point(shallowProjectionIndex,image.rows-1),Scalar(0,255,0),7,8,0);
//
//    vector<TextualData>leftBoxes;
//    vector<TextualData>rightBoxes;
//
//    for(int i=0;i<elemBoxes.size();i++) {
//        TextualData box=elemBoxes[i];
//        if(shallowProjectionIndex-box.getRect().x>box.getRect().width/2) {
//            leftBoxes.push_back(box);
//        }
//        else {
//            rightBoxes.push_back(box);
//        }
//    }
//
//
//    cout<<endl;
//
//    bool p1=false,p2=false,p3=false;
//
//    // Find Information About the Partnership : Part 1
//    int indexPart1=findMinTextIndex(leftBoxes,"Information About the Partnership");
//    int indexPart2=findMinTextIndex(leftBoxes,"Information About the Partner");
//    int indexPart3=findMinTextIndex(rightBoxes,"Partner's Share of Current Year Income");
//
//    if(indexPart1!=-1 || indexPart2!=-1 && indexPart3!=-1) {
//        cout<<leftBoxes[indexPart1].getText()<<endl;
//        cout<<leftBoxes[indexPart2].getText()<<endl;
//        cout<<rightBoxes[indexPart3].getText()<<endl;
//
//        TextualData part1TextualData=leftBoxes[indexPart1];
//        TextualData part2TextualData=leftBoxes[indexPart2];
//        TextualData part3TextualData=rightBoxes[indexPart3];
//
//        vector<TextualData>part1Data;
//        vector<TextualData>part2Data;
//        vector<TextualData>part3Data;
//
//        Rect part1Rect=part1TextualData.getRect();
//        Rect part2Rect=part2TextualData.getRect();
//        Rect part3Rect=part3TextualData.getRect();
//
//        // Search in left boxes which are below indexPart1 and above indexPart2: Add them to part 1
//        // Search in left boxes which are below indexPart2: Add them to part 2
//        for(int i=0;i<leftBoxes.size();i++) {
//            TextualData current=leftBoxes[i];
//
//            // TODO: Change 10 offset to something dynamic
//            if((current.getRect().y )>=part2TextualData.getRect().y- 10) {
//                part2Data.push_back(current);
//                part2Rect=part2Rect|current.getRect();
//            }
//            else if(current.getRect().y>=part1TextualData.getRect().y && current.getRect().y<part2TextualData.getRect().y) {
//                part1Data.push_back(current);
//                part1Rect=part1Rect|current.getRect();
//            }
//        }
//
//        // Search in right boxes which are below indexPart3: Add them to part 3
//        for(int i=0;i<rightBoxes.size();i++) {
//            TextualData current=rightBoxes[i];
//            if(current.getRect().y>=part3TextualData.getRect().y) {
//                part3Data.push_back(current);
//                part3Rect=part2Rect|current.getRect();
//            }
//        }
//
//        drawBoxes(image,part1Data,Scalar(255,0,0));
//        drawBoxes(image,part2Data,Scalar(0,255,0));
//        drawBoxes(image,part3Data,Scalar(0,0,255));
//    }
//
//    imwrite(outputDirectoryPath + workFile + ".png", image);
//}
//
//int main2(int argc, char **argv) {
//    if (argc != 2) {
//        cout << "Error in arguments." << endl;
//        return 0;
//    }
//
//    string directory = argv[1];
//    if (directory[directory.length() - 1] != '/')
//        directory = directory + '/';
//
//    fstream streamImageFilesList(directory + "images/files.txt");
//    fstream streamJsonFilesList(directory + "text/files.txt");
//
//    string imageFile;
//    string jsonFile;
//
//    while (getline(streamImageFilesList, imageFile)) {
//        getline(streamJsonFilesList, jsonFile);
//
//        string workFile = HelperMethods::removeFileExtension(imageFile);
//
//        imageFile = directory + "images/" + imageFile;
//        jsonFile = directory + "text/" + jsonFile;
//
//        run(imageFile, jsonFile, directory + "/output/", workFile);
//
//    }
//}
//
//void getWords(Json::Value root, vector<TextualData> &outputVector, float height, float width) {
//
//    root = root["Pages"][0];
//
//    Json::Value words = root["Fields"];
//    for (int i = 0; i < words.size(); i++) {
//        cout<<"I am here"<<endl;
//        Json::Value word = words[i];
//        string value = word["Value"].asString();
//        Json::Value rectangle = word["Region"];
//
//        int t = rectangle["t"].asInt();
//        int l = rectangle["l"].asInt();
//        int b = rectangle["b"].asInt();
//        int r = rectangle["r"].asInt();
//
//        TextualData entry;
//        entry.setRect(Rect(l, t, r - l, b - t));
//        entry.setText(value);
//        outputVector.push_back(entry);
//    }
//}
//
//
//int main(int argc, char **argv) {
//    ifstream jsonFile("/home/shahrukhqasim/Desktop/2.json");
//    Json::Value value;
//    jsonFile>>value;
//    vector<TextualData>words;
//    Mat img=imread("/home/shahrukhqasim/Desktop/2.png",1);
//    getWords(value,words,img.rows,img.cols);
//    cout<<words.size()<<endl;
//    drawBoxes(img,words,Scalar(0,0,255));
//    HelperMethods::displayImageResizable(img);
//}