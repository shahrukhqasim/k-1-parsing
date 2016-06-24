/*
 * HelperMethods.h
 *
 *  Created on: Jun 22, 2016
 *      Author: Shah Rukh Qasim
 */


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>

#ifndef HELPERMETHODS_H_
#define HELPERMETHODS_H_


using namespace cv;
using namespace std;

class HelperMethods {
public:

    /**
     * Displays image in @image in a resizable window with the help of programs installed on the host machine
     * */
    static void displayImageResizable(const Mat &image);


    /**
     * Outputs @param image to file specified by param @fileName
     * */
    static void outputImage(const Mat &image, const string &fileName);

    static string removeFileExtension(const string &fileName);

};

#endif /* HELPERMETHODS_H_ */
