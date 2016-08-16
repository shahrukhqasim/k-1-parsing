#include "Mapper.h"
#include "Model/ModelParser.h"
#include "Processor2.h"
#include "Model/RepeatInputNode.h"
#include "Model/MappingJob.h"

using namespace std;
using namespace cv;

namespace std {
    template<>
    struct hash<TextualData> {
        inline size_t operator()(const TextualData &x) const {
            size_t value = hash<string>()(x.getText());
            value += x.getRect().x + x.getRect().y + x.getRect().width + x.getRect().height;

            return value;
        }
    };
}

Mapper::Mapper(const vector<TextualData> &data, shared_ptr<Node> node, shared_ptr<Node> ultimateParent, int width,
               int height) {
    this->data = data;
    this->node = node;
    this->ultimateParent = ultimateParent;
    this->width = width;
    this->height = height;
}

void Mapper::executeTextFields() {
    recursiveCallText(node);
}

void Mapper::executeInputFields() {
    recursiveCallInput(node);
}

int Mapper::findMinTextIndex(const vector<TextualData> &data, const string &textToFind) {
    int minDistance = 99999999;
    int minIndex = -1;
    for (int i = 0; i < data.size(); i++) {
        string dataCurrent=data[i].getText();
        string dataCurrent2;

        for(int i=0;i<dataCurrent.size();i++) {
            if(HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2+=dataCurrent[i];
        }

        EditDistance editDistance;
        int newDistance = editDistance.lDistance(dataCurrent2.c_str(),
                                                 textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
        }
    }

    return minIndex;
}


pair<string, Rect> Mapper::findTextWithRules(shared_ptr<Node> node) {

    string text = "";
    Rect regionFound;
    bool regionDefined = false;
    for_each(data.begin(), data.end(), [&](const TextualData &currentData) {
        bool ruleMatched = true;
//        cout << node->rules.size();
//        cout<<currentData.getText()<<" "<<currentData.getRect()<<endl;
        for_each(node->rules.begin(), node->rules.end(), [&](std::function<bool(const TextualData &n)> currentRule) {
            bool b = currentRule(currentData);
            ruleMatched = ruleMatched & b;

//            cout << " " << b << endl;
        });
        if (ruleMatched) {
            text += " " + currentData.getText();

            if (!regionDefined) {
                regionDefined = true;
                regionFound = currentData.getRect();
            }
            else {
                regionFound = regionFound | currentData.getRect();
            }
//            cout << " Rule matched" << endl;
        } else {
//            cout << " Not matched" << endl;
        }
    });

    return pair<string, Rect>(text, regionFound);
}


void Mapper::recursiveCallInput(shared_ptr<Node> node) {
//    Rect rectangle;
//    bool regionAssigned;
    if (dynamic_pointer_cast<TextNode>(node) != nullptr) {

    }
    else if (dynamic_pointer_cast<RepeatInputNode>(node) != nullptr) {
        shared_ptr<RepeatInputNode> rModel = dynamic_pointer_cast<RepeatInputNode>(node);

//        cout << "Repeat here" << endl;

        Rect r=MappingJob(ultimateParent,node->id,width,height).map();


        vector<TextualData> croppedTextualData;
        for_each(data.begin(), data.end(), [&](TextualData &d) {
            if ((r & d.getRect()).area() == d.getRect().area()) {
//                cout << d.getText() << endl;
                croppedTextualData.push_back(d);
            }
        });

        // Sort wrt y co-ordinates
        sort(croppedTextualData.begin(), croppedTextualData.end(),
             [&](const TextualData &d1, const TextualData &d2) -> bool {
                 return (d1.getRect().y < d2.getRect().y);
             });

        unordered_map<TextualData, pair<int, int>> dataX;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData &d) {
            dataX[d] = pair<int, int>(-1, -1);
        });


        int rowValue = 0;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
            if (dataX[alpha].second == -1) {
//                cout << "Data " << alpha.getText() << endl;
                dataX[alpha].second = rowValue;
                // Both of the coordinates are defined, find all matching columns
                for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> beta) {
                    if ((Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                         Rect(alpha.getRect().y, 0, alpha.getRect().height, 10)).area() != 0) {
                        dataX[beta.first].second = rowValue;
                    }
                });
                rowValue++;
            }
        });

        // Sort wrt x co-ordinates
        sort(croppedTextualData.begin(), croppedTextualData.end(),
             [&](const TextualData &d1, const TextualData &d2) -> bool {
                 return (d1.getRect().x < d2.getRect().x);
             });

        int colValue = 0;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
            if (dataX[alpha].first == -1) {
                dataX[alpha].first = colValue;
                // Both of the coordinates are defined, find all matching columns
                for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> beta) {
                    if ((Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                         Rect(alpha.getRect().x, 0, alpha.getRect().width, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                        dataX[beta.first].first = colValue;
                    }
                });
                colValue++;
            }
        });

        string dx = "";

        for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
            dx += string("(");
            dx += to_string(alpha.second.first);
            dx += string(",");
            dx += to_string(alpha.second.second);
            dx += string(")");
            dx += string("=>");
            dx += alpha.first.getText() + "\n";
        });



        string dx2 = "";
        Rect regionX;
        bool regionXDef=false;

        vector<TextualData>dataVector;

        for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> alpha) {
            dataVector.push_back(alpha.first);
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
            //dx2 += alpha.first.getText() + "|";
            if(!(alpha.first.getRect().x==0||alpha.first.getRect().y==0)) {
                if (!regionXDef) {
                    regionX = alpha.first.getRect();
                    regionXDef = true;
                }
                else
                    regionX = alpha.first.getRect() | regionX;
            }
        });

        sort(dataVector.begin(), dataVector.end(),
             [](const TextualData & a, const TextualData & b) -> bool
             {
                 return a.getRect().y < b.getRect().y;
             });

        for_each(dataVector.begin(),dataVector.end(),[&] (const TextualData &x) {
            dx2 += x.getText() + "|";
        });

        if (dx2.length() > 0) {
            if (dx2[dx2.length() - 1] == '|') {
                dx2 = dx2.substr(0, dx2.length() - 1);
            }
        }


//        rModel->region=r;
        rModel->dataCoordinates = dx;
        rModel->data = dx2;
        rModel->regionDefined=dataVector.size()!=0?true:false;
        rModel->region=regionX;


    }
    else if (dynamic_pointer_cast<InputNode>(node) != nullptr) {
//        cout << "Running on: " << node->id << endl;
        shared_ptr<InputNode> iModel = dynamic_pointer_cast<InputNode>(node);
        if (iModel->inputType == InputNode::INPUT_ALPHA_NUMERIC || iModel->inputType == InputNode::INPUT_NUMERIC) {
            convertRulesToFunctions(node);
            pair<string, Rect> oup = findTextWithRules(node);
            string textExtracted = oup.first;
            iModel->data = textExtracted;
            iModel->region = oup.second;
            iModel->regionDefined = oup.first.size()==0?false:true;
//            cout << iModel->id << ": " << iModel->data << endl;
        }
    }
    else if (dynamic_pointer_cast<TableNode>(node) != nullptr) {
        shared_ptr<TableNode> tModel = dynamic_pointer_cast<TableNode>(node);

        int left = -1;
        int top = -1;
        int right = -1;
        int bottom = -1;


        for_each(tModel->rulesModel.begin(), tModel->rulesModel.end(), [&](pair<string, unordered_set<string>> alpha) {
            if (alpha.first == "is_below") {
                shared_ptr<Node> theNode = ModelParser::findNode(
                        HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                if (theNode->regionDefined) {
                    if (top == -1 || (top < theNode->region.y + theNode->region.height && top != -1))
                        top = theNode->region.y + theNode->region.height;
                }
//                cout << "::" << theNode->id << endl;
//                cout << theNode->region << endl;
//                cout << (dynamic_pointer_cast<TextNode>(theNode))->textAssigned << endl;
            }

            else if (alpha.first == "is_above") {
                shared_ptr<Node> theNode = ModelParser::findNode(
                        HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                if (theNode->regionDefined) {
                    if (bottom == -1 || (bottom > theNode->region.y && top != -1))
                        bottom = theNode->region.y;
                }
            }

            else if (alpha.first == "is_right_to") {
                shared_ptr<Node> theNode = ModelParser::findNode(
                        HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                if (theNode->regionDefined) {
                    if (left == -1 || left < (theNode->region.x + theNode->region.width && top != -1))
                        left = theNode->region.x + theNode->region.width;
                }
            }

            else if (alpha.first == "is_left_to") {
                shared_ptr<Node> theNode = ModelParser::findNode(
                        HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
                if (theNode->regionDefined) {
                    if (right == -1 || (right > theNode->region.x && top != -1))
                        right = theNode->region.x;
                }
            }
        });
        if (left == -1)
            left = 0;
        if (top == -1)
            top = 0;
        if (right == -1)
            right = width - 1;
        if (bottom == -1)
            bottom = height - 1;

//        cout << left << " " << top << " " << right << " " << bottom << endl;
//        cout << width << " " << height << endl;
        Rect r(left, top, right - left, bottom - top);

//        cout << "R=" << r << endl;

        vector<TextualData> croppedTextualData;
        for_each(data.begin(), data.end(), [&](TextualData &d) {
            if ((r & d.getRect()).area() != 0) {
//                cout << d.getText() << endl;
                croppedTextualData.push_back(d);
            }
        });

        // Sort wrt y co-ordinates
        sort(croppedTextualData.begin(), croppedTextualData.end(),
             [&](const TextualData &d1, const TextualData &d2) -> bool {
                 return (d1.getRect().y < d2.getRect().y);
             });

        unordered_map<TextualData, pair<int, int>> dataX;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData &d) {
            dataX[d] = pair<int, int>(-1, -1);
        });


        int rowValue = 0;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
            if (dataX[alpha].second == -1) {
//                cout << "Data " << alpha.getText() << endl;
                dataX[alpha].second = rowValue;
                // Both of the coordinates are defined, find all matching columns
                for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> beta) {
                    if ((Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                         Rect(alpha.getRect().y, 0, alpha.getRect().height, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                        dataX[beta.first].second = rowValue;
                    }
                });
                rowValue++;
            }
        });

        // Sort wrt x co-ordinates
        sort(croppedTextualData.begin(), croppedTextualData.end(),
             [&](const TextualData &d1, const TextualData &d2) -> bool {
                 return (d1.getRect().x < d2.getRect().x);
             });

        int colValue = 0;
        for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
            if (dataX[alpha].first == -1) {
//                cout << "Data " << alpha.getText() << endl;
                dataX[alpha].first = colValue;
                // Both of the coordinates are defined, find all matching columns
                for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> beta) {
                    if ((Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                         Rect(alpha.getRect().x, 0, alpha.getRect().width, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                        dataX[beta.first].first = colValue;
                    }
                });
                colValue++;
            }
        });

        for_each(tModel->tableEntries.begin(), tModel->tableEntries.end(), [&](pair<string, shared_ptr<Node>> alpha) {
            if (dynamic_pointer_cast<TextNode>(alpha.second) != nullptr) {
                vector<string> splits = HelperMethods::regexSplit(alpha.first.substr(1, alpha.first.length() - 2), ",");
                int x = stoi(splits[0]);
                int y = stoi(splits[1]);

                int pos = findMinTextIndex(croppedTextualData, dynamic_pointer_cast<TextNode>(alpha.second)->text);
                if (pos != -1) {
                    TextualData ddd = croppedTextualData[pos];

                    dataX[ddd].first = x;
                    dataX[ddd].second = y;
                }
            }
        });

        for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> alpha) {
            if (alpha.second.first != -1 && alpha.second.second != -1) {
                // Both of the coordinates are defined, find all matching columns
                for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> beta) {
                    if ((Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                         Rect(alpha.first.getRect().x, 0, alpha.first.getRect().width, 10)).area() != 0)
                        dataX[beta.first].first = alpha.second.first;
                });
            }
        });

        for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
        });


        string dx = "";

        for_each(dataX.begin(), dataX.end(), [&](pair<TextualData, pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
            string coordinateString;

            coordinateString += string("(");
            coordinateString += to_string(alpha.second.first);
            coordinateString += string(",");
            coordinateString += to_string(alpha.second.second);
            coordinateString += string(")");

            dx += string("(");
            dx += to_string(alpha.second.first);
            dx += string(",");
            dx += to_string(alpha.second.second);
            dx += string(")");
            dx += string("=>");
            dx += alpha.first.getText() + "\n";

//            cout<<"Finding "<<coordinateString<<endl;
            if(tModel->tableEntries.find(coordinateString)!=tModel->tableEntries.end()) {
                shared_ptr<Node>nx=tModel->tableEntries[coordinateString];
                if(dynamic_pointer_cast<InputNode>(nx)!= nullptr) {
                    shared_ptr<InputNode>nx2=dynamic_pointer_cast<InputNode>(nx);
//                    cout<<"SETTING "<<coordinateString<<" to "<<alpha.first.getText()<<endl;
                    nx2->data+=alpha.first.getText();
                    nx2->region=alpha.first.getRect();
                    nx2->regionDefined=true;
                }
            }

        });




        tModel->data = dx;

    }

    if (node->subNodes.size() != 0) {
        for_each(node->subNodes.begin(), node->subNodes.end(), [&](pair<string, shared_ptr<Node>> current) {
            recursiveCallInput(current.second);
        });
    }
//
//    node->region=rectangle;
//
//    // Verify with rules over here
//
//    return rectangle;
}

Rect Mapper::recursiveCallText(shared_ptr<Node> node) {
    Rect rectangle;
    bool regionAssigned;
    if (dynamic_pointer_cast<TextNode>(node) != nullptr) {
        shared_ptr<TextNode> tNode = dynamic_pointer_cast<TextNode>(node);
        string text = tNode->text;
        int minIndex = findMinTextIndex(data, text);
        if (minIndex != -1) {
            node->region = data[minIndex].getRect();
            tNode->textAssigned = data[minIndex].getText();
            rectangle = node->region;
            taken.push_back(data[minIndex]);
//        cout<<"Assigning to: "<<tNode->id<<" "<<data[minIndex].getRect()<<endl;
            data.erase(data.begin() + minIndex);
            regionAssigned = true;
            tNode->regionDefined = true;
        }
    }
    else if (dynamic_pointer_cast<InputNode>(node) != nullptr) {
//        shared_ptr<InputNode> iModel=dynamic_pointer_cast<InputNode>(node);
//        string type="";
//        switch(iModel->inputType) {
//            case InputNode::INPUT_ALPHA_NUMERIC:
//                type="alpha_numeric";
//                break;
//            case InputNode::INPUT_NUMERIC:
//                type="numeric";
//                break;
//            case InputNode::INPUT_CHECKBOX:
//                type="boolean";
//                break;
//        }
    }
    else if (dynamic_pointer_cast<TableNode>(node) != nullptr) {
        shared_ptr<TableNode> tModel = dynamic_pointer_cast<TableNode>(node);

        for_each(tModel->tableEntries.begin(), tModel->tableEntries.end(), [&](pair<string, shared_ptr<Node>> kv) {
            shared_ptr<Node> current = kv.second;

//            Rect newStuff = recursiveCallText(current);
//            if (regionAssigned) {
//                rectangle = newStuff | rectangle;
//            }
//            else {
//                rectangle = newStuff;
//                regionAssigned = true;
//            }

        });
    }


    if (node->subNodes.size() != 0) {
        Json::Value subNodes;
        for_each(node->subNodes.begin(), node->subNodes.end(), [&](pair<string, shared_ptr<Node>> current) {
            Rect newStuff = recursiveCallText(current.second);
            if (regionAssigned) {
                rectangle = newStuff | rectangle;
            }
            else {
                rectangle = newStuff;
                regionAssigned = true;
            }
        });
    }

//    node->region = rectangle;

    // Verify with rules over here

    return rectangle;
}

// TODO: Move this function to ModelParser
void Mapper::convertRulesToFunctions(shared_ptr<Node> theNode) {
    for_each(theNode->rulesModel.begin(), theNode->rulesModel.end(), [&](pair<string, unordered_set<string>> thePair) {
        if (thePair.first == "is_below") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelParser::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
//                        cout << "Is below: " << second->id;

                        return Processor2::isBelow(third.getRect(), second->region);
                    });
                }
            });

        }
        else if (thePair.first == "is_above") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelParser::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
//                        cout << "Is above: " << second->id<<" "<<second->region;
//                    cout<<x<<endl;
                        return Processor2::isAbove(third.getRect(), second->region);
                    });
                }
            });

        }
        else if (thePair.first == "is_left_to") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelParser::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
//                        cout << "Is left to: " << second->id;
                        return Processor2::isLeftTo(third.getRect(), second->region);
                    });
                }
            });

        }
        else if (thePair.first == "is_right_to") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelParser::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
//                        cout << "Is right to: " << second->id;
                        return Processor2::isRightTo(third.getRect(), second->region);
                    });
                }
            });

        }
    });
}

Rect Mapper::findRectFromRules(shared_ptr<Node> rModel, shared_ptr<Node> ultimateParent, int width, int height) {

    int left, top, bottom, right;


    left = 0;
    top = 0;
    right = width - 1;
    bottom = height - 1;

    for_each(rModel->rulesModel.begin(), rModel->rulesModel.end(), [&](pair<string, unordered_set<string>> alpha) {
        if (alpha.first == "is_below") {
            shared_ptr<Node> theNode = ModelParser::findNode(
                    HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
            if (theNode->regionDefined) {
                if (top == -1 || (top < theNode->region.y + theNode->region.height && top != -1))
                    top = theNode->region.y + theNode->region.height;
            }
        }

        else if (alpha.first == "is_above") {
            shared_ptr<Node> theNode = ModelParser::findNode(
                    HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
            if (theNode->regionDefined) {
                if (bottom == -1 || (bottom > theNode->region.y && top != -1))
                    bottom = theNode->region.y;
            }
        }

        else if (alpha.first == "is_right_to") {
            shared_ptr<Node> theNode = ModelParser::findNode(
                    HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
            if (theNode->regionDefined) {
                if (left == -1 || left < (theNode->region.x + theNode->region.width && top != -1))
                    left = theNode->region.x + theNode->region.width;
            }
        }

        else if (alpha.first == "is_left_to") {
            shared_ptr<Node> theNode = ModelParser::findNode(
                    HelperMethods::regexSplit(*(alpha.second.begin()), "[:]"), ultimateParent);
            if (theNode->regionDefined) {
                if (right == -1 || (right > theNode->region.x && top != -1))
                    right = theNode->region.x;
            }
        }
    });

    return Rect(left,top,right-left,bottom-top);

}
