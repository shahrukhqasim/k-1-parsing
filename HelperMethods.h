/*
 * HelperMethods.h
 *
 *  Created on: Jun 22, 2016
 *      Author: shahrukhqasim
 */

#ifndef HELPERMETHODS_H_
#define HELPERMETHODS_H_


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string>

using namespace cv;
using namespace std;

class HelperMethods {
public:

	/**
	 * Displays image in @image in a resizable window with the help of programs installed on the host machine
	 * */
	static void displayImageResizable(const Mat&image);


	/**
	 * Outputs @image to file specified by @fileName
	 * */
	static void outputImage(const Mat&image, const string& fileName);

};

#endif /* HELPERMETHODS_H_ */
