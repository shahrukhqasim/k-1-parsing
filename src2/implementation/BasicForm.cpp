//
// Created by shahrukhqasim on 8/27/16.
//

#include "BasicForm.h"

void BasicForm::getText(std::vector<TextualData> &text) {
    text=BasicForm::text;
}

void BasicForm::getRasterImage(cv::Mat &image) {
    image=BasicForm::image;
}

BasicForm::BasicForm(const cv::Mat &image, const std::vector<TextualData> &text) : image(image), text(text) {}
