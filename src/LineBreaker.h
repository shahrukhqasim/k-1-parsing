//
// Created by Talha Rehman on 7/1/16.
//

#include "OcrProgram.h"

#ifndef K_1_PARSING2_LINEBREAKER_H
#define K_1_PARSING2_LINEBREAKER_H


/**
 * We are not using this class. This is suposed to do line segmentation. But it is not working well.
 */
class LineBreaker {
public:
    LineBreaker(const Mat&image);
    void run();
    void getResult(vector<Rect>&result);
private:
    double calculateMean(vector<int>value);
    void projectionProfile(Mat& input, vector<int>& P);
    void cropImage(Mat& input, Mat& output, vector<Rect>& rboxes);
    void distanceBwBoxes(vector<Rect>& rboxes, vector<int>& dist);
    void cleanBoxes(vector<Rect>& rboxes, vector<int>& areaofCC);
    void sortBoxes(vector<Rect>& output);
    void mergeBoxes(vector<Rect>& rboxes, vector<int>& dist );
    void conCompFast(cv::Mat &img, std::vector<cv::Rect> &rboxes, int type,std::vector<int>& areaofConComp);
    vector<Rect> rboxes;
    Mat img;
};


#endif //K_1_PARSING2_LINEBREAKER_H
