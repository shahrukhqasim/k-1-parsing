#pragma once
/** 
 * @file checkbox.h
 * This file contains class definitions for detecting checkboxes
 * in a PDF rendered image. 
 * 
 * @author Faisal Shafait (faisalshafait@gmail.com)  
 * 
 * @version 0.1  
 * 
 */ 
 
#include "stdio.h" 
#include <string> 
#include <fstream>
#include <sstream>
// Open CV headers 
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
 
class CCheckBox{
public:
    cv::Rect outerBBox;
    cv::Rect innerBBox;
    double innerFillRatio;
    bool isFilled;
    bool validOuterBBox;
    bool validInnerBBox;
    
    CCheckBox(){
        outerBBox = cv::Rect(0, 0, 0, 0);
        innerBBox = cv::Rect(0, 0, 0, 0);
        isFilled = false;
        validOuterBBox = false;
        validInnerBBox = false;
        innerFillRatio = 0.0;
    }
    
};

class CDetectCheckBoxes{
public:
    bool detectCheckBoxes(cv::Mat &imgMATgray, std::vector<CCheckBox> &cBoxes);  
    bool detectCheckBoxes(std::string &inFileName, std::vector<CCheckBox> &cBoxes);
private:
    void fillRatioFilter(cv::Mat &imgBin, std::vector<CCheckBox> &cBoxes);
    double computeFillRatio(cv::Mat &imgBin, cv::Rect &bbox);
    void mergeHoles(std::vector<cv::Rect> &holes);
    bool isAlmostSquare(cv::Rect &r);
    void binarizeOtsu(cv::Mat &gray, cv::Mat &binary);
};
