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

#include "csv/CSVparser.hpp"

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

void loadImage(Mat&image) {
	image = imread("form.png", CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		exit(0);
	}

}

void displayImage(Mat&image) {
	namedWindow("Display window", WINDOW_NORMAL); // Create a window for display.
	imshow("Display window", image);                 // Show our image inside it

	waitKey(0);
}

//void drawBoxes(TesseractFinder&finder, Mat&image) {
//  for(int i=0;i<finder.plotData1.size();i++) {
//    Point p1=finder.plotData1[i];
//    Point p2=finder.plotData2[i];
//    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

//    rectangle( image, p1, p2, color, 2, 8, 0 );
//  }
//}

// Page,Left,Top,Width,Height,FieldName,MappedFieldName,FieldDataType
void process(Mat&image, vector<ValueData>&valueData) {
	csv::Parser file = csv::Parser("model.csv");

	string mappedFieldName;
	float left;
	float top;
	float width;
	float height;

	for(int i=0;i<file.rowCount();i++) {
		mappedFieldName=file[i]["MappedFieldName"];
		left=stof(file[i]["Left"]);
		top=stof(file[i]["Top"]);
		width=stof(file[i]["Width"]);
		height=stof(file[i]["Height"]);

		cout << "Read a line: " << mappedFieldName << endl;

		Point tll(left * 2.777, (792 - top) * 2.777);
		Point brr(tll.x + width * 2.777, tll.y + height * 2.777);
		valueData.push_back( { tll, brr, mappedFieldName });
	}
}

int main() {
	Mat image;
	vector<ValueData> values;

	loadImage(image);
	process(image, values);
	TesseractFinder finder("form.png");
	finder.run();
	//drawBoxes(finder,image);
	match(finder.getRecognizedData(), values, image);
	displayImage(image);

	return 0;
}

void match(vector<KeyData> keyData, vector<ValueData> valueData, Mat&image) {
	EditDistance editDistanceCalculator;

	for (int i = 0; i < valueData.size(); i++) {
		int minIndex = -1;
		int minDistance = 9999999;
		for (int j = 0; j < keyData.size(); j++) {
			cout << "Comparing " << valueData[i].exactKeyText << " with "
					<< keyData[j].text << endl;
			int newDistance = editDistanceCalculator.lDistance(
					valueData[i].exactKeyText.c_str(), keyData[j].text.c_str());
			cout << "Result is " << newDistance << endl;
			if (minIndex == -1 || newDistance < minDistance) {
				cout << "Was: " << minDistance;
				cout << " Is: " << newDistance << endl;
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
			valueData.erase(valueData.begin() + minIndex);
		}
	}
}
