/*
 * HelperMethods.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: shahrukhqasim
 */

#include "HelperMethods.h"


void HelperMethods::displayImageResizable(const Mat&image) {
	imwrite("temp/displayImage.png",image);
	system("xdg-open temp/displayImage.png");
}


void HelperMethods::outputImage(const Mat&image, const string& fileName) {
	imwrite(fileName,image);
}
