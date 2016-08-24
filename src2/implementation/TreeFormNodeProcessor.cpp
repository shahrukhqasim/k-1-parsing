//
// Created by shahrukhqasim on 8/22/16.
//

#include "TreeFormNodeProcessor.h"
#include "TreeFormModel.h"
#include "TextTreeFormNode.h"
#include "InputTreeFormNode.h"
#include "RepeatInputTreeFormNode.h"
#include "TableTreeFormNode.h"


// TODO: Move this to correct place
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

bool TreeFormNodeProcessor::process(std::shared_ptr<TreeFormNodeInterface> ptr,
                                    std::shared_ptr<TreeFormModelInterface> model, int iteration,
                                    bool &childrenDone) {
    if (iteration == 0) {
        cv::Rect rectangle;
        if (std::dynamic_pointer_cast<TextTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<TextTreeFormNode> tNode = std::dynamic_pointer_cast<TextTreeFormNode>(ptr);
            std::string text = tNode->getText();

            int minIndex = findTextWithMinimumEditDistance(text);
            if (minIndex != -1) {
                tNode->setRegion(TreeFormNodeProcessor::text[minIndex].getRect());
                tNode->setTextAssigned(TreeFormNodeProcessor::text[minIndex].getText());
                rectangle = tNode->getRegion();
                tNode->setRegionDefined(true);
                takenText.push_back(TreeFormNodeProcessor::text[minIndex]);
                TreeFormNodeProcessor::text.erase(TreeFormNodeProcessor::text.begin() + minIndex);
            }

        }
    } else if (iteration == 1) {
        if (std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<RepeatInputTreeFormNode> rModel = std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr);

//        cout << "Repeat here" << endl;
            std::vector<std::function<bool(const TextualData &n)>> functionalRules;
            convertRulesToFunctions(rModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);
            std::vector<TextualData> croppedTextualData = findVectorizedTextFromFunctionalRules(functionalRules);


            // Sort wrt y co-ordinates
            std::sort(croppedTextualData.begin(), croppedTextualData.end(),
                      [&](const TextualData &d1, const TextualData &d2) -> bool {
                          return (d1.getRect().y < d2.getRect().y);
                      });

            std::unordered_map<TextualData, std::pair<int, int>> dataX;
            std::for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData &d) {
                dataX[d] = std::pair<int, int>(-1, -1);
            });


            int rowValue = 0;
            std::for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
                if (dataX[alpha].second == -1) {
//                cout << "Data " << alpha.getText() << endl;
                    dataX[alpha].second = rowValue;
                    // Both of the coordinates are defined, find all matching columns
                    for_each(dataX.begin(), dataX.end(), [&](std::pair <TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                             cv::Rect(alpha.getRect().y, 0, alpha.getRect().height, 10)).area() != 0) {
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
                    for_each(dataX.begin(), dataX.end(), [&](std::pair <TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                             cv::Rect(alpha.getRect().x, 0, alpha.getRect().width, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                            dataX[beta.first].first = colValue;
                        }
                    });
                    colValue++;
                }
            });

            std::string dx = "";

            for_each(dataX.begin(), dataX.end(), [&](std::pair <TextualData, std::pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
                dx += std::string("(");
                dx += std::to_string(alpha.second.first);
                dx += std::string(",");
                dx += std::to_string(alpha.second.second);
                dx += std::string(")");
                dx += std::string("=>");
                dx += alpha.first.getText() + "\n";


                std::string coordinateString;

                coordinateString += std::string("(");
                coordinateString += std::to_string(alpha.second.first);
                coordinateString += std::string(",");
                coordinateString += std::to_string(alpha.second.second);
                coordinateString += std::string(")");


//            cout<<"Finding "<<coordinateString<<endl;
            });


            std::string dx2 = "";
            cv::Rect regionX;
            bool regionXDef = false;

            std::vector <TextualData> dataVector;

            for_each(dataX.begin(), dataX.end(), [&](std::pair <TextualData, std::pair<int, int>> alpha) {
                dataVector.push_back(alpha.first);
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
                //dx2 += alpha.first.getText() + "|";
                if (!(alpha.first.getRect().x == 0 || alpha.first.getRect().y == 0)) {
                    if (!regionXDef) {
                        regionX = alpha.first.getRect();
                        regionXDef = true;
                    } else
                        regionX = alpha.first.getRect() | regionX;
                }
            });

            sort(dataVector.begin(), dataVector.end(),
                 [](const TextualData &a, const TextualData &b) -> bool {
                     return a.getRect().y < b.getRect().y;
                 });

            int number = 0;
            for_each(dataVector.begin(), dataVector.end(), [&](const TextualData &x) {
                dx2 += x.getText() + "|";
                number++;

                {
                    std::shared_ptr <InputTreeFormNode> newDynamicNode = std::shared_ptr<InputTreeFormNode>(
                            new InputTreeFormNode(InputTreeFormNode::INPUT_ALPHA_NUMERIC));
                    newDynamicNode->setId(rModel->getId() + ":" + std::to_string(number));

                    rModel->addChild(std::to_string(number),newDynamicNode);
                    std::shared_ptr <BasicTreeFormNode> nx = std::dynamic_pointer_cast<BasicTreeFormNode>(rModel->getChild(std::to_string(number)));
                    if (std::dynamic_pointer_cast<InputTreeFormNode>(nx) != nullptr) {
                        std::shared_ptr <InputTreeFormNode> nx2 = std::dynamic_pointer_cast<InputTreeFormNode>(nx);
//                    cout<<"SETTING "<<coordinateString<<" to "<<alpha.first.getText()<<endl;
                        nx2->setData(nx2->getData()+x.getText());
                        nx2->setRegion(x.getRect());
                        nx2->setRegionDefined(true);
                    }
                }
            });

            if (dx2.length() > 0) {
                if (dx2[dx2.length() - 1] == '|') {
                    dx2 = dx2.substr(0, dx2.length() - 1);
                }
            }


//        rModel->region=r;
            rModel->setData(dx2);
            rModel->setRegionDefined(dataVector.size() != 0 ? true : false);
            rModel->setRegion(regionX);



        } else if (std::dynamic_pointer_cast<InputTreeFormNode>(ptr) != nullptr) {
//        cout << "Running on: " << node->id << endl;
            std::shared_ptr<InputTreeFormNode> iModel = std::dynamic_pointer_cast<InputTreeFormNode>(ptr);
            if (iModel->getInputType() == InputTreeFormNode::INPUT_ALPHA_NUMERIC ||
                iModel->getInputType() == InputTreeFormNode::INPUT_NUMERIC) {
                std::vector<std::function<bool(const TextualData &n)>> functionalRules;
                convertRulesToFunctions(iModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);
                std::pair<std::string, cv::Rect> oup = findTextFromFunctionalRules(functionalRules);
                std::string textExtracted = oup.first;
                iModel->setData(textExtracted);
                iModel->setRegion(oup.second);
                iModel->setRegionDefined(oup.first.size() == 0 ? false : true);
//            cout << iModel->id << ": " << iModel->data << endl;
            }
        }
        else if (std::dynamic_pointer_cast<TableTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<TableTreeFormNode> tModel = std::dynamic_pointer_cast<TableTreeFormNode>(ptr);

            int left = -1;
            int top = -1;
            int right = -1;
            int bottom = -1;


            std::for_each(tModel->rulesModel.begin(), tModel->rulesModel.end(), [&](std::pair<std::string, std::unordered_set<std::string>> alpha) {
                if (alpha.first == "is_below") {
                    std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(TreeFormModel::getNode(root,HelperMethods::regexSplit(*(alpha.second.begin()), "[:]")));
                    if (theNode->isRegionDefined()) {
                        if (top == -1 || (top < theNode->getRegion().y + theNode->getRegion().height && top != -1))
                            top = theNode->getRegion().y + theNode->getRegion().height;
                    }
//                cout << "::" << theNode->id << endl;
//                cout << theNode->region << endl;
//                cout << (dynamic_pointer_cast<TextNode>(theNode))->textAssigned << endl;
                } else if (alpha.first == "is_above") {
                    std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(TreeFormModel::getNode(root,HelperMethods::regexSplit(*(alpha.second.begin()), "[:]")));
                    if (theNode->isRegionDefined()) {
                        if (bottom == -1 || (bottom > theNode->getRegion().y && top != -1))
                            bottom = theNode->getRegion().y;
                    }
                } else if (alpha.first == "is_right_to") {
                    std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(TreeFormModel::getNode(root,HelperMethods::regexSplit(*(alpha.second.begin()), "[:]")));
                    if (theNode->isRegionDefined()) {
                        if (left == -1 || left < (theNode->getRegion().x + theNode->getRegion().width && top != -1))
                            left = theNode->getRegion().x + theNode->getRegion().width;
                    }
                } else if (alpha.first == "is_left_to") {
                    std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(TreeFormModel::getNode(root,HelperMethods::regexSplit(*(alpha.second.begin()), "[:]")));
                    if (theNode->isRegionDefined()) {
                        if (right == -1 || (right > theNode->getRegion().x && top != -1))
                            right = theNode->getRegion().x;
                    }
                }
            });
            if (left == -1)
                left = 0;
            if (top == -1)
                top = 0;
            if (right == -1)
                right = image.cols - 1;
            if (bottom == -1)
                bottom = image.rows - 1;

//        cout << left << " " << top << " " << right << " " << bottom << endl;
//        cout << width << " " << height << endl;
            cv::Rect r(left, top, right - left, bottom - top);

//        cout << "R=" << r << endl;

            std::vector<TextualData> croppedTextualData;
            std::for_each(text.begin(), text.end(), [&](TextualData &d) {
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

            std::unordered_map<TextualData, std::pair<int, int>> dataX;
            for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData &d) {
                dataX[d] = std::pair<int, int>(-1, -1);
            });


            int rowValue = 0;
            for_each(croppedTextualData.begin(), croppedTextualData.end(), [&](TextualData alpha) {
                if (dataX[alpha].second == -1) {
//                cout << "Data " << alpha.getText() << endl;
                    dataX[alpha].second = rowValue;
                    // Both of the coordinates are defined, find all matching columns
                    std::for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                                cv::Rect(alpha.getRect().y, 0, alpha.getRect().height, 10)).area() != 0) {
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
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                                cv::Rect(alpha.getRect().x, 0, alpha.getRect().width, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                            dataX[beta.first].first = colValue;
                        }
                    });
                    colValue++;
                }
            });

            std::for_each(tModel->getStartIterator(), tModel->getEndIterator(), [&](std::pair<std::string, std::shared_ptr<TreeFormNodeInterface>> alpha) {
                if (std::dynamic_pointer_cast<TextTreeFormNode>(alpha.second) != nullptr) {
                    std::vector<std::string> splits = HelperMethods::regexSplit(alpha.first.substr(1, alpha.first.length() - 2), ",");
                    int x = std::stoi(splits[0]);
                    int y = std::stoi(splits[1]);

                    int pos = findTextWithMinimumEditDistance(croppedTextualData, std::dynamic_pointer_cast<TextTreeFormNode>(alpha.second)->getText());
                    if (pos != -1) {
                        TextualData ddd = croppedTextualData[pos];

                        dataX[ddd].first = x;
                        dataX[ddd].second = y;
                    }
                }
            });

            for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
                if (alpha.second.first != -1 && alpha.second.second != -1) {
                    // Both of the coordinates are defined, find all matching columns
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                                cv::Rect(alpha.first.getRect().x, 0, alpha.first.getRect().width, 10)).area() != 0)
                            dataX[beta.first].first = alpha.second.first;
                    });
                }
            });

            for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
            });


            std::string dx = "";

            std::for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
                std::string coordinateString;

                coordinateString += std::string("(");
                coordinateString += std::to_string(alpha.second.first);
                coordinateString += std::string(",");
                coordinateString += std::to_string(alpha.second.second);
                coordinateString += std::string(")");

                dx += std::string("(");
                dx += std::to_string(alpha.second.first);
                dx += std::string(",");
                dx += std::to_string(alpha.second.second);
                dx += std::string(")");
                dx += std::string("=>");
                dx += alpha.first.getText() + "\n";

//            cout<<"Finding "<<coordinateString<<endl;
                if (tModel->getChild(coordinateString) != nullptr) {
                    std::shared_ptr<TreeFormNodeInterface> nx = tModel->getChild(coordinateString);
                    if (std::dynamic_pointer_cast<InputTreeFormNode>(nx) != nullptr) {
                        std::shared_ptr<InputTreeFormNode> nx2 = std::dynamic_pointer_cast<InputTreeFormNode>(nx);
//                    cout<<"SETTING "<<coordinateString<<" to "<<alpha.first.getText()<<endl;
                        nx2->setData(nx2->getData()+ alpha.first.getText());
                        nx2->setRegion(alpha.first.getRect());
                        nx2->setRegionDefined(true);
                    }
                }

            });


//            tModel->data = dx;

        }
    }
}

void TreeFormNodeProcessor::setForm(const std::shared_ptr<RawFormInterface> &form) {
    TreeFormNodeProcessor::form = form;
    form->getText(text);
    form->getRasterImage(image);
}

int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::string textToFind) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    for (int i = 0; i < text.size(); i++) {
        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;

        for (int i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
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

int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData>data,std::string textToFind) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    for (int i = 0; i < text.size(); i++) {
        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;

        for (int i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
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

// TODO: Fix root place
void TreeFormNodeProcessor::convertRulesToFunctions(std::shared_ptr<BasicTreeFormNode> theNode,
                                                    std::shared_ptr<TreeFormModel> model,
                                                    std::shared_ptr<BasicTreeFormNode> root,
                                                    std::vector<std::function<bool(
                                                            const TextualData &n)>> &functionalRules) {

    std::for_each(theNode->rulesModel.begin(), theNode->rulesModel.end(),
                  [&](std::pair<std::string, std::unordered_set<std::string>> thePair) {
                      if (thePair.first == "is_below") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      if (a.y > b.y + b.height) {
                                          return true;
                                      }
                                      return false;
                                  });
                              }
                          });

                      } else if (thePair.first == "is_above") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      if (a.y + a.height < b.y) {
                                          return true;
                                      }
                                      return false;
                                  });
                              }
                          });

                      } else if (thePair.first == "is_left_to") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      if (a.x + a.width < b.x) {
                                          return true;
                                      }
                                      return false;
                                  });
                              }
                          });

                      }
                      if (thePair.first == "is_right_to") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      if (a.x > b.x + b.width) {
                                          return true;
                                      }
                                      return false;
                                  });
                              }
                          });

                      }
                      if (thePair.first == "has_vertical_overlap_with") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      return std::max(0, std::min(a.x + a.width, b.x + b.width) - std::max(a.x, b.x)) >
                                             0;
                                  });
                              }
                          });

                      } else if (thePair.first == "has_horizontal_overlap_with") {
                          std::unordered_set<std::string> value = thePair.second;
                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                      model->getNode(root, hierarchy));
                              if (second->isRegionDefined()) {
                                  functionalRules.push_back([=](const TextualData &third) -> bool {
                                      cv::Rect a = third.getRect();
                                      cv::Rect b = second->getRegion();
                                      return std::max(0,
                                                      std::min(a.y + a.height, b.y + b.height) - std::max(a.y, b.y)) >
                                             0;
                                  });
                              }
                          });

                      }
                  });
}

std::pair<std::string, cv::Rect> TreeFormNodeProcessor::findTextFromFunctionalRules(
        std::vector<std::function<bool(const TextualData &n)>> functionalRules) {
    std::string text = "";
    cv::Rect regionFound;
    bool regionDefined = false;
    std::for_each(TreeFormNodeProcessor::text.begin(), TreeFormNodeProcessor::text.end(),
                  [&](const TextualData &currentData) {
                      bool ruleMatched = true;
                      for_each(functionalRules.begin(), functionalRules.end(),
                               [&](std::function<bool(const TextualData &n)> currentRule) {
                                   bool b = currentRule(currentData);
                                   ruleMatched = ruleMatched & b;

                               });
                      if (ruleMatched) {
                          text += " " + currentData.getText();

                          if (!regionDefined) {
                              regionDefined = true;
                              regionFound = currentData.getRect();
                          } else {
                              regionFound = regionFound | currentData.getRect();
                          }
                      } else {
                      }
                  });

    return std::pair<std::string, cv::Rect>(text, regionFound);
}

std::vector<TextualData> TreeFormNodeProcessor::findVectorizedTextFromFunctionalRules(
        std::vector<std::function<bool(const TextualData &n)>> functionalRules) {
    std::vector<TextualData> result;
    std::for_each(TreeFormNodeProcessor::text.begin(), TreeFormNodeProcessor::text.end(),
                  [&](const TextualData &currentData) {
                      bool ruleMatched = true;
                      for_each(functionalRules.begin(), functionalRules.end(),
                               [&](std::function<bool(const TextualData &n)> currentRule) {
                                   bool b = currentRule(currentData);
                                   ruleMatched = ruleMatched & b;

                               });
                      if (ruleMatched) {
                          result.push_back(currentData);
                      } else {
                      }
                  });

    return result;
}
