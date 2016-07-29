//
// Created by shahrukhqasim on 7/28/16.
//

#include "Mapper.h"
#include "Model/ModelBuilder.h"
#include "Processor2.h"


Mapper::Mapper(const vector<TextualData> &data, shared_ptr<Node> node, shared_ptr<Node> ultimateParent) {
    this->data = data;
    this->node = node;
    this->ultimateParent = ultimateParent;
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


string Mapper::findTextWithRules(shared_ptr<Node> node) {

    string text = "";
    for_each(data.begin(), data.end(), [&](const TextualData &currentData) {
        bool ruleMatched = true;
        cout << node->rules.size();
        cout<<currentData.getText()<<" "<<currentData.getRect()<<endl;
        for_each(node->rules.begin(), node->rules.end(), [&](std::function<bool(const TextualData &n)> currentRule) {
            bool b = currentRule(currentData);
            ruleMatched = ruleMatched & b;
            cout << " " << b << endl;
        });
        if (ruleMatched) {
            text += " " + currentData.getText();
            cout << " Rule matched" << endl;
        } else {
            cout << " Not matched" << endl;
        }
    });

    return text;
}


void Mapper::recursiveCallInput(shared_ptr<Node> node) {
//    Rect rectangle;
//    bool regionAssigned;
    if (dynamic_pointer_cast<TextNode>(node) != nullptr) {

    }
    else if (dynamic_pointer_cast<InputNode>(node) != nullptr) {
        cout << "Running on: " << node->id << endl;
        shared_ptr<InputNode> iModel = dynamic_pointer_cast<InputNode>(node);
        convertRulesToFunctions(node);
        string textExtracted = findTextWithRules(node);
        iModel->data = textExtracted;
    }
    else if (dynamic_pointer_cast<TableNode>(node) != nullptr) {
        shared_ptr<TableNode> tModel = dynamic_pointer_cast<TableNode>(node);

        for_each(tModel->tableEntries.begin(), tModel->tableEntries.end(), [&](pair<string, shared_ptr<Node>> kv) {
            shared_ptr<Node> current = kv.second;

            recursiveCallInput(current);
        });
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
        node->region = data[minIndex].getRect();
        tNode->textAssigned = data[minIndex].getText();
        rectangle = node->region;
        taken.push_back(data[minIndex]);
        cout<<"Assigning to: "<<tNode->id<<" "<<data[minIndex].getRect()<<endl;
        data.erase(data.begin() + minIndex);
        regionAssigned = true;
        tNode->regionDefined=true;
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

            Rect newStuff = recursiveCallText(current);
            if (regionAssigned) {
                rectangle = newStuff | rectangle;
            }
            else {
                rectangle = newStuff;
                regionAssigned = true;
            }

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

// TODO: Move this function to ModelBuilder
void Mapper::convertRulesToFunctions(shared_ptr<Node> theNode) {
    for_each(theNode->rulesModel.begin(), theNode->rulesModel.end(), [&](pair<string, unordered_set<string>> thePair) {
        if (thePair.first == "is_below") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelBuilder::findNode(hierarchy, ultimateParent);
                if(second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
                        cout << "Is below: " << second->id;

                        return Processor2::isBelow(third.getRect(), second->region);
                    });
                }
            });

        }
        else if (thePair.first == "is_above") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelBuilder::findNode(hierarchy, ultimateParent);
                if(second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
                        cout << "Is above: " << second->id<<" "<<second->region;
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
                shared_ptr<Node> second = ModelBuilder::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
                        cout << "Is left to: " << second->id;
                        return Processor2::isLeftTo(third.getRect(), second->region);
                    });
                }
            });

        }
        else if (thePair.first == "is_right_to") {
            unordered_set<string> value = thePair.second;
            for_each(value.begin(), value.end(), [&](const string &x) {
                vector<string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                shared_ptr<Node> second = ModelBuilder::findNode(hierarchy, ultimateParent);
                if (second->regionDefined) {
                    theNode->rules.push_back([=](const TextualData &third) -> bool {
                        cout << "Is right to: " << second->id;
                        return Processor2::isRightTo(third.getRect(), second->region);
                    });
                }
            });

        }
    });
}