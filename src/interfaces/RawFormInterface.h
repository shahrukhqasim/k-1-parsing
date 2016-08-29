//
// Created by shahrukhqasim on 8/21/16.
//

#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../TextualData.h"

#ifndef K1_PARSING_RAWFORMINTERFACE_H
#define K1_PARSING_RAWFORMINTERFACE_H


class RawFormInterface {
public:
    virtual void getText(std::vector<TextualData>&text)=0;
    virtual void getRasterImage(cv::Mat&image)=0;
};


#endif //K1_PARSING_RAWFORMINTERFACE_H
