#include <iostream>

#include "stdio.h"
#include <vector>
#include <string>
#include <fstream>
// Tesseract headers
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// Open CV headers
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <leptonica/allheaders.h>

#include "TesseractFinder.h"
#include "EditDistance.h"

#include "../csv/CSVparser.hpp"
#include "HelperMethods.h"

using namespace std;
using namespace tesseract;
using namespace cv;

RNG rng(12345);

struct ValueData {
	Point p1;
	Point p2;
	string exactKeyText;
};

void recognizeText(const string& path, vector<ValueData>&valueData);
void match(vector<KeyData> keyData, vector<ValueData> valueData, Mat&image);

void loadImage(Mat&image, string fileName, int mode) {
	image = imread(fileName, mode);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image: " << fileName << std::endl;
		exit(0);
	}

}

void displayImage(Mat&image) {
	namedWindow("Display window", WINDOW_NORMAL); // Create a window for display.
	imshow("Display window", image);                 // Show our image inside it

	waitKey(0);
}

void drawBoxes(TesseractFinder&finder, Mat&image) {
	vector<KeyData> data = finder.getRecognizedData();
	for (int i = 0; i < data.size(); i++) {

		Point p1 = data[i].p1;
		Point p2 = data[i].p2;
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		rectangle(image, p1, p2, color, 3, 8, 0);
	}
}

// Page,Left,Top,Width,Height,FieldName,MappedFieldName,FieldDataType
void process(Mat&image, vector<ValueData>&valueData) {
	// TODO: Change path or something
	csv::Parser file = csv::Parser("model.csv");

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
		valueData.push_back( { tll, brr, mappedFieldName });
	}
}

// static void conCompFast(cv::Mat &img, std::vector<cv::Rect> &rboxes, float max_x=1.0, float max_y=1.0, float min_area=0, int type=8);
void computeSegmentation(Mat&image,vector<Rect>&boxes) {
    HelperMethods::invertImage(image);
	HelperMethods::conCompFast(image,boxes,0.5,0.5,1024,4);
    HelperMethods::invertImage(image);

    Mat image2;
    cvtColor(image,image2,CV_GRAY2RGB);
    image=image2;

//    for(int i=0;i<boxes.size();i++) {
//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),rng.uniform(0, 255));
//		Rect box=boxes[i];
//		rectangle(image, box, color, 3, 8, 0);
//	}
//    HelperMethods::displayImageResizable(image);

}


void cleanImage(Mat&image) {
    Mat image3;
    cvtColor(image,image3,CV_RGB2GRAY);
    image=image3;

    HelperMethods::binarizeShafait(image,image3,100,0.5);

    image=image3;

    vector<Rect>boxes;
    HelperMethods::conCompFast(image,boxes,1,1,0,8);

    Mat image2(image.rows,image.cols, image.type());

    Scalar color(255,255,255);
    rectangle(image2,Rect(0,0,image2.cols,image2.rows),color,CV_FILLED,8,0);

    for(int i=0;i<boxes.size();i++) {
        Rect box=boxes[i];
        if(!(box.width>10*box.height || box.width*10>image.cols||box.height*10>image.rows)) {
            Mat maskImage(image.rows,image.cols,image.type());
            maskImage.setTo(0);
            rectangle(maskImage,box,255,CV_FILLED,8,0);
            //image2(boxes[i])=image(boxes[i]);
            //image.copyTo(image2(Rect(0, 0, frame1.cols, frame1.rows)));
            image.copyTo(image2,maskImage);
        }
    }

    image=image2.clone();
}




int main() {
    cout<<"Running batch job"<<endl;
    for(int i=0;i<=10;i++) {
        string fileName = "../../input/";
        fileName+=to_string(i);
        fileName+=".png";
        string fileNameO = "../../output/";
        fileNameO+=to_string(i);
        fileNameO+=".png";
        string fileNameO2="../../output/";
        fileNameO2+=to_string(i);
        fileNameO2+="_boxesRemoved.png";
        string fileNameO3="../../output/";
        fileNameO3+=to_string(i);
        fileNameO3+="_rulingsRemoved.png";


        cout<<"Running on sample "<<fileName<<endl;
        cout<<"B: "<<fileNameO<<endl;
        cout<<"C: "<<fileNameO2<<endl;

        Mat image;
        vector<ValueData> values;
        vector<Rect> boxes;
        loadImage(image, fileName,0);
        //process(image, values);
        //
        computeSegmentation(image, boxes);
        cleanImage(image);
        HelperMethods::outputImage(image,fileNameO3);
        loadImage(image, fileName,1);
        // Detect in boxes
        Mat image2 = image.clone();
        TesseractFinder finder(fileNameO3, boxes);
        finder.run();

        Scalar color(255, 255, 255);
        for (int i = 0; i < boxes.size(); i++) {
            Rect box = boxes[i];
            rectangle(image2, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), color, CV_FILLED, 8,
                      0);
        }


        HelperMethods::outputImage(image2, fileNameO2);
        TesseractFinder finder2(fileNameO2);
        finder2.run();

        drawBoxes(finder, image);
        drawBoxes(finder2, image);
        //match(finder.getRecognizedData(), values, image);
        HelperMethods::outputImage(image, fileNameO);
        cout<<"Run complete on "<<i<<endl;

        image.release();
        image2.release();
    }
	return 0;
}

void match(vector<KeyData> keyData, vector<ValueData> valueData, Mat&image) {
	EditDistance editDistanceCalculator;

	for (int i = 0; i < valueData.size(); i++) {
		cout << "Searching for " << valueData[i].exactKeyText << endl;
		int minIndex = -1;
		int minDistance = 9999999;
		for (int j = 0; j < keyData.size(); j++) {
			//cout << "Comparing " << valueData[i].exactKeyText << " with "
			//		<< keyData[j].text << endl;
			int newDistance = editDistanceCalculator.lDistance(
					valueData[i].exactKeyText.c_str(), keyData[j].text.c_str());
			//cout << "Result is " << newDistance << endl;
			if (minIndex == -1 || newDistance < minDistance) {
				//cout << "Was: " << minDistance;
				//cout << " Is: " << newDistance << endl;
				minIndex = j;
				minDistance = newDistance;
			}
		}
		if (minIndex != -1) {
			cout << "Found " << valueData[i].exactKeyText << "for "
					<< keyData[minIndex].text << endl;
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
					rng.uniform(0, 255));
			rectangle(image, keyData[minIndex].p1, keyData[minIndex].p2, color,
					2, 8, 0);
			rectangle(image, valueData[i].p1, valueData[i].p2, color, 2, 8, 0);
			keyData.erase(keyData.begin() + minIndex);
		} else {
			cout << "Count not find for " << valueData[i].exactKeyText << endl;
		}
	}
}
