//
// Created by shahrukhqasim on 8/21/16.
//

#include <iostream>
#include "interfaces/TreeFormModelInterface.h"
#include "implementation/TreeFormModel.h"
#include "implementation/TreeFormProcessor.h"
#include "fstream"
#include "implementation/BasicForm.h"

void getWords(Json::Value root, std::vector<TextualData> &outputVector) {
    root = root["Pages"][0];

    Json::Value words = root["Words"];
    for (int i = 0; i < words.size(); i++) {
        Json::Value word = words[i];
        std::string value = word["Value"].asString();
        Json::Value rectangle = word["Region"];

        int t = rectangle["t"].asInt();
        int l = rectangle["l"].asInt();
        int b = rectangle["b"].asInt();
        int r = rectangle["r"].asInt();

        TextualData entry;
        entry.setRect(cv::Rect(l, t, r - l, b - t));
        entry.setText(value);
        outputVector.push_back(entry);
    }
}

int main() {
    // Static function because if MDL is invalid, it just returns null which can't be used further in a processor
    std::shared_ptr<TreeFormModelInterface> model = TreeFormModel::constructFormModel(std::ifstream("/home/shahrukhqasim/workspace/kbranches/k-1-parsing/data/processing/model.mdl"));

    std::cout<<"Model: "<<model<<std::endl;
    TreeFormProcessor processor(model);

    std::string imagePath="/home/shahrukhqasim/workspace/kbranches/k-1-parsing/data/processing/images/002 K1 Face Original.png";
    std::string ocrPath="/home/shahrukhqasim/workspace/kbranches/k-1-parsing/data/processing/text/002.K1.Face.Original.REDACTED-1.json";

    cv::Mat image=cv::imread(imagePath,1);
    Json::Value jsonWords;

    std::ifstream ocr(ocrPath);
    ocr>>jsonWords;

    std::vector<TextualData>text;
    getWords(jsonWords,text);

    std::shared_ptr<BasicForm> basicForm=std::shared_ptr<BasicForm> (new BasicForm(image,text));
    bool resultB=processor.processForm(basicForm);

    std::cout<<"Processed: "<<resultB<<std::endl;
    Json::Value result;
    std::cout<<processor.getResult(result)<<std::endl;

    std::cout<<result<<std::endl;

}