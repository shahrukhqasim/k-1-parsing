//  main.cpp
//  Project_ocr
//
//  Created by Talha Rehman on 3/2/16.




#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "stdio.h"
#include <string>
#include <fstream>
#include "union-find.h"

// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



using namespace cv;
using namespace std;

void heuristicFilter(vector<Rect>& rboxes, vector<int>& areaofCC);
void cropImage(Mat&, Mat&, vector<Rect>& rboxes);
void projectionProfile(Mat&, vector<int>&);
void cleanBoxes(vector<Rect>& , vector<int>& );
void distanceBwBoxes(vector<Rect>&, vector<int>&);
void sortBoxes(vector<Rect>&);
void mergeBoxes(vector<Rect>&, vector<int>&);


int test( int argc, const char** argv ){

    string path="/Users/talha/Desktop/result.png";

    Mat binarizedImg;

    //load input image

    Mat img = imread("/Users/talha/Desktop/test.png", CV_LOAD_IMAGE_UNCHANGED); //read the image data in the file "MyPic.JPG" and store it in 'img'



    //rescaling the image
    float m_ReceiptWidth=1200;
    int imWidth = img.cols;
    Mat rescaledImg;
    double scale = m_ReceiptWidth / imWidth;
    resize(img, rescaledImg, Size(), scale, scale, INTER_LANCZOS4);

    img=rescaledImg;


    //-------Binarization--------------------------

    Mat imgMATgray, imgMATbin;

    cvtColor(img, imgMATgray, COLOR_RGB2GRAY);

    binarizeShafait(imgMATgray, imgMATbin, 30, 0.05);

    //binarized image is in imgMatBin

    //--------connected componenets----------------

    vector<Rect> rboxes;
    vector<int> areaofCC;
    vector<int> dist;
    //area of cc
    areaofCC=conCompFast(imgMATbin, rboxes,  8);
    areaofCC.erase (areaofCC.begin());


    cleanBoxes(rboxes, areaofCC);
    sortBoxes( rboxes);
    distanceBwBoxes(rboxes, dist);

    for(int i=0; i<dist.size(); i++)
        cout<<dist[i]<<" ";


    mergeBoxes(rboxes, dist);




    //display bounding boxes
    for(int i=0; i<rboxes.size(); i++){

        rectangle(img, rboxes[i], CV_RGB(255,0,0));

    }


    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", img );




    //wait infinite time for a keypress
    waitKey(0);

    return 0;

}



double calculateMean(vector<int>value) {
    double sum = 0;
    for(int i = 0; i < value.size(); i++)
        sum += value[i];
    return (sum / value.size());
}






//function definations




void projectionProfile(Mat& input, vector<int>& P){
    for(int i=0; i<input.rows; i++){
        for(int j=0; j<input.cols; j++){

            if(input.at<int>(i,j)==0){
                P[i]++;

            }
        }
    }
}




//crop bounding boxes from parent image and copy them into new image
void cropImage(Mat& input, Mat& output, vector<Rect>& rboxes){
    for(int i=0; i<rboxes.size(); i++){
        for(int k=0;k<rboxes[i].height;k++) {
            for(int l=0;l<rboxes[i].width;l++) {
                output.at<uchar>(rboxes[i].y+l,rboxes[i].x+k)=input.at<uchar>(rboxes[i].y+l,rboxes[i].x+k);
            }
        }
    }
}



void distanceBwBoxes(vector<Rect>& rboxes, vector<int>& dist){

    for(int i=0; i<rboxes.size()-1; i++) {

        dist.push_back( rboxes[i+1].x - rboxes[i ].x -rboxes[i].width);

    }

}




void cleanBoxes(vector<Rect>& rboxes, vector<int>& areaofCC){

    int Tarea=100;

    vector<Rect> rboxes_1;
    vector<int> areaofCC_1;


    for(int i=0; i<areaofCC.size(); i++){

        if(areaofCC[i]> Tarea){

            areaofCC_1.push_back(areaofCC[i]);
            rboxes_1.push_back(rboxes[i]);

        }

    }

    rboxes=rboxes_1;
    areaofCC=areaofCC_1;



}



void sortBoxes(vector<Rect>& output){


    //implements algo

    for (int i = output.size() - 1; i > 0; i--){
        for (int j = 0; j <= i - 1; j++){
            if (output[j].x > output[j + 1].x)
                swap(output[j], output[j + 1]);
        }
    }

}






void mergeBoxes(vector<Rect>& rboxes, vector<int>& dist ){


    float meanDistance=calculateMean(dist);
    cout<<meanDistance;
    vector<Rect> words=rboxes;
    Rect temp;

    for(int i=0; i<dist.size(); ){


        if(dist[i]< 1.1*meanDistance ){

            temp=Rect(words[i].x,words[i].y,(words[i].width+words[i+1].width)+meanDistance, words[i].height),
                    words.erase(words.begin()+i);
            dist.erase(dist.begin()+i);
            words[i]=temp;
        }
        else
            i++;
    }

    rboxes=words;

}
