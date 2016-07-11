//
// Created by shahrukhqasim on 6/25/16.
//

#include "AccuracyProgram.h"

float AccuracyProgram::testAccuracy() {
    int size = expectedOutput.size();
    int matches = 0;

    // Those elements which are found are entered into this vector
    vector<TextualData> found;

    // Those elements from tesseract output which are matched to anything
    vector<TextualData> ocrMatched;


    // Go through OCR output
    for (int i = 0; i < ocrOutput.size(); i++) {
        // Go through expected output (Ground Truth)
        for (int j = 0; j < expectedOutput.size(); j++) {
            // If any matches
            if (ocrOutput[i].compare(expectedOutput[j])) {
                // Push the found element into found vector
                found.push_back(expectedOutput[j]);

                // Remove it from expected output. It cannot occur twice
                expectedOutput.erase(expectedOutput.begin() + j);

                // and add one to matches for accuracy figure
                matches++;
                break;

            }
        }
    }

    // Those elements from tesseract output which are not matched to anything
    vector<TextualData> notMatched;
    for (int i = 0; i < ocrOutput.size(); i++) {
        bool matched = false;
        for (int j = 0; j < ocrMatched.size(); j++) {
            if (ocrOutput[i].equals(ocrMatched[j])) {
                matched = true;
                break;
            }
        }
        if (!matched) {
            notMatched.push_back(ocrOutput[i]);
        }
    }


    // Now expectedOutput contains the elements not found and found vector contains the found elements
    // Moreover ocrMatched vector contains those elements which were not matched to anything
    // when compared

    // First draw all the found elements in green
    Scalar green(0, 255, 0);
    for (int i = 0; i < found.size(); i++) {
        rectangle(theImage, found[i].getRect(), green, 3, 8, 0);
    }


	//Then draw all the not found elements of expected output in red
    Scalar red(0, 0, 255);
    for (int i = 0; i < expectedOutput.size(); i++) {
        rectangle(theImage, expectedOutput[i].getRect(), red, 3, 8, 0);
    }

    // Then draw all the not found elements ocr output in blue
    Scalar blue(255, 0, 0);
    for (int i = 0; i < ocrOutput.size(); i++) {
        rectangle(theImage, ocrOutput[i].getRect(), blue, 3, 8, 0);
    }


    // Calculate accuracy percentage and return it
    if (size != 0) {
        return matches * 100.0 / size;
    }
    else
        return 0;
}


AccuracyProgram::AccuracyProgram(string programOutputFile, string expectedOutputFilename, string inputFile,
                                 string comparisonFile) {
    this->programOutputFile = programOutputFile;
    this->expectedOutputFile = expectedOutputFilename;
    this->inputFile = inputFile;
    this->comparisonFile = comparisonFile;
}

void AccuracyProgram::getWords(Json::Value root, vector<TextualData> &outputVector) {
    root = root["Pages"][0];

    Json::Value words = root["Words"];
    for (int i = 0; i < words.size(); i++) {
        Json::Value word = words[i];
        string value = word["Value"].asString();
        Json::Value rectangle = word["Region"];

        int t = rectangle["t"].asInt();
        int l = rectangle["l"].asInt();
        int b = rectangle["b"].asInt();
        int r = rectangle["r"].asInt();

        TextualData entry;
        entry.setRect(Rect(l, t, r - l, b - t));
        entry.setText(value);
        outputVector.push_back(entry);
    }
}

void AccuracyProgram::cleanWords(vector<TextualData> &root) {
    vector<TextualData> words2;
    for (int i = 0; i < root.size(); i++) {
        TextualData word = root[i];
        string w = word.getText();
        bool isVerified = false;
        // TODO: Change it use regular expressions
        for (int i = 0; i < w.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(w[i])) {
                isVerified = true;
                break;
            }
        }
        if (isVerified) {
            words2.push_back(word);
        }
    }
    root.clear();
    root.insert(root.begin(), words2.begin(), words2.end());
}


void AccuracyProgram::compare2(Json::Value rootProgram, Json::Value rootExpected) {
    // Get words from the JSON file
    getWords(rootProgram, ocrOutput);
    getWords(rootExpected, expectedOutput);

    // If there is no expected output, then there is no comparison
    if (expectedOutput.size() == 0) {
        successful = false;
    }
    else {
        successful = true;
    }

    // Remove all data entries which do not contain alpha-numeric characters
    cleanWords(expectedOutput);
    cout << "Number of entries in OCR result: " << ocrOutput.size() << " Number of entries in GT: " << expectedOutput.size() << endl;

    // Run the actual test
    accuracy = testAccuracy();
    cout << "Accuracy is " << accuracy << endl;
}

void AccuracyProgram::runAccuracyTest(string path) {
    // If path does not end with a '/', append one yourself
    if (path[path.length() - 1] != '/')
        path = path + '/';

    // We have three set of files

    // ocrJsonFiles.txt must be in directory programOutput. It is supposed to contain
    // name of OCR output JSON files without absolute path
    ifstream inputStream(path+"programOutput/ocrJsonFiles.txt");

    // This files.txt must be in directory programOuptut. It is supposed to contain
    // JSON files of ground truth
    ifstream inputStream2(path+"expectedOutput/files.txt");

    //This file.txt contains list of program input images and should be in programInput folder
    ifstream inputStream3(path+"programInput/files.txt");

    // Strings to read files line by line
    std::string lineOcrResult; // To read file names of OCR output JSON files
    std::string lineGroundTruth; // To read files names of the ground truth
    std::string lineProgramInputImages; // To read file names of the program input images


    // Just strings to hold path directories
    const string inputFolder = path+"programInput";
    const string outputFolder = path+"programOutput";
    const string expectedFolder = path+"expectedOutput";
    const string plotOutputFolder = path+"plottedDataComparison/";


    // To calculate average, will contain number of successful tests (where accuracy!=0)
    int num = 0;

    // To calculate average, will contain sum of accuracy percentage of successful tests (where accuracy!=0)
    float sum = 0;

    // Read all lines from ground truth
    while (getline(inputStream2, lineGroundTruth)) {
        // Number of lines in OCR result and program input images should be same so read them as well
        getline(inputStream, lineOcrResult);
        getline(inputStream3, lineProgramInputImages);

        // Make paths variables to hold current image test. Names are self explanatory
        string inputImagePath = inputFolder + "/" + lineProgramInputImages;
        string plotImagePath = plotOutputFolder + lineProgramInputImages;
        string programFilePath = outputFolder + "/" + lineOcrResult;
        string expectedFilePath = expectedFolder + "/" + lineGroundTruth;

        cout<<inputImagePath<<endl;

        cout << "Running on " << programFilePath << endl;

        // Now make an instance of AccuracyProgram to run test on one case
        AccuracyProgram program(programFilePath, expectedFilePath, inputImagePath, plotImagePath);

        // Run the test and store accuracy figure
        float accuracy = program.run();

        // If accuracy is not equal to zero, that means the test was successful
        if (accuracy != 0) {
            sum += accuracy;
            num++;
        }
        // Something must have gone wrong. Should not include that in test results.
        else {
            cerr<<"Error occured in "<<lineGroundTruth<<endl;
        }
    }

    // Print the average accuracy
    cout << "Average accuracy=" << sum / num << " from " << num << " forms" << endl;
}

float AccuracyProgram::run() {

    // Contains the JSON of OCR result
    Json::Value rootProgram;

    // Contains the JSON of the ground truth
    Json::Value rootExpected;

    // Read the JSONs and store in the above mentioned variables
    ifstream programStream(programOutputFile);
    programStream >> rootProgram;
    ifstream expectedStream(expectedOutputFile);
    expectedStream >> rootExpected;


    // Read the image file in grayscale
    Mat image = imread(inputFile, 0);
    if (!image.data) {
        cerr << "Could not read the input image file";
        exit(0);
    }

    // Change it to RGB to plot colored result
    cvtColor(image, theImage, CV_GRAY2RGB);

    // Run the test
    compare2(rootProgram, rootExpected);

    // And print the output plotted result file
    HelperMethods::outputImage(theImage, comparisonFile);

    // Return the accuracy to the caller
    return accuracy;
}
