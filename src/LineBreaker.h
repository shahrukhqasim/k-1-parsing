#include "OcrProgram.h"

#ifndef K_1_PARSING2_LINEBREAKER_H
#define K_1_PARSING2_LINEBREAKER_H


/**
 * We are not using this class. This is suposed to do line segmentation. But it is not working well.
 */
class LineBreaker {
public:
    LineBreaker(const cv::Mat&image);
    void run();
    void getResult(std::vector<cv::Rect>&result);
private:
    double calculateMean(std::vector<int>value);
    void projectionProfile(cv::Mat& input, std::vector<int>& P);
    void cropImage(cv::Mat& input, cv::Mat& output, std::vector<cv::Rect>& rboxes);
    void distanceBwBoxes(std::vector<cv::Rect>& rboxes, std::vector<int>& dist);
    void cleanBoxes(std::vector<cv::Rect>& rboxes, std::vector<int>& areaofCC);
    void sortBoxes(std::vector<cv::Rect>& output);
    void mergeBoxes(std::vector<cv::Rect>& rboxes, std::vector<int>& dist );
    void conCompFast(cv::Mat &img, std::vector<cv::Rect> &rboxes, int type,std::vector<int>& areaofConComp);
    std::vector<cv::Rect> rboxes;
    cv::Mat img;
};


#endif //K_1_PARSING2_LINEBREAKER_H
