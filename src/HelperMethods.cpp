/*
 * HelperMethods.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: Shah Rukh Qasim
 */

#include "HelperMethods.h"
#include "union-find.h"

#include "iostream"

void HelperMethods::displayImageResizable(const Mat&image) {
	imwrite("temp/displayImage.png",image);
	system("xdg-open temp/displayImage.png");
}


void HelperMethods::outputImage(const Mat&image, const string& fileName) {
	imwrite(fileName,image);
}

string HelperMethods::removeFileExtension(const string &fileName) {
	int dotIndex=-1;
	for(int i=fileName.length()-1;i>=0;i--) {
		if(fileName[i]=='.') {
			dotIndex = i;
			break;
		}
	}

	if(dotIndex>0)
		return fileName.substr(0,dotIndex);

	std::cerr<<"Error in file name:"<<fileName<<endl;
	exit(0);
}