#include "OcrProgram.h"
#include "AccuracyProgram.h"

#include "regex"
using namespace std;
using namespace cv;

OcrProgram::OcrProgram(string inputFileName, string inputFolder, string outputFolder) {
    this->inputFileName=inputFileName;
    this->inputFolder=inputFolder;
    this->outputFolder=outputFolder;
}

void OcrProgram::loadBinaryImage(cv::Mat &image, string fileName, int mode) {
    Mat image2 = imread(fileName, mode);   // Read the file
    if (!image2.data)                      // Check for invalid input
    {
        cout << "Could not open or find the image: " << fileName << std::endl;
        exit(0);
    }
    Preprocessor::binarizeShafait(image2,image,100,0.5);

}


void OcrProgram::run() {
    // Load the binary image from the path provided
    loadBinaryImage(originalImage, inputFolder + "/" + inputFileName, 0);

    // Then compute its segmentation
    doSegmentation();

    // Clean the image of any rulings and write it to disk
    cleanImageAndWriteToDisk();

    // Run two phase OCR
    runOcr();

    // Output OCR results to a JSON file
    outputToJson();

    // Output plotted image file to the folder provided
    outputResult();
}

void OcrProgram::doSegmentation() {
    // Clone the input image so we can safely make changes
    Mat image=originalImage.clone();

    // Invert black and white colors
    Preprocessor::invertImage(image);

    // Compute big black connnected components and store the result in segments
    // These have to be the rulings
    Preprocessor::conCompFast(image,segments,0.5,0.5,1024,4);
}

void OcrProgram::runOcr() {
    // First run OCR on the segments
    TesseractFinder finder1(outputFolder+"/"+cleanedImageFileName);
    finder1.run();

    // Then remove the segments
    Mat imageWithoutSegments=cleanedImage.clone();
    Scalar color(255, 255, 255);
    for (int i = 0; i < segments.size(); i++) {
        Rect box = segments[i];
        rectangle(imageWithoutSegments, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), color, CV_FILLED, 8,0);
    }

    // and output to a file
    string withoutSegmentsImageFileName=HelperMethods::removeFileExtension(inputFileName)+"_withoutSegments.png";
    HelperMethods::outputImage(imageWithoutSegments,outputFolder+"/"+withoutSegmentsImageFileName);

    // Run OCR on parts which weren't part of any of segments
    TesseractFinder finder2(outputFolder+"/"+withoutSegmentsImageFileName,segments);
    finder2.run();

    // Now merge the results of both runs
    data=finder1.getRecognizedData();
    cout<<"SIze is "<<data.size()<<endl;
    vector<OcrResult>data2=finder2.getRecognizedData();
    data.insert(data.end(), data2.begin(), data2.end());


    // This will segment any missing sentences and remove leading and training white
    // spaces using regular expressions. It will also remove any leading or trailing "\n"
    // Note that "\n" is not a whitespace character but rather a texual thing
    // that tesseract ouputs occasionally
    vector<OcrResult> data3;
    // Remove leading and trailing whitespaces
    for(OcrResult& result : data) {
        result.text = regex_replace(result.text, regex("(^\\s+)|(\\s|\\\\n)+$"), "");
        vector<string>elements=HelperMethods::regexSplit(result.text);
        if(elements.size()>1) {
//            cout<<Rect(result.p1,result.p2)<<endl;
            Mat subImage=cleanedImage(Rect(result.p1,result.p2)).clone();
            vector<Rect> subRectangles;
            double percentage=0;
            double single=1.0*(result.p2.x-result.p1.x)/result.text.length();
            double startingX=result.p1.x;
            for(int i=0;i<elements.size();i++) {
                OcrResult subElement;
                subElement.text=elements[i];
                subElement.p1.y=result.p1.y;
                subElement.p2.y=result.p2.y;

                subElement.p1.x=startingX;
                startingX+=single*elements[i].length();
                subElement.p2.x=startingX;
                startingX+=single;

                data3.push_back(subElement);
            }
        }
        else {
            data3.push_back(result);
        }
    }
    data=data3;
}

void OcrProgram::cleanImageAndWriteToDisk() {
    // Compute all black connected components
    vector<Rect>boxes;
    Preprocessor::conCompFast(originalImage,boxes,1,1,0,8);

//    cleanedImage=originalImage.clone();

    // Make a new image matrix in which textual connected components will be copied
    cleanedImage=Mat(originalImage.rows,originalImage.cols, originalImage.type());

    // For white color
    const Scalar whiteColor(255,255,255);

    // Fill the new image with white
    rectangle(cleanedImage,Rect(0,0,cleanedImage.cols,cleanedImage.rows),whiteColor,CV_FILLED,8,0);

    // Run through all the connected components
    for(int i=0;i<boxes.size();i++) {
        // Get the current component
        Rect box=boxes[i];

        // If the connected component is small enough to be a textual data, copy it to the cleaned image
        if(!(box.width>10*box.height || box.width*10>originalImage.cols||box.height*10>originalImage.rows)) {
            Mat maskImage(originalImage.rows,originalImage.cols,originalImage.type());
            maskImage.setTo(0);
            rectangle(maskImage,box,255,CV_FILLED,8,0);
            originalImage.copyTo(cleanedImage,maskImage);
        }
    }

    // Write cleaned image to file
    cleanedImageFileName=HelperMethods::removeFileExtension(inputFileName)+"_cleaned.png";
    HelperMethods::outputImage(cleanedImage,outputFolder+"/"+cleanedImageFileName);
}

void OcrProgram::outputToJson() {
    Json::Value root;
    Json::Value pages;
    pages["PageNumber"]=1;
    pages["Width"]=originalImage.cols;
    pages["Height"]=originalImage.rows;

    Json::Value words;

    for(int i=0;i<data.size();i++) {
        OcrResult wordEntry=data[i];
        string s=wordEntry.text;
        Json::Value word;
        Json::Value rectangle;
        rectangle["l"]=wordEntry.p1.x;
        rectangle["t"]=wordEntry.p1.y;
        rectangle["r"]=wordEntry.p2.x;
        rectangle["b"]=wordEntry.p2.y;
        word["Value"]=s;
        word["Region"]=rectangle;

        pages["Words"][i]=word;
    }

    root["Pages"][0]=pages;

    fstream outputFile(outputFolder+"/"+HelperMethods::removeFileExtension(inputFileName)+".json",ios::out);
    outputFile<<root;
    outputFile.close();
}

void OcrProgram::outputResult() {
    Mat image;
    cvtColor(originalImage,image,CV_GRAY2RGB);

    for (int i = 0; i < data.size(); i++) {

        Point p1 = data[i].p1;
        Point p2 = data[i].p2;
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                              rng.uniform(0, 255));

        rectangle(image, p1, p2, color, 3, 8, 0);
    }

    HelperMethods::outputImage(image,outputFolder+"/"+inputFileName);
}

//void OcrProgram::oneToOneMatch() {
//	// TODO: Change path or something
//	csv::Parser file = csv::Parser(inputFolder+"/model.csv");
//
//	string mappedFieldName;
//	float left;
//	float top;
//	float width;
//	float height;
//
//	for (int i = 0; i < file.rowCount(); i++) {
//		mappedFieldName = file[i]["MappedFieldName"];
//		left = stof(file[i]["Left"]);
//		top = stof(file[i]["Top"]);
//		width = stof(file[i]["Width"]);
//		height = stof(file[i]["Height"]);
//
//		cout << "Read a line: " << mappedFieldName << endl;
//
//		Point tll(left * 2.777, (792 - top) * 2.777);
//		Point brr(tll.x + width * 2.777, tll.y + height * 2.777);
//
//        documentModelData.push_back( { tll, brr, mappedFieldName });
//	}
//}

void OcrProgram::runOcrProgram(string path) {
    // If path does not end with a '/', append one yourself
    if (path[path.length() - 1] != '/')
        path = path + '/';

    // Program input image files are named in files.txt in programInput folder.
    // It is read line by line using this stream object
    ifstream imageFilesInputStream(path+"programInput/files.txt");

    // Ground truth JSON files are named in files.txt in expectedOuput folder.
    // This is read line by line using this stream object
    ifstream expectedFilesInputStream(path+"expectedOutput/files.txt");

    // Contains the name of the current test case input image
    std::string imageFileNameLine;

    // Contains the name of the ground truth file of the test case (JSON)
    std::string expectedOuputFileNameLine;

    // For calculation of average, to store sum and num
    double accuracySum=0;
    int numForms=0;

    // Read each input image file name from stream to string line by line
    while(getline(imageFilesInputStream, imageFileNameLine)) {
        // There should be equal number of Ground Truth files. So read them as well
        getline(expectedFilesInputStream, expectedOuputFileNameLine);

        cout<<"Running on "<<imageFileNameLine<<endl;

        // Make a OCR job object and run it. Results will be written to the specified folder
        OcrProgram(imageFileNameLine,path+"programInput",path+"/programOutput").run();

        // Now put the path in these variables
        string programJsonFilePath=path+"programOutput/"+HelperMethods::removeFileExtension(imageFileNameLine)+".json";
        string expectedJsonFilePath=path+"expectedOutput/"+expectedOuputFileNameLine;
        string inputImagePath=path+"programInput/"+imageFileNameLine;
        string plotImagePath=path+"plottedDataComparison/"+imageFileNameLine;

        // And run teh accuracy test
        float newAccuracy=AccuracyProgram(programJsonFilePath,expectedJsonFilePath,inputImagePath,plotImagePath).run();

        // If the accuracy is zero, there must be something wrong otherwise add it to compute average
        if(newAccuracy!=0) {
            accuracySum += newAccuracy;
            numForms++;
        }
        else {
            cout<<"Error in "<<imageFileNameLine<<endl;
        }
        cout<<"Average so far: "<<accuracySum/numForms<<endl;

        cout<<"Run complete on "<<imageFileNameLine<<endl;
    }

    cout<<endl;
    cout<<"Average accuracy is: "<<accuracySum/numForms<<endl;
    cout<<"Receipts run: "<<numForms<<endl;
}
