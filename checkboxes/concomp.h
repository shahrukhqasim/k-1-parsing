#pragma once
/** 
 * @file concomp.h
 * This file contains code for connected component analysis. 
 * 
 * @author Faisal Shafait (faisalshafait@gmail.com)  
 * 
 * @version 0.1  
 * 
 */ 
 
#include "stdio.h" 
#include "union-find.h"
// Open CV headers 
#include "opencv2/core/core.hpp" 
 
void findConComp(cv::Mat &img, std::vector<cv::Rect> &rboxes,
                 float max_x, float max_y, float min_area, int type);
                 
  
