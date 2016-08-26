/** 
 * @file concomp.cc
 * This file contains code for connected component analysis. 
 * 
 * @author Faisal Shafait (faisalshafait@gmail.com)  
 * 
 * @version 0.1  
 * 
 */ 
 
#include "concomp.h"
 

/** 
 * This function uses union find structures to build connected components
 * in a horizontal and a vertical sweep through the image
 *  
 * @param[in]     img Input CV_8U Mat data structure representing a binary image
 *                Foreground is assumed 0 and background is 255.
 * @param[in]     max_x Maximum allowed width of a connected component as 
 *                a fraction of image width
 * @param[in]     max_y Maximum allowed height of a connected component as 
 *                a fraction of image height
 * @param[in]     min_area Minimum allowed area (width x height) of a valid 
 *                connected component.
 * @param[in]     type Use 4 or 8 connected neighborhood.
 * @param[out]    rboxes An array of bounding boxes of valid connected components.
 * 
 */ 
void findConComp(cv::Mat &img, std::vector<cv::Rect> &rboxes,
                 float max_x, float max_y, float min_area, int type){	
    assert(img.depth() == CV_8U);
    cv::Mat labelImg = cv::Mat::zeros(img.rows, img.cols, CV_64F);
    int label = 0;
    CUnionFind *uf = new CUnionFind(img.rows*img.cols);
    int l1, l2;
    for (int y=0, Y=img.rows; y<Y; y++){	
        for (int x=0, X=img.cols ; x<X; x++){
            if (img.at<uchar>(y,x)==0){
				// adapt from left neighbor
                if (x>0 && img.at<uchar>(y,x-1)==0){
					labelImg.at<double>(y,x)=label;
                } else {
                    labelImg.at<double>(y,x)=++label;
                } // top neighbor
                if (y>0 && labelImg.at<double>(y-1,x)!=0){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x>0 && labelImg.at<double>(y-1,x-1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x-1));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x<X-1 && labelImg.at<double>(y-1,x+1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x+1));
                    if (l1 != l2) uf->set(l1,l2);
                }
            }
        }
    }
    //fprintf(stderr, "%d labels found!\n", label);
    for (int y=0; y<img.rows; y++) 
        for (int x=0; x<img.cols; x++) 
            labelImg.at<double>(y,x) = uf->find(labelImg.at<double>(y,x));
	
    // Init Bboxes and Seeds
    std::vector<cv::Rect> bb(label+1, cv::Rect());
	std::vector<bool> empty(label+1, true);
    int l;
    cv::Rect b;
    int l_old = 0 ;
    for (int y=0, Y=img.rows; y<Y; y++){
        for (int x=0, X=img.cols; x<X; x++){ 
            if (img.at<uchar>(y,x) == 0){
                l = uf->find(labelImg.at<double>(y,x));
                cv::Rect pt(x,y,1,1);
                if(empty[l]){
                    bb[l] = pt;
                    empty[l] = false;
                } else {
                    bb[l] |= pt;
                }
            }
        }
    }
    // Exclude boxes that are too big or too small or irregular
    float max_x_box = img.cols*max_x ;
    float max_y_box = img.rows*max_y ;
    cv::Rect imgDim(0,0,img.cols-1,img.rows-1);
    for (int i=1; i<=label; i++){
        if (uf->isRoot(i)){
            cv::Rect rc = bb[i];
            rc &= imgDim;
            if ( (rc.height > max_y_box) || (rc.width > max_x_box) ||
               (rc.height*rc.width < min_area) || rc.height > 1.5*rc.width || rc.width > 1.5*rc.height) continue;
            rboxes.push_back(rc);
//	        fprintf(stderr,"Root: %d %d %d %d\n", bb[i].x, bb[i].y, bb[i].width, bb[i].height);
        }
    }
    delete uf;
}