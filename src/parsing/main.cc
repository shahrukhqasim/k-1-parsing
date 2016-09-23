#include <iostream>

//#include "stdio.h"
#include <vector>
#include <tesseract/baseapi.h>
#include "opencv2/core/core.hpp"

#include <regex>

using namespace std;
using namespace tesseract;
using namespace cv;

//#include <iostream>
#include "interfaces/TreeFormModelInterface.h"
#include "implementation/TreeFormModel.h"
#include "implementation/TreeFormProcessor.h"
//#include "fstream"
#include "implementation/BasicForm.h"
#include "accuracy_test/InputFieldsAccuracyTest.h"

void getWords(Json::Value root, std::vector<TextualData> &outputVector) {
    root = root["Pages"][0];

    Json::Value words = root["Words"];
    for (unsigned int i = 0; i < words.size(); i++) {
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

inline bool readJson(const std::string& path, Json::Value&output) {
    std::ifstream file(path);
    file>>output;
    return 0;
}

void batchProcess(std::string parentPath, bool evaluate) {
    if (parentPath[parentPath.length() - 1] != '/')
        parentPath = parentPath + "/";

//    cout << "Running on " << parentPath << endl;

    std::fstream streamImageFilesList(parentPath + "images/files.txt");
    std::fstream streamJsonFilesList(parentPath + "text/files.txt");
    std::fstream streamGroundTruthFilesList;

    if (evaluate)
        streamGroundTruthFilesList.open(parentPath + "groundTruth/files.txt");

    std::string imageFile;
    std::string ocrTextFile;
    std::string groundTruthFile;
    std::string modelFilePath = parentPath + "model.mdl";

    std::string outputFolder = parentPath + "output/";



    std::shared_ptr<TreeFormModelInterface> model = TreeFormModel::constructFormModel(ifstream(modelFilePath));
    TreeFormProcessor processor(model);

    Json::Value bindingsFile;
    readJson(parentPath+"groundTruthBindings.json",bindingsFile);

    float accuracySum = 0;
    int num = 0;

    while (getline(streamImageFilesList, imageFile)) {
        getline(streamJsonFilesList, ocrTextFile);
        groundTruthFile = "";
        if (evaluate)
            getline(streamGroundTruthFilesList, groundTruthFile);

        if (groundTruthFile.length() == 0 && evaluate) {
            std::cerr << "Ground truth not found" << std::endl;
        }

        std::string workFile = HelperMethods::removeFileExtension(imageFile);

        imageFile = parentPath + "images/" + imageFile;
        ocrTextFile = parentPath + "text/" + ocrTextFile;
        if (groundTruthFile.length() != 0)
            groundTruthFile = parentPath + "groundTruth/" + groundTruthFile;
        std::cout << imageFile << std::endl;
        std::cout << ocrTextFile << std::endl;
        std::cout << groundTruthFile << std::endl;


        //float x = Processor2(imageFile, ocrTextFile, groundTruthFile, modelFilePath, outputFolder, workFile).execute();

        std::cout<<imageFile<<std::endl;
        cv::Mat image;
        image=cv::imread(imageFile,1);

        Json::Value textJson;
        readJson(ocrTextFile,textJson);

        std::vector<TextualData>text;
        getWords(textJson,text);


        std::shared_ptr<BasicForm> basicForm=std::shared_ptr<BasicForm> (new BasicForm(image,text));

        if(!processor.processForm(basicForm)) {
            std::cerr << "Error processing form: " << imageFile << std::endl;
            continue;
        }
        Json::Value outputFromProcessor;
        processor.getResult(outputFromProcessor);


        imwrite(outputFolder+workFile+".checkboxes.png", processor.getCheckboxesImage());
        imwrite(outputFolder+workFile+".division.png", processor.getDivisionImage());
        imwrite(outputFolder+workFile+".fields.png", processor.getFieldsImage());

        std::ofstream outputResultStream(outputFolder+workFile+".fields.json");
        outputResultStream<<outputFromProcessor;

        if(groundTruthFile.length()!=0) {
            Json::Value gtJson;
            readJson(groundTruthFile,gtJson);

            cv::Mat imageOutputAccuracy=image.clone();

            InputFieldsAccuracyTest tester(bindingsFile,gtJson,outputFromProcessor,imageOutputAccuracy);
            float acc=tester.calculateAccuracy(true,true,true);
            num++;
            accuracySum+=acc;

            imwrite(outputFolder+workFile+".accuracy.png", tester.getErrorsImage());

            std::cout<<"Accuracy ="<<acc<<std::endl;
        }

    }

    if (evaluate)
        std::cout << "Average accuracy " << accuracySum / num << std::endl;

}

/**
 * This function prints arguments help
 */
void printHelp() {
    cout<<"Argument 1 should either be -e, or -p to run only  processing evaluation program or processing program respectively."<<endl<<endl;
    cout<<" * In case of -p, you must give path as second argument of a folder which contains these directories:\n";
    cout<<"\t * 1. images - should contain png files of the document images along with files.txt which must contain names (not path) of the image files"<<endl;
    cout<<"\t * 2. output - an empty folder which will get the output. Existing files will be overwritten"<<endl;
    cout<<"\t * 3. text - a file which must contain OCR output of the document in JSON agreed upon format"<<endl<<endl;
    cout<<" * In case of -e, you must give path as second argument of a folder which contains these directories:\n";
    cout<<"\t * 1. groundTruth - should contain JSON files of the ground truth in the agreed upon format along with files.txt which must contain names (not path) of the JSON ground truth files"<<endl;
    cout<<"\t * 2. images - should contain png files of the document images along with files.txt which must contain names (not path) of the image files"<<endl;
    cout<<"\t * 3. output - an empty folder which will get the output. Existing files will be overwritten"<<endl;
    cout<<"\t * 4. text - a file which must contain OCR output of the document in JSON agreed upon format"<<endl;
    cout<<endl<<endl<<"All files.txt should contain consecutive filenames. First document has to appear at the top and so on"<<endl;
    // TODO: Show prompt for processing program and and model builder program
}


/**
 * This function is the main entry point of the program which takes command line arguments
 * @param[in] argc number of arguments which include program binary path
 * @param[in] argv the argument strings themselves
 *
 * @return insignificant. Will return 0 in most cases.
 */
int main(int argc, char**argv) {
    if(argc!=3) {
        cout<<"Error in arguments."<<endl;
        printHelp();
        exit(0);
    }

    // Firt argument is always -o or -a. If it is -o, we have to run OCR Job+Accuracy Job
    string job=argv[1];
    // Second argument is the path to the parent directory. Which must contain various folders
    string path=argv[2];


    if(job==string("-p")) {
        batchProcess(path,false);
    }
    else if(job==string("-e")) {
        batchProcess(path,true);
    }
    else {
        cout<<"Error in argument 1. It can either be -a, -o, -p or -m"<<endl;
    }


	return 0;
}
