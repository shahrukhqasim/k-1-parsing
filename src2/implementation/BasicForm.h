//
// Created by shahrukhqasim on 8/27/16.
//

#ifndef K1_PARSING_BASICFORM_H
#define K1_PARSING_BASICFORM_H


#include "../interfaces/RawFormInterface.h"

class BasicForm : public RawFormInterface{
protected:
    cv::Mat image;
    std::vector<TextualData>text;
public:

    BasicForm(const cv::Mat &image, const std::vector<TextualData> &text);

    virtual void getText(std::vector<TextualData> &text) override;

    virtual void getRasterImage(cv::Mat &image) override;

};


#endif //K1_PARSING_BASICFORM_H
