//
// Created by shahrukhqasim on 7/28/16.
//

#include "Processor2.h"
#include "AccuracyProgram.h"
#include "Model/ModelBuilder.h"
#include "Mapper.h"
#include "Model/RepeatInputNode.h"
#include "Model/MappingJob.h"


Processor2::Processor2(const string &imageFilePath, const string &textFilePath, const string &groundTruthFilePath,
                       const string &modelFilePath,
                       const string &outputFolder, const string &outputFileName) {
    this->imageFilePath = imageFilePath;
    this->textFilePath = textFilePath;
    this->groundTruthFilePath = groundTruthFilePath;
    this->modelFilePath = modelFilePath;
    this->outputFolder = outputFolder;
    this->outputFileName = outputFileName;
}


float Processor2::execute() {
    readData();
    divideIntoParts();
    ModelBuilder builder;
    documentNode = builder.execute(modelFilePath);


    Json::Value output2;


    processHeader1();
    processPart1();
    processPart2();
    processPart3();
    outputDataToJson();

//    Json::Value output;
//    ModelBuilder::convertToJson(output, documentNode);
//    cout << output;

    int tests = 0;
    int testsPassed = 0;
//    for_each(groundTruth.begin(),groundTruth.end(),[&] (pair<string,shared_ptr<GroundTruth>>x){
//        tests++;
//        const char*testX=x.first.c_str();
//        if(x.second->taken||x.second->value.length()==0)
//            testsPassed++;
//    });

    ofstream outputTree(outputFolder + "/" + outputFileName + "_tree.json");
    ModelBuilder::convertToJson(output2, documentNode->subNodes["DOCUMENT"]);
    outputTree << output2;
    outputTree.flush();
    outputTree.close();


    for (auto x:groundTruth) {
        if (x.second->value.compare("True") == 0 || x.second->value.compare("False") == 0) {
            continue;
        }

        tests++;
        const char *testX = x.first.c_str();

        string c;
        for (int i = 0; i < x.second->value.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(x.second->value[i]))
                c = c + x.second->value[i];
        }

        if (x.second->taken || c.length() == 0)
            testsPassed++;
        else
            cout << "Not passed " << x.first << " - " << x.second->value << endl;

    }

    accuracyTests = tests;
    this->testsPassed = testsPassed;

    if (accuracyTests != 0)
        return (100.0 * this->testsPassed / (float) accuracyTests);
    else
        return 0;
}

bool Processor2::isBelow(const Rect &a, const Rect &b) {
    if (a.y > b.y + b.height) {
        return true;
    }
    return false;
}

bool Processor2::isAbove(const Rect &a, const Rect &b) {
    if (a.y + a.height < b.y) {
        return true;
    }
    return false;
}


bool Processor2::isLeftTo(const Rect &a, const Rect &b) {
    if (a.x + a.width < b.x) {
        return true;
    }
    return false;
}

bool Processor2::isRightTo(const Rect &a, const Rect &b) {
    if (a.x > b.x + b.width) {
        return true;
    }
    return false;
}

string
Processor2::findTextWithRules(vector<std::function<bool(const TextualData &)>> rules, const vector<TextualData> &data) {
    string text = "";
    for_each(data.begin(), data.end(), [&](const TextualData &currentData) {
        bool ruleMatched = true;
        for_each(rules.begin(), rules.end(), [&](std::function<bool(const TextualData &)> currentRule) {
            ruleMatched = ruleMatched & currentRule(currentData);
        });
        if (ruleMatched) {
            text += " " + currentData.getText();

        }
    });

    return text;
}

string Processor2::findTextWithRulesOnlyRightMost(vector<std::function<bool(const
                                                                            TextualData &)>> rules,
                                                  const vector<TextualData> &data
) {
}

void Processor2::processHeader1() {
    shared_ptr<Node> header1Node = documentNode->subNodes["DOCUMENT"]->subNodes["HEADER_1"];
    Mapper mapper(header1Data, header1Node, documentNode, image.cols, image.rows);
    mapper.executeTextFields();
    mapper.executeInputFields();
}

void Processor2::processHeader2() {
}

void Processor2::processPart1() {
    shared_ptr<Node> part1Node = documentNode->subNodes["DOCUMENT"]->subNodes["PART_1"];
    Mapper mapper(part1Data, part1Node, documentNode, image.cols, image.rows);
    mapper.executeTextFields();
    mapper.executeInputFields();
}

void Processor2::processPart2() {
    shared_ptr<Node> part2Node = documentNode->subNodes["DOCUMENT"]->subNodes["PART_2"];
    Mapper mapper(part2Data, part2Node, documentNode, image.cols, image.rows);
    mapper.executeTextFields();
    mapper.executeInputFields();

    ofstream outta(outputFolder + "/");
}

void Processor2::processPart3() {
    shared_ptr<Node> part3Node = documentNode->subNodes["DOCUMENT"]->subNodes["PART_3"];
    Mapper mapper(part3Data, part3Node, documentNode, image.cols, image.rows);
    mapper.executeTextFields();
    mapper.executeInputFields();
}

void Processor2::outputDataToJson() {
    outputJson = Json::Value();
    outputJson["Pages"][0] = Json::Value();
    outputJson["Pages"][0]["PageNumber"] = 1;

    recursiveInputFieldsToJson(documentNode->subNodes["DOCUMENT"]);

    imwrite(outputFolder + outputFileName + "_output.png", image);

    ofstream outputStream(outputFolder + outputFileName + "_extracted.json");
    outputStream << outputJson;
}

void Processor2::outputBindingLine(shared_ptr<Node> node, Rect region) {
//    cout<<"Finding binding"<<mappedGround.size()<<endl;


    if (((int) node->id.find("HEADER_1")) >= 0) {
        for (auto i:mappedGroundH1) {
            rectangle(image, i.second, Scalar(0, 255, 0), 3, 8, 0);
            if ((i.second & region).area() != 0) {
                cout << "BIND " << node->id << " \"" << i.first << '\"' << endl;
            }
        }
    }
    else if (((int) node->id.find("HEADER_2")) >= 0) {
        for (auto i:mappedGroundH2) {
            rectangle(image, i.second, Scalar(0, 255, 0), 3, 8, 0);
            if ((i.second & region).area() != 0) {
                cout << "BIND " << node->id << " \"" << i.first << '\"' << endl;
            }
        }
    }
    else if (((int) node->id.find("PART_1")) >= 0) {
        for (auto i:mappedGroundP1) {
            rectangle(image, i.second, Scalar(0, 255, 0), 3, 8, 0);
            if ((i.second & region).area() != 0) {
                cout << "BIND " << node->id << " \"" << i.first << '\"' << endl;
            }
        }
    }
    else if (((int) node->id.find("PART_2")) >= 0 || ((int) node->id.find("TABLE")) >= 0) {
        for (auto i:mappedGroundP2) {
            rectangle(image, i.second, Scalar(0, 255, 0), 3, 8, 0);
            if ((i.second & region).area() != 0) {
                cout << "BIND " << node->id << " \"" << i.first << '\"' << endl;
            }
        }
    }
    else if (((int) node->id.find("PART_3")) >= 0) {
        for (auto i:mappedGroundP3) {
            rectangle(image, i.second, Scalar(0, 255, 0), 3, 8, 0);
            if ((i.second & region).area() != 0) {
                cout << "BIND " << node->id << " \"" << i.first << '\"' << endl;
            }
        }
    }
}

void Processor2::visualize(shared_ptr<Node> node) {

    if(dynamic_pointer_cast<InputNode>(node)) {
        shared_ptr<InputNode>iModel=dynamic_pointer_cast<InputNode>(node);

        if ((int) (node->id.find("TABLE")) >= 0)
            return;
        vector<string> hierarchy = HelperMethods::regexSplit(node->id, "[:]");
        if (hierarchy.size() != 0) {
            hierarchy = vector<string>(hierarchy.begin(), hierarchy.end() - 1);
            shared_ptr<Node> foundNode = ModelBuilder::findNode(hierarchy, documentNode);
            if (foundNode != nullptr) {
                if (foundNode->regionDefined) {
                    Scalar randomColor=randomColors[((unsigned int) rng) % 5];
                    rectangle(image, foundNode->region, randomColor, 3, 8, 0);
                    putText(image, iModel->data, foundNode->region.br(), 1, 2, randomColor, 2);
                }
            }
        }
    }
}

void Processor2::testAccuracy(shared_ptr<InputNode> node) {
//    cout<<"Figure="<<node->bindedGroundTruthEntries.size()<<endl;

//     Check only alpha numeric stuff
    string c;
    for (int i = 0; i < node->data.size(); i++) {
        if (HelperMethods::isAlphaNumericNotSpace(node->data[i]))
            c = c + node->data[i];
    }

    if (c.length() == 0) {
        return;
    }
    bool doBreak = false;
    bool matched = false;
    accuracyTests++;



    vector<string> values = HelperMethods::regexSplit(node->data, "[|]");

    for (string j:values) {
        for (auto i:node->bindedGroundTruthEntries) {
            if (groundTruth.find(i) == groundTruth.end())
                continue;
            shared_ptr<GroundTruth> g = groundTruth[i];


            if (g == nullptr || g->taken)
                continue;
            if (HelperMethods::nearEqualComparison(g->value, j)) {

                Scalar randomColor = randomColors[(int) rng % 5];//Scalar((int) rng % 256, (int) rng % 256, (int) rng % 256);

//                rectangle(image, g->rect, randomColor, 3, 8, 0);

//                vector<string> hierarchy = HelperMethods::regexSplit(node->id, "[:]");
//                if (hierarchy.size() != 0) {
//                    hierarchy = vector<string>(hierarchy.begin(), hierarchy.end() - 1);
//                    shared_ptr<Node> foundNode = ModelBuilder::findNode(hierarchy, documentNode);
//                    if (foundNode != nullptr) {
//                        if (foundNode->regionDefined) {
//                            rectangle(image, foundNode->region, randomColor, 3, 8, 0);
//                            rectangle(image, node->region, randomColor, 3, 8, 0);
//                        }
//                    }
//                }


//                rectangle(image, node->region, randomColor, 3, 8, 0);
//                putText(image, j, g->rect.tl(), 1, 2, randomColor);

                g->taken = true;
                break;
            }
        }
    }

//    int bindedIndexLoop = 0;
//    for (auto i:node->bindedGroundTruthEntries) {
//
//        if (groundTruth.find(i) == groundTruth.end())
//            continue;
//        shared_ptr<GroundTruth> g = groundTruth[i];
//
//
//        if (g == nullptr || g->taken)
//            continue;
//
////        cout<<node->data<<endl;
//        vector<string> values = HelperMethods::regexSplit(node->data, "[|]");
//
//        if (values.size() > 0) {
////            cout<<"HELPPPPP";
////            cout<<node)<<endl;
//        }

//        for (auto x:values) {
//            if (HelperMethods::nearEqualComparison(g->value, x)) {
//                Scalar randomColor = Scalar((int) rng % 256, (int) rng % 256, (int) rng % 256);
//
////                rectangle(image, g->rect , randomColor, 3, 8, 0);
////                rectangle(image, node->region , randomColor, 3, 8, 0);
////
////                putText(image,node->data,g->rect.tl(),1,2,randomColor);
//
//                g->taken = true;
//
////                cout << "Passed:" << node->id << endl;
//
//
//                testsPassed++;
//                return;
//            }
//        }
//        if (doBreak)
//            break;
//    }
//    if(!matched);
//        cout << "Not Passed:" << node->id<<" - "<<node->data << endl;
}

void Processor2::recursiveInputFieldsToJson(shared_ptr<Node> node) {
    if (dynamic_pointer_cast<InputNode>(node) != nullptr) {
//        cout << "Running on: " << node->id << endl;
        shared_ptr<InputNode> iModel = dynamic_pointer_cast<InputNode>(node);

        Json::Value value;
        value["Id"] = iModel->id;
        value["Value"] = iModel->data;

        Json::Value region;
        region["l"] = iModel->region.x;
        region["t"] = iModel->region.y;
        region["r"] = iModel->region.x + iModel->region.width;
        region["b"] = iModel->region.y + iModel->region.height;

        value["Region"] = region;

        Rect regionX = MappingJob(documentNode, iModel->id, image.cols, image.rows).map();


        if (groundTruthFilePath.length() != 0)
            testAccuracy(iModel);
        visualize(iModel);

        Mat image2 = image.clone();

        outputJson["Pages"][0]["Fields"][lastIndexJson++] = value;
    }
    if (dynamic_pointer_cast<TableNode>(node) != nullptr) {
        shared_ptr<TableNode> tModel = dynamic_pointer_cast<TableNode>(node);

        if (tModel->tableEntries.size() != 0) {
            for_each(tModel->tableEntries.begin(), tModel->tableEntries.end(),
                     [&](pair<string, shared_ptr<Node>> current) {
                         recursiveInputFieldsToJson(current.second);
                     });
        }
    }

    if (node->subNodes.size() != 0) {
        for_each(node->subNodes.begin(), node->subNodes.end(), [&](pair<string, shared_ptr<Node>> current) {
            recursiveInputFieldsToJson(current.second);
        });
    }
}

TextualData Processor2::extractTextualDataType1(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part1Data, key);
    TextualData d = part1Data[index];
    part1Data.erase(part1Data.begin() + index);
    return d;
}

TextualData Processor2::extractTextualDataType2(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part2Data, key);
    TextualData d = part2Data[index];
    part2Data.erase(part2Data.begin() + index);
    return d;
}

TextualData Processor2::extractTextualDataType3(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part3Data, key);
    TextualData d = part3Data[index];
    part3Data.erase(part3Data.begin() + index);
    return d;
}


void Processor2::divideIntoParts() {
    mergeWordBoxes(words, mergedWords);

    Mat image = this->image.clone();


    Mat vProjection(1, image.cols, CV_16U);
    vProjection = 0;
    for (int i = 0; i < mergedWords.size(); i++) {
        Rect r = mergedWords[i].getRect();
        for (int j = r.x; j < r.x + r.width; j++)
            vProjection.at<short>(0, j) += r.height;
    }

    for (int i = 0; i < vProjection.cols; i++) {
//        line(image,Point(i,0),Point(i,vProjection.at<short>(0,i)),Scalar(255,0,0));
    }

    int quarter = vProjection.cols / 6;

    int shallowProjectionIndex = quarter;
    for (int i = quarter * 2; i < quarter * 4; i++) {
        if (vProjection.at<short>(0, i) < vProjection.at<short>(0, shallowProjectionIndex))
            shallowProjectionIndex = i;
    }

    xDivisionCoordinate = shallowProjectionIndex;

    // Draw the separation line
    line(image, Point(shallowProjectionIndex, 0), Point(shallowProjectionIndex, image.rows - 1), Scalar(0, 255, 0), 7,
         8, 0);

    vector<TextualData> leftBoxes;
    vector<TextualData> rightBoxes;


    vector<pair<string, Rect>> mappedGroundL;
    vector<pair<string, Rect>> mappedGroundR;

    for (int i = 0; i < mergedWords.size(); i++) {
        TextualData box = mergedWords[i];
        if (shallowProjectionIndex - box.getRect().x > box.getRect().width / 2) {
            leftBoxes.push_back(box);
        }
        else {
            rightBoxes.push_back(box);
        }
    }


    for (int i = 0; i < mappedGround.size(); i++) {
        pair<string, Rect> box = mappedGround[i];
        if (shallowProjectionIndex - box.second.x > box.second.width / 2) {
            mappedGroundL.push_back(box);
        }
        else {
            mappedGroundR.push_back(box);
        }
    }



    // Find Information About the Partnership : Part 1
    int indexPart1 = findMinTextIndex(leftBoxes, "Information About the Partnership");
    int indexPart2 = findMinTextIndex(leftBoxes, "Information About the Partner");
    int indexPart3 = findMinTextIndex(rightBoxes, "Partner's Share of Current Year Income");

    if (indexPart1 != -1 || indexPart2 != -1 && indexPart3 != -1) {
        TextualData part1TextualData = leftBoxes[indexPart1];
        TextualData part2TextualData = leftBoxes[indexPart2];
        TextualData part3TextualData = rightBoxes[indexPart3];

        Rect part1Rect = part1TextualData.getRect();
        Rect part2Rect = part2TextualData.getRect();
        Rect part3Rect = part3TextualData.getRect();

        // Search in left boxes which are below indexPart1 and above indexPart2: Add them to part 1
        // Search in left boxes which are below indexPart2: Add them to part 2
        for (int i = 0; i < leftBoxes.size(); i++) {
            TextualData current = leftBoxes[i];

            // TODO: Change 10 offset to something dynamic
            if ((current.getRect().y) >= part2TextualData.getRect().y - 10) {
                part2Data.push_back(current);
                part2Rect = part2Rect | current.getRect();
            }
            else if (current.getRect().y >= part1TextualData.getRect().y &&
                     current.getRect().y < part2TextualData.getRect().y) {
                part1Data.push_back(current);
                part1Rect = part1Rect | current.getRect();
            }
            else if (current.getRect().y < part1TextualData.getRect().y - 10) {
                header1Data.push_back(current);
            }
        }

        // Search in right boxes which are below indexPart3: Add them to part 3
        for (int i = 0; i < rightBoxes.size(); i++) {
            TextualData current = rightBoxes[i];
            if (current.getRect().y >= part3TextualData.getRect().y) {
                part3Data.push_back(current);
            }
            else if (current.getRect().y < part3TextualData.getRect().y - 10) {
                header2Data.push_back(current);
            }
        }


        // Search in left boxes which are below indexPart1 and above indexPart2: Add them to part 1
        // Search in left boxes which are below indexPart2: Add them to part 2
        for (int i = 0; i < mappedGroundL.size(); i++) {
            pair<string, Rect> current = mappedGroundL[i];

            // TODO: Change 10 offset to something dynamic
            if ((current.second.y) >= part2TextualData.getRect().y - 10) {
                mappedGroundP2.push_back(current);
            }
            else if (current.second.y >= part1TextualData.getRect().y &&
                     current.second.y < part2TextualData.getRect().y) {
                mappedGroundP1.push_back(current);
            }
            else if (current.second.y < part1TextualData.getRect().y - 10) {
                mappedGroundH1.push_back(current);
            }
        }

        // Search in right boxes which are below indexPart3: Add them to part 3
        for (int i = 0; i < mappedGroundR.size(); i++) {
            pair<string, Rect> current = mappedGroundR[i];
            if (current.second.y >= part3TextualData.getRect().y) {
                mappedGroundP3.push_back(current);
            }
            else if (current.second.y < part3TextualData.getRect().y - 10) {
                mappedGroundH2.push_back(current);
            }
        }


        drawBoxes(image, part1Data, Scalar(255, 0, 0));
        drawBoxes(image, part2Data, Scalar(0, 255, 0));
        drawBoxes(image, part3Data, Scalar(0, 0, 255));
        drawBoxes(image, part1Data, Scalar(255, 255, 0));
        drawBoxes(image, header1Data, Scalar(0, 255, 255));
        drawBoxes(image, header2Data, Scalar(255, 0, 255));
    }

    // Output the file
    string fileName = outputFileName + "_divided.png";
    imwrite(outputFolder + fileName, image);

}

void Processor2::getFieldValues(Json::Value root, vector<TextualData> &outputVector) {

    root = root["Pages"][0];

    Json::Value words = root["Fields"];
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

void Processor2::readData() {

    if (groundTruthFilePath.length() != 0) {
        cout << groundTruthFilePath << endl;
        ifstream theFile(groundTruthFilePath);
        Json::Value parsedData;
        theFile >> parsedData;

        parsedData = parsedData["Pages"][0]["Fields"];

        vector<Rect> rectangles;

        for (int i = 0; i < parsedData.size(); i++) {
            Json::Value region = parsedData[i]["Region"];
            string name = parsedData[i]["Name"].asString();
            string value = parsedData[i]["Value"].asString();

            int l = region["l"].asInt();
            int t = region["t"].asInt();
            int r = region["r"].asInt();
            int b = region["b"].asInt();

            mappedGround.push_back(pair<string, Rect>(name, Rect(l, t, r - l, b - t)));


//        cout<<"Inserting "<<name<<endl;
            //{Rect(l, t, r - l, b - t),value}
            groundTruth[name] = shared_ptr<GroundTruth>(new GroundTruth(Rect(l, t, r - l, b - t), value));

        }
    }

    // Read the input image
    image = imread(imageFilePath, 1);
    if (!image.data)
        cerr << "Error reading form";

    // Read the OCR result
    Json::Value jsonWords;
    ifstream jsonOcrWordsStream(textFilePath);
    jsonOcrWordsStream >> jsonWords;
    AccuracyProgram::getWords(jsonWords, words);
}

void Processor2::mergeWordBoxes(const vector<TextualData> &words, vector<TextualData> &elemBoxes) {
    // Merge the words extracted from Tesseract to obtain text-lines. The logic used for text-line extraction
    // is to merge two consecutive words if they overlap along the y-axis, and the gap between them is smaller
    // than the height of the shorter word.
    int nRects = words.size();
    bool newElem = true;
    TextualData elem, prevWord;
    for (int i = 0; i < nRects; i++) {
        TextualData currWord = words[i];
        if (!newElem) {
            int hGap = currWord.getRect().x - prevWord.getRect().x - prevWord.getRect().width;
            int hGapThresh = max(currWord.getRect().height, prevWord.getRect().height);
            bool vOverlap = false;
            if (((currWord.getRect().y <= prevWord.getRect().y) &&
                 (currWord.getRect().y + currWord.getRect().height > prevWord.getRect().y)) ||
                ((prevWord.getRect().y <= currWord.getRect().y) &&
                 (prevWord.getRect().y + prevWord.getRect().height > currWord.getRect().y)))
                vOverlap = true;
            if (vOverlap && (hGap > 0) && (hGap < hGapThresh)) {
                elem = elem | currWord;
                prevWord = currWord;
            }
            else {
//                if(elem.width > elem.height){
                elemBoxes.push_back(elem);
//                }
                newElem = true;
            }
        }
        if (newElem) {
            elem = currWord;
            newElem = false;
            prevWord = currWord;
            continue;
        }
    }
    elemBoxes.push_back(elem);

}

int Processor2::findMinTextIndex(const vector<TextualData> &data, const string &textToFind) {
    int minDistance = 99999999;
    int minIndex = -1;
    for (int i = 0; i < data.size(); i++) {
        EditDistance editDistance;
        int newDistance = editDistance.lDistance(data[i].getText().c_str(),
                                                 textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
        }
    }

    return minIndex;
}

void Processor2::drawBoxes(Mat &image, const vector<TextualData> &data, const Scalar &color) {
    for (int i = 0; i < data.size(); i++) {
        rectangle(image, data[i].getRect(), color, 3, 8, 0);
    }
}

void Processor2::runProcessorProgram(string parentPath, bool evaluate) {
    if (parentPath[parentPath.length() - 1] != '/')
        parentPath = parentPath + "/";

    cout << "Running on " << parentPath << endl;

    fstream streamImageFilesList(parentPath + "images/files.txt");
    fstream streamJsonFilesList(parentPath + "text/files.txt");
    fstream streamGroundTruthFilesList;

    if (evaluate)
        streamGroundTruthFilesList.open(parentPath + "groundTruth/files.txt");

    string imageFile;
    string jsonFile;
    string groundTruthFile;
    string modelFilePath = parentPath + "model.mdl";

    string outputFolder = parentPath + "output/";

    float accuracySum = 0;
    int num = 0;

    while (getline(streamImageFilesList, imageFile)) {
        getline(streamJsonFilesList, jsonFile);
        groundTruthFile = "";
        if (evaluate)
            getline(streamGroundTruthFilesList, groundTruthFile);

        cout << imageFile << endl;
        if (groundTruthFile.length() == 0 && evaluate) {
            cerr << "Ground truth not found" << endl;
        }

        string workFile = HelperMethods::removeFileExtension(imageFile);

        imageFile = parentPath + "images/" + imageFile;
        jsonFile = parentPath + "text/" + jsonFile;
        if (groundTruthFile.length() != 0)
            groundTruthFile = parentPath + "groundTruth/" + groundTruthFile;


        float x = Processor2(imageFile, jsonFile, groundTruthFile, modelFilePath, outputFolder, workFile).execute();
        accuracySum += x;
        num++;

        if (groundTruthFile.length() != 0)
            cout << "Accuracy is " << x << endl;

//        break;

    }

    if (groundTruthFile.length() != 0)
        cout << "Average accuracy " << accuracySum / num;

}

GroundTruth::GroundTruth(const Rect &rect, const string &value) : rect(rect), value(value) {}
