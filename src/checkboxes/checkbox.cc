/** 
 * @file pdf-ocr.cc 
 * This is the main file for extracting checkboxes from images.
 * 
 * 
 * @author Faisal Shafait (faisalshafait@gmail.com)  
 * 
 * @version 0.1  
 * 
 */

#include "stdio.h"
#include <string>
#include "checkbox.h"
#include "../Preprocessor.h"

using namespace std;
using namespace cv;


/**
 * This function implements global image thresholding using Otsu's algorithm
 *
 * @param[in]     gray Gray scale input image as an OpenCV Mat object.
 * @param[out]    binary Output image binarized using Otsu's method.
 */
void CDetectCheckBoxes::binarizeOtsu(Mat &gray, Mat &binary){
    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
}

/**
 * This function decides whether the input Rect has a square shape
 * with a reasonable margin for distortions
 *
 * @param[in]     r Input rectangle
 * @return    True if the input rectangle appears like a square, False otherwise.
 */
bool CDetectCheckBoxes::isAlmostSquare(Rect &r){
    float factor = 0.3;
    float hmin = (float) (r.height * (1.0 - factor));
    float hmax = (float) (r.height * (1.0 + factor));

    return (r.width > hmin) && (r.width < hmax) && (r.area() < 5000) && (r.area()>800);
}


void CDetectCheckBoxes::mergeHoles(vector<Rect> &holes){
    int nHoles = (int) holes.size();
    vector<Rect> merged;
    vector<bool> used((unsigned long) nHoles, false);
    for(int i=0; i<nHoles; i++){
        if(used[i]) continue;
        Rect r = holes[i];
        Rect re = r;
        re.x -= 10;
        re.width += 20;
        Rect mr;
        bool intersection = false;
        for(int j=i+1; j<nHoles; j++){
            if(used[j]) continue;
            Rect n = holes[j];
            Rect inter = n & re;
            if(inter.area()){
                intersection = true;
                re |= n;
                r |= n;
                used[j] = true;
                holes[i] = r;
            }
        }
        if(!used[i]){
            merged.push_back(holes[i]);
        }
    }
    swap(merged, holes);
}

double CDetectCheckBoxes::computeFillRatio(Mat &imgBin, Rect &bbox){
    double fr = 0.0;
    int margin = 2;
    if(!bbox.area()) return 0.0;
    //fprintf(stderr, "BBox: %d %d %d %d\n", bbox.x, bbox.y, bbox.width, bbox.height);
    int nRows = imgBin.rows;
    int nCols = imgBin.cols;
    int r0 = max(bbox.y, 0);
    int r1 = min(nRows-1, bbox.y+bbox.height-1);
    int c0 = max(bbox.x, 0);
    int c1 = min(nCols-1, bbox.x+bbox.width-1);
    //fprintf(stderr, "Final Box: %d %d %d %d\n", c0, r0, c1-c0, r1-r0);
    double counter = 0;
    double black = 0;
    //Mat img = imgBin.clone();
    for(int r=r0+margin; r<r1-margin; r++){
        for(int c=c0+margin; c<c1-margin; c++){
            counter ++;
            if(imgBin.at<uchar>(r,c) == 0){
                black ++;
            }
            //img.at<uchar>(r,c) = 128;
        }
    }
    //imwrite("binary.png", img);
    if(!counter) return 0.0;
    fr = black / counter;
//    fprintf(stderr, " %.2f / %.2f = %.2f\n", black, counter, fr);
    return fr;
}

void CDetectCheckBoxes::fillRatioFilter(Mat &imgBin, vector<CCheckBox> &cBoxes){
    int nBoxes = cBoxes.size();
    for(int i=0; i<nBoxes; i++){
        CCheckBox cb = cBoxes[i];
        double fr = 0.0;
        if(cb.validInnerBBox){
            fr = computeFillRatio(imgBin, cb.innerBBox);
        }
        else if(cb.validOuterBBox){
            fr = computeFillRatio(imgBin, cb.outerBBox);
        }
        //fprintf(stderr, "%.2f   ", fr);
        cBoxes[i].innerFillRatio = fr;
    }
    //fprintf(stderr, "\n");
}

bool CDetectCheckBoxes::detectCheckBoxes(Mat &imgMATgray, vector<CCheckBox> &cBoxes){

    Mat imgMATbin, imgMATbinInv, imgMATbinMorph, imgMATbinH, imgMATbinV;
    binarizeOtsu(imgMATgray, imgMATbin);
    imgMATbinInv = 255 - imgMATbin;
    Mat elementH(2, 20, CV_8U, cv::Scalar(1));
    Mat elementV(20, 2, CV_8U, cv::Scalar(1));
    erode(imgMATbinInv, imgMATbinH, elementH);
    erode(imgMATbinInv, imgMATbinV, elementV);
    dilate(imgMATbinH, imgMATbinH, elementH);
    dilate(imgMATbinV, imgMATbinV, elementV);
    imgMATbinMorph = imgMATbinH | imgMATbinV;

//	ifstream infile("coords.csv");
//	if (!infile.is_open()){
//		fprintf(stderr, "%s", "file not open!\n");
//		exit(-1);
//	}

    int ex_size = 0;
    vector<Rect> holes;
//	for (size_t i = 0; i < x.size(); i++){
    Mat new_mat = imgMATbinMorph;//(Rect(x[i], y[i], width[i], height[i]));
    /*imshow("out", new_mat);
    waitKey(0);
    destroyWindow("out");*/
    Preprocessor::conCompFast(new_mat, holes, 2, 2, 200, 4);
//		for (int k = ex_size; k < holes.size(); k++){
//			holes[k].x += x[i];
//			holes[k].y += y[i];
//		}
    ex_size = holes.size();
//	}
    //mergeHoles(holes);
//	fprintf(stderr, "%s\n", "done!");

    imgMATbinMorph = 255 - imgMATbinMorph;
    vector<Rect> rboxes;
//	fprintf(stderr, "%s\n", "done!");

    ex_size = 0;
//	for (size_t i = 0; i < x.size(); i++){
    new_mat = imgMATbinMorph;//(Rect(x[i], y[i], width[i], height[i]));
    /*imshow("out", new_mat);
    waitKey(0);
    destroyWindow("out");*/
    Preprocessor::conCompFast(new_mat, rboxes, 2, 2, 200, 4);
//		for (int k = ex_size; k < rboxes.size(); k++){
//			rboxes[k].x += x[i];
//			rboxes[k].y += y[i];
//		}
    ex_size = rboxes.size();

//	}


    vector<bool> usedInner(holes.size(), false);


    for(int i=0; i<holes.size(); i++){

        if(isAlmostSquare(holes[i])){
            CCheckBox cb;
             cb.outerBBox = holes[i];
            cb.validOuterBBox = true;
            cb.innerBBox = holes[i];
            cb.validInnerBBox = true;
            usedInner[i] = true;

            cBoxes.push_back(cb);
            //rectangle(imgMAT, rboxes[i], Scalar(0,0,255), 2);
        }

    }


    fillRatioFilter(imgMATbin, cBoxes);

    for(int i=0; i<cBoxes.size(); i++){
        CCheckBox cb = cBoxes[i];
        if(cb.innerFillRatio < 0.05){
            cBoxes[i].isFilled = false;
        } else {
            cBoxes[i].isFilled = true;
        }
    }

    return true;
}

bool CDetectCheckBoxes::detectCheckBoxes(string &inFileName, vector<CCheckBox> &cBoxes){

    string::size_type pAtExt = inFileName.find_last_of('.');   // Find extension point
    string fileBaseName = inFileName.substr(0,pAtExt);

    // Read image with OpenCV
    Mat imgMATgray = imread(inFileName, IMREAD_GRAYSCALE);
    if(imgMATgray.empty()){
        fprintf(stderr, "Error: Could not read image %s with OPENCV! Skipping ...\n", inFileName.c_str());
        return false;
    }
    Mat imgMAT = imread(inFileName);

    Mat imgMATbin, imgMATbinInv, imgMATbinMorph, imgMATbinH, imgMATbinV;
    binarizeOtsu(imgMATgray, imgMATbin);
    imgMATbinInv = 255 - imgMATbin;
    Mat elementH(2, 20, CV_8U, cv::Scalar(1));
    Mat elementV(20, 2, CV_8U, cv::Scalar(1));
    erode(imgMATbinInv, imgMATbinH, elementH);
    erode(imgMATbinInv, imgMATbinV, elementV);
    dilate(imgMATbinH, imgMATbinH, elementH);
    dilate(imgMATbinV, imgMATbinV, elementV);
    imgMATbinMorph = imgMATbinH | imgMATbinV;
    string binFile = fileBaseName + "-bin.png";
    imwrite(binFile, imgMATbin);
    string binMFile = fileBaseName + "-binMorph.png";
    imwrite(binMFile, imgMATbinMorph);
    string binVFile = fileBaseName + "-binV.png";
    imwrite(binVFile, imgMATbinV);
    string binHFile = fileBaseName + "-binH.png";
    imwrite(binHFile, imgMATbinH);

    //Mat imgMATholes;
    //Mat element5(7, 7, CV_8U, cv::Scalar(1));
    //dilate(imgMATbin, imgMATholes, element5);
    //erode(imgMATholes, imgMATholes, element5);

    ifstream infile("coords.csv");
    if (!infile.is_open()){
        fprintf(stderr, "%s", "file not open!\n");
        exit(-1);
    }
    vector <int> x, y, width, height;
    string line, temp;

    while (getline(infile, line)){
        istringstream lines(line);
        getline(lines, temp, ',');
        x.push_back(atoi(temp.c_str()));
        getline(lines, temp, ',');
        y.push_back(atoi(temp.c_str()));
        getline(lines, temp, ',');
        width.push_back(atoi(temp.c_str()));
        getline(lines, temp, ',');
        height.push_back(atoi(temp.c_str()));
    }

    for (int j = 0; j < x.size(); j++)
        fprintf(stderr, "%d " "%d " "%d " "%d \n", x[j], y[j], width[j], height[j]);

    vector<Rect> holes;
    int ex_size = 0;
    for (size_t i = 0; i < x.size(); i++){
        Mat new_mat = imgMATbinMorph(Rect(x[i], y[i], width[i], height[i]));
        imshow("out", new_mat);
        waitKey(0);
        destroyWindow("out");
        Preprocessor::conCompFast(new_mat, holes, 0.9, 0.9, 200, 4);
        for (int k = ex_size; k < holes.size(); k++){
            holes[k].x += x[i];
            holes[k].y += y[i];
        }
        ex_size = holes.size();
    }
    mergeHoles(holes);
    fprintf(stderr, "%s ", "done!");

    imgMATbinMorph = 255 - imgMATbinMorph;
    vector<Rect> rboxes;
    ex_size = 0;
    for (size_t i = 0; i < x.size(); i++){
        Mat new_mat = imgMATbinMorph(Rect(x[i], y[i], width[i], height[i]));
        /*imshow("out", new_mat);
        waitKey(0);
        destroyWindow("out");*/
        Preprocessor::conCompFast(new_mat, rboxes, 0.9, 0.9, 200, 4);
        for (int k = ex_size; k < rboxes.size(); k++){
            rboxes[k].x += x[i];
            rboxes[k].y += y[i];
        }
        ex_size = rboxes.size();
    }
    fprintf(stderr, "%s\n", "done!");

    vector<bool> usedInner(holes.size(), false);
    for(int i=0; i<rboxes.size(); i++){
        if(isAlmostSquare(rboxes[i])){
            CCheckBox cb;
            cb.outerBBox = rboxes[i];
            cb.validOuterBBox = true;
            for(int j=0; j<holes.size(); j++){
                Rect inter = rboxes[i] & holes[j];
                if(inter.area() == holes[j].area()){
                    cb.innerBBox = holes[j];
                    cb.validInnerBBox = true;
                    usedInner[j] = true;
                    break;
                }
            }
            cBoxes.push_back(cb);
            //rectangle(imgMAT, rboxes[i], Scalar(0,0,255), 2);
        }
    }
    for(int i=0; i<holes.size(); i++){
        if(!usedInner[i]){
            CCheckBox cb;
            cb.innerBBox = holes[i];
            cb.validInnerBBox = true;
            cBoxes.push_back(cb);
            //rectangle(imgMAT, holes[i], Scalar(255,0,0), 2);
        }
    }

    fillRatioFilter(imgMATbin, cBoxes);

    for(int i=0; i<cBoxes.size(); i++){
        CCheckBox cb = cBoxes[i];
        if(cb.innerFillRatio < 0.1){
            cBoxes[i].isFilled = false;
            if(cb.validOuterBBox){
                rectangle(imgMAT, cb.outerBBox, Scalar(0,0,255), 2);
            } else if(cb.validInnerBBox){
                rectangle(imgMAT, cb.innerBBox, Scalar(0,0,255), 2);
            }
        } else {
            cBoxes[i].isFilled = true;
            if(cb.validOuterBBox){
                rectangle(imgMAT, cb.outerBBox, Scalar(255,0,0), 2);
            } else if(cb.validInnerBBox){
                rectangle(imgMAT, cb.innerBBox, Scalar(255,0,0), 2);
            }
        }

        char score[80];
        sprintf(score, "%.2f", 100.0 * cb.innerFillRatio);
        string str(score);
        Point org;
//                if(cb.validOuterBBox)
//                    org = Point (cb.outerBBox.y + cb.outerBBox.height, cb.outerBBox.x + cb.outerBBox.width + 5);
//                else
//                    org = Point (cb.innerBBox.y + cb.innerBBox.height, cb.innerBBox.x + cb.innerBBox.width + 15);

        if(cb.validOuterBBox)
            org = Point (cb.outerBBox.x + cb.outerBBox.width, cb.outerBBox.y + cb.outerBBox.height );
        else
            org = Point (cb.innerBBox.x + cb.innerBBox.width, cb.innerBBox.y + cb.innerBBox.height );

        //putText(imgMAT, str, org, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,0), 2);

    }

    string ccFile = fileBaseName + "-cc.png";
    imwrite(ccFile, imgMAT);


    return true;
}

