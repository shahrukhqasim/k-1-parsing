#include "Preprocessor.h"
using namespace cv;

void Preprocessor::conCompFast(const cv::Mat &img, std::vector<cv::Rect> &rboxes, float max_x, float max_y, float min_area, int type) {
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
                    l2 = uf->find((int) labelImg.at<double>(y - 1, x));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x>0 && labelImg.at<double>(y-1,x-1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find((int) labelImg.at<double>(y - 1, x - 1));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x<X-1 && labelImg.at<double>(y-1,x+1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find((int) labelImg.at<double>(y - 1, x + 1));
                    if (l1 != l2) uf->set(l1,l2);
                }
            }
        }
    }
    //fprintf(stderr, "%d labels found!\n", label);
    for (int y=0; y<img.rows; y++)
        for (int x=0; x<img.cols; x++)
            labelImg.at<double>(y,x) = uf->find((int) labelImg.at<double>(y, x));

    // Init Bboxes and Seeds
    std::vector<cv::Rect> bb((unsigned long) (label + 1), cv::Rect());
    std::vector<bool> empty((unsigned long) (label + 1), true);
    int l;
    //cv::Rect b;
    //int l_old = 0 ;
    for (int y=0, Y=img.rows; y<Y; y++){
        for (int x=0, X=img.cols; x<X; x++){
            if (img.at<uchar>(y,x) == 0){
                l = uf->find((int) labelImg.at<double>(y, x));
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
    // Exclude boxes that are too big or too small
    float max_x_box = img.cols*max_x ;
    float max_y_box = img.rows*max_y ;
    cv::Rect imgDim(0,0,img.cols-1,img.rows-1);
    for (int i=1; i<=label; i++){
        if (uf->isRoot(i)){
            cv::Rect rc = bb[i];
            rc &= imgDim;
            if ( (rc.height > max_y_box) || (rc.width > max_x_box) ||
                 (rc.height*rc.width < min_area)) continue;
            rboxes.push_back(rc);
//	        fprintf(stderr,"Root: %d %d %d %d\n", bb[i].x, bb[i].y, bb[i].width, bb[i].height);
        }
    }
    delete uf;
}


void Preprocessor::binarizeShafait(Mat &gray, Mat &binary, int w, double k){
    Mat sum, sumsq;
    gray.copyTo(binary);
    int half_width = w >> 1;
    integral(gray, sum, sumsq,  CV_64F);
    for(int i=0; i<gray.rows; i ++){
        for(int j=0; j<gray.cols; j++){
            int x_0 = (i > half_width) ? i - half_width : 0;
            int y_0 = (j > half_width) ? j - half_width : 0;
            int x_1 = (i + half_width >= gray.rows) ? gray.rows - 1 : i + half_width;
            int y_1 = (j + half_width >= gray.cols) ? gray.cols - 1 : j + half_width;
            double area = (x_1-x_0) * (y_1-y_0);
            double mean = (sum.at<double>(x_0,y_0) + sum.at<double>(x_1,y_1) - sum.at<double>(x_0,y_1) - sum.at<double>(x_1,y_0)) / area;
            double sq_mean = (sumsq.at<double>(x_0,y_0) + sumsq.at<double>(x_1,y_1) - sumsq.at<double>(x_0,y_1) - sumsq.at<double>(x_1,y_0)) / area;
            double stdev = sqrt(sq_mean - (mean * mean));
            double threshold = mean * (1 + k * ((stdev / 128) -1) );
            if (gray.at<uchar>(i,j) > threshold)
                binary.at<uchar>(i,j) = 255;
            else
                binary.at<uchar>(i,j) = 0;
        }
    }
}

void Preprocessor::invertImage(Mat &image) {
    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            image.at<uchar>(i,j)= (uchar) (255 - image.at<uchar>(i, j));

        }
    }
}
