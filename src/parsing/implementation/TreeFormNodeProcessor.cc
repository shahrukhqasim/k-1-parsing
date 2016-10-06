
#include "TreeFormNodeProcessor.h"
#include "TreeFormModel.h"
#include "TextTreeFormNode.h"
#include "InputTreeFormNode.h"
#include "RepeatInputTreeFormNode.h"
#include "TableTreeFormNode.h"
#include "DivisionRuleWithReference.h"
#include <regex>

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
    childrenDone = false;
    // Pre-processing iteration
    if (iteration == 0) {
        numNotFoundTextNodes=0;

        mergeWordBoxes(unmergedText, text);

        CDetectCheckBoxes checkboxesDetector;

        cv::Mat imageGrey;
        cv::cvtColor(image, imageGrey, CV_BGR2GRAY);
        checkboxesDetector.detectCheckBoxes(imageGrey, checkboxes);

        childrenDone = true;

        auto drawImage = getIterationOutputImage("checkboxes");

        for (auto i:checkboxes) {
            if (i.validOuterBBox) {
                cv::rectangle(*drawImage, i.outerBBox, cv::Scalar(0, 255, 0), 3, 8, 0);
            } else if (i.validInnerBBox) {
//                cv::rectangle(*drawImage, i.innerBBox, cv::Scalar(0, 255, 0), 3, 8, 0);
            }
        }

        return true;
    }
        // Text division iteration
    else if (iteration == 1) {

        std::shared_ptr<cv::Mat> drawImage = getIterationOutputImage("division");
        for (size_t i = 0; i < text.size(); i++) {
            cv::rectangle(*drawImage, text[i].getRect(), cv::Scalar(255, 0, 0), 1, 8, 0);
        }

        std::shared_ptr<BasicTreeFormNode> bNode = std::dynamic_pointer_cast<BasicTreeFormNode>(ptr);
        if (bNode->divisionRules.size() != 0) {
            bool isLeft = false;
            bool isRight = false;

            for (auto i:bNode->divisionRules) {
                std::shared_ptr<DivisionRule> r = i;
                if (i->getRuleKeyword() == "is_left_half") {
                    isLeft = true;
                } else if (i->getRuleKeyword() == "is_right_half") {
                    isRight = true;
                }
            }

            if (isLeft && isRight) {
                std::cerr << "Division rule cannot be both left and right";
                return false;
            }

            std::vector<TextualData> textB = text;

            int left = 0;
            int top = 0;
            int right = image.cols;
            int bottom = image.rows;


            if (isLeft || isRight) {
                cv::Mat vProjection(1, image.cols, CV_16U);
                vProjection = 0;
                for (size_t i = 0; i < text.size(); i++) {
                    cv::Rect r = text[i].getRect();
                    for (int j = r.x; j < r.x + r.width; j++) {
                        if(j>=0&&j<image.cols) {
                            vProjection.at<short>(0, j) += r.height;
                        }
                    }
                }


                std::shared_ptr<cv::Mat> drawImage = getIterationOutputImage("division");
                for (int i = 0; i < vProjection.cols; i++) {
//                    line(*drawImage, cv::Point(i, 0), cv::Point(i, vProjection.at<short>(0, i)), cv::Scalar(255, 0, 0));
                }


                int quarter = vProjection.cols / 8;

                int shallowProjectionIndex = quarter;
                for (int i = quarter * 3; i < quarter * 5; i++) {
                    if (vProjection.at<short>(0, i) < vProjection.at<short>(0, shallowProjectionIndex))
                        shallowProjectionIndex = i;
                }

                std::vector<TextualData> leftBoxes;
                std::vector<TextualData> rightBoxes;

                for (auto box:text) {
                    if (shallowProjectionIndex - box.getRect().x > box.getRect().width / 2) {
                        leftBoxes.push_back(box);
                    } else {
                        rightBoxes.push_back(box);
                    }
                }

                if (isLeft) {
                    right = shallowProjectionIndex;
                    textB = leftBoxes;
                } else if (isRight) {
                    left = shallowProjectionIndex;
                    textB = rightBoxes;
                }
            }

            for (auto i:bNode->divisionRules) {
                if (std::dynamic_pointer_cast<DivisionRuleWithReference>(i) != nullptr) {
                    std::shared_ptr<DivisionRuleWithReference> castedRule = std::dynamic_pointer_cast<DivisionRuleWithReference>(
                            i);

                    std::shared_ptr<TreeFormNodeInterface> nodeF = TreeFormModel::getNode(root,
                                                                                          HelperMethods::regexSplit(
                                                                                                  castedRule->getOtherNodeId(),
                                                                                                  ":"));

                    std::shared_ptr<TextTreeFormNode> node = std::dynamic_pointer_cast<TextTreeFormNode>(nodeF);
                    std::string referenceText = node->getText();

                    int index = findTextWithMinimumEditDistance(textB, referenceText);
                    TextualData foundText;
                    if (index >= 0) {
                        foundText = textB[index];
                    } else {
                        std::cerr << "Error finding min edit distance text" << std::endl;
                        return false;
                    }

                    if (castedRule->getRuleKeyword() == "is_above") {
                        if (bottom > foundText.getRect().y)
                            bottom = foundText.getRect().y;
                    } else if (castedRule->getRuleKeyword() == "is_below") {
                        if (top < foundText.getRect().y + foundText.getRect().height)
                            top = foundText.getRect().y + foundText.getRect().height;
                    } else if (castedRule->getRuleKeyword() == "is_left_to") {
                        if (right > foundText.getRect().x)
                            right = foundText.getRect().x;
                    } else if (castedRule->getRuleKeyword() == "is_right_to") {
                        if (left < foundText.getRect().x + foundText.getRect().width)
                            left = foundText.getRect().x + foundText.getRect().width;
                    } else if (castedRule->getRuleKeyword() == "is_above_inclusive") {
                        if (bottom > foundText.getRect().y + foundText.getRect().height)
                            bottom = foundText.getRect().y + foundText.getRect().height;
                    } else if (castedRule->getRuleKeyword() == "is_below_inclusive") {
                        if (top < foundText.getRect().y)
                            top = foundText.getRect().y;
                    } else {
                        std::cerr << "Division rule not identified" << std::endl;
                        return false;
                    }
                }
            }

            textDividedRegion[bNode->getId()] = cv::Rect(left, top, right - left, bottom - top);
            std::shared_ptr<cv::Mat> drawImage = getIterationOutputImage("division");

            cv::Scalar color(0, 0, 255);
            if (colorBucket.size() != 0) {
                color = colorBucket[0];
                colorBucket.erase(colorBucket.begin());
            }


            cv::rectangle(*drawImage, cv::Rect(left, top, right - left, bottom - top), color, 3, 8, 0);

        }
    }
        // Minimum text iteration
    else if (iteration == 2) {
        bool divisionRegionIdentified = false;
        cv::Rect dividedTextRegion;

        for (auto i:textDividedRegion) {
            if (ptr->getId().find(i.first) != std::string::npos) {
                dividedTextRegion = i.second;
                divisionRegionIdentified = true;
            }
        }

        cv::Rect rectangle;
        if (std::dynamic_pointer_cast<TextTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<TextTreeFormNode> tNode = std::dynamic_pointer_cast<TextTreeFormNode>(ptr);
            std::string text = tNode->getText();

            int minIndex;
            int numMatches;
            if (divisionRegionIdentified) {
                minIndex = findTextWithMinimumEditDistance(TreeFormNodeProcessor::text, text, dividedTextRegion,
                                                           numMatches);
            } else {
                minIndex = findTextWithMinimumEditDistance(TreeFormNodeProcessor::text, text, numMatches);
            }


            if (minIndex != -1) {
                tNode->setRegion(TreeFormNodeProcessor::text[minIndex].getRect());
                tNode->setTextAssigned(TreeFormNodeProcessor::text[minIndex].getText());
                rectangle = tNode->getRegion();
//                std::cout << tNode->getText() << " - " << tNode->getTextAssigned() << std::endl;
                tNode->setRegionDefined(true);

                if (numMatches == 1) {
                    takenText.push_back(TreeFormNodeProcessor::text[minIndex]);
                    TreeFormNodeProcessor::text.erase(TreeFormNodeProcessor::text.begin() + minIndex);
                } else {
                    std::vector<TextualData> possibleMatches;
                    float min=findTextWithMinimumEditDistanceMulti(TreeFormNodeProcessor::text, text, divisionRegionIdentified?dividedTextRegion:cv::Rect(0,0,image.cols,image.rows),
                                                               possibleMatches);
                    if(min/text.size()<0.5) {
                        problemNodes[tNode] = possibleMatches;

//                        std::cout << "Possible matches are: " << std::endl;
//                        for (auto i:possibleMatches) {
//                            std::cout << tNode->getId() << ": " << i.getText() << std::endl;
//                        }
                    }
                }

                onlyTextNodes.push_back(tNode);
            }
            else {
                numNotFoundTextNodes++;
            }
        } else if (std::dynamic_pointer_cast<TableTreeFormNode>(ptr) != nullptr) {
            childrenDone = true;
        }
    }
    else if(iteration==3) {
        // Resolve problem nodes using dynamic programming
//        std::cout<<"Problem size: "<<problemNodes.size()<<std::endl;

        if(problemNodes.size()!=0) {

            std::vector<int> selectedEntries(problemNodes.size(), 0);
            std::vector<int> minSelectedEntries(problemNodes.size(), 0);

            std::vector<std::function<bool()>> rules;

            std::vector<std::pair<std::shared_ptr<TextTreeFormNode>, std::vector<TextualData>>> problemNodesB;

            for (auto i:problemNodes) {
                problemNodesB.push_back(i);

                std::vector<std::function<bool()>> functionalRules;
                {
                    std::shared_ptr<TreeFormModel> model2 = std::dynamic_pointer_cast<TreeFormModel>(model);
                    std::shared_ptr<TextTreeFormNode> theNode = i.first;
                    std::for_each(theNode->rulesModel.begin(), theNode->rulesModel.end(),
                                  [&](std::pair<std::string, std::unordered_set<std::string>> thePair) {
                                      if (thePair.first == "is_below") {
                                          std::unordered_set<std::string> value = thePair.second;
                                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
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
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
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
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
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
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
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
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
                                                      cv::Rect b = second->getRegion();
                                                      return std::max(0, std::min(a.x + a.width, b.x + b.width) -
                                                                         std::max(a.x, b.x)) >
                                                             0;
                                                  });
                                              }
                                          });

                                      } else if (thePair.first == "has_horizontal_overlap_with") {
                                          std::unordered_set<std::string> value = thePair.second;
                                          std::for_each(value.begin(), value.end(), [&](const std::string &x) {
                                              std::vector<std::string> hierarchy = HelperMethods::regexSplit(x, "[:]");
                                              std::shared_ptr<BasicTreeFormNode> second = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                                      model2->getNode(root, hierarchy));
                                              if (second->isRegionDefined()) {
                                                  functionalRules.push_back([=]() -> bool {
                                                      cv::Rect a = theNode->getRegion();
                                                      cv::Rect b = second->getRegion();
                                                      return std::max(0,
                                                                      std::min(a.y + a.height, b.y + b.height) -
                                                                      std::max(a.y, b.y)) >
                                                             0;
                                                  });
                                              }
                                          });

                                      }
                                  });
                }
                rules.insert(rules.begin(), functionalRules.begin(), functionalRules.end());
            }

            std::function<int()> calculateCost = [&]() {
                int newCost = 0;
                for (auto i:rules) {
                    if (!i())
                        newCost++;
                }
                return newCost;
            };

            int cost = calculateCost();

            std::function<void(int level)> recursive = [&](int level) {
                for (int i = 0; i < problemNodesB[level].second.size(); i++) {
                    selectedEntries[level] = i;
                    problemNodesB[level].first->setText(problemNodesB[level].second[i].getText());
                    problemNodesB[level].first->setRegion(problemNodesB[level].second[i].getRect());
                    int newCost = calculateCost();
                    if (newCost < cost) {
                        minSelectedEntries = selectedEntries;
                    }
                    if (level + 1 < problemNodesB.size())
                        recursive(level + 1);
                }
            };

            recursive(0);

            for (size_t i = 0; i < minSelectedEntries.size(); i++) {
                problemNodesB[i].first->setText(problemNodesB[i].second[minSelectedEntries[i]].getText());
                problemNodesB[i].first->setRegion(problemNodesB[i].second[minSelectedEntries[i]].getRect());

            }
        }

        childrenDone=true;
    }
        // Input node iteration
    else if (iteration == 4) {
        if (std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<RepeatInputTreeFormNode> rModel = std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr);

//        cout << "Repeat here" << endl;
            std::vector<std::function<bool(const TextualData &n)>> functionalRules;
            convertRulesToFunctions(rModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);


            bool divisionRegionIdentified = false;
            cv::Rect dividedTextRegion;

            for (auto i:textDividedRegion) {
                if (ptr->getId().find(i.first) != std::string::npos) {
                    dividedTextRegion = i.second;
                    divisionRegionIdentified = true;
                }
            }

            if (divisionRegionIdentified) {
                functionalRules.push_back([=](const TextualData &third) -> bool {
                    return (third.getRect() & dividedTextRegion).area() != 0;
                });
            }


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
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
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
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().x, 0, beta.first.getRect().width, 10) &
                             cv::Rect(alpha.getRect().x, 0, alpha.getRect().width, 10)).area() != 0) {
//                        cout << "Merging " << beta.first.getText() << " with " << alpha.getText() << endl;
                            dataX[beta.first].first = colValue;
                        }
                    });
//                    colValue++;
                }
            });

            std::string dx = "";

            for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
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

            std::vector<TextualData> dataVector;

            std::vector<std::vector<TextualData>>dataVector2d;

            std::unordered_set<TextualData> done;

            for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
                if (done.find(alpha.first) == done.end()) {
                    done.insert(alpha.first);
                    TextualData t = alpha.first;
                    std::vector<TextualData> t2d;
                    t2d.push_back(t);
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                             cv::Rect(alpha.first.getRect().y, 0, alpha.first.getRect().height, 10)).area() != 0 &&
                            !(alpha.first == beta.first)) {
                            t = t | beta.first;
                            t2d.push_back(beta.first);
                            done.insert(beta.first);
                        }
                    });
                    dataVector2d.push_back(t2d);
//                    dataVector.push_back(t);
//            cout << "Assigned to " << alpha.first.getText() << " value " << alpha.second.first << ","
//                 << alpha.second.second << endl;
                    //dx2 += alpha.first.getText() + "|";
                    if (!(t.getRect().x == 0 || t.getRect().y == 0)) {
                        if (!regionXDef) {
                            regionX = t.getRect();
                            regionXDef = true;
                        } else
                            regionX = t.getRect() | regionX;
                    }
                }
            });

            for(auto& i:dataVector2d) {
                std::sort(i.begin(),i.end(),
                          [&](const TextualData &d1, const TextualData &d2) -> bool {
                              return (d1.getRect().x < d2.getRect().x);
                          });
                TextualData t;
                bool iterationZero=true;
                for(auto&j:i) {
                    t=iterationZero?j:t|j;
                    iterationZero=false;
                }
                dataVector.push_back(t);
            }

            sort(dataVector.begin(), dataVector.end(),
                 [](const TextualData &a, const TextualData &b) -> bool {
                     return a.getRect().y < b.getRect().y;
                 });

            int number = 0;
            for_each(dataVector.begin(), dataVector.end(), [&](const TextualData &x) {
                dx2 += x.getText() + "|";
                number++;

                {
                    std::shared_ptr<InputTreeFormNode> newDynamicNode = std::shared_ptr<InputTreeFormNode>(
                            new InputTreeFormNode(InputTreeFormNode::INPUT_ALPHA_NUMERIC));
                    newDynamicNode->setId(rModel->getId() + ":" + std::to_string(number));

                    std::string descriptiveNameWithPlaceHolder = rModel->getDescriptiveName();
                    std::string descriptiveNameWithPlaceHolderReplacedWithRowNumber = "";
                    std::regex numberPlaceHolder("%d");
                    std::regex_replace(std::back_inserter(descriptiveNameWithPlaceHolderReplacedWithRowNumber),
                                       descriptiveNameWithPlaceHolder.begin(), descriptiveNameWithPlaceHolder.end(),
                                       numberPlaceHolder, std::to_string(number));

                    newDynamicNode->setDescriptiveName(descriptiveNameWithPlaceHolderReplacedWithRowNumber);

                    rModel->addChild(std::to_string(number), newDynamicNode);
                    std::shared_ptr<BasicTreeFormNode> nx = std::dynamic_pointer_cast<BasicTreeFormNode>(
                            rModel->getChild(std::to_string(number)));
                    if (std::dynamic_pointer_cast<InputTreeFormNode>(nx) != nullptr) {
                        std::shared_ptr<InputTreeFormNode> nx2 = std::dynamic_pointer_cast<InputTreeFormNode>(nx);
//                    cout<<"SETTING "<<coordinateString<<" to "<<alpha.first.getText()<<endl;
                        nx2->setData(nx2->getData() + x.getText());
                        nx2->setRegion(x.getRect());
                        nx2->setRegionDefined(true);


                        std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                        cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                        rectangle(*theImage, nx2->getRegion(), randomColor, 3, 8, 0);
                        putText(*theImage, nx2->getData(), nx2->getRegion().br(), 1, 2, randomColor, 2);
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

            childrenDone = true;


        } else if (std::dynamic_pointer_cast<InputTreeFormNode>(ptr) != nullptr) {
//        cout << "Running on: " << node->id << endl;
            std::shared_ptr<InputTreeFormNode> iModel = std::dynamic_pointer_cast<InputTreeFormNode>(ptr);
            if (iModel->getInputType() == InputTreeFormNode::INPUT_ALPHA_NUMERIC ||
                iModel->getInputType() == InputTreeFormNode::INPUT_NUMERIC) {

                bool divisionRegionIdentified = false;
                cv::Rect dividedTextRegion;

                for (auto i:textDividedRegion) {
                    if (ptr->getId().find(i.first) != std::string::npos) {
                        dividedTextRegion = i.second;
                        divisionRegionIdentified = true;
                    }
                }

                std::vector<std::function<bool(const TextualData &n)>> functionalRules;
                if (divisionRegionIdentified) {
                    functionalRules.push_back([=](const TextualData &third) -> bool {
                        return (third.getRect() & dividedTextRegion).area() != 0;
                    });
                }
                convertRulesToFunctions(iModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);
                std::pair<std::string, cv::Rect> oup = findTextFromFunctionalRules(functionalRules);

                std::string textExtracted = oup.first;
                iModel->setData(textExtracted);
                iModel->setRegion(oup.second);
                iModel->setRegionDefined(oup.first.size() == 0 ? false : true);

                if (oup.first.size() != 0) {
                    std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                    cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                    rectangle(*theImage, iModel->getRegion(), randomColor, 3, 8, 0);
                    putText(*theImage, iModel->getData(), iModel->getRegion().br(), 1, 2, randomColor, 2);
                }


//            cout << iModel->id << ": " << iModel->data << endl;
            }
                // Checkboxes
            else {


                int left = -1;
                int top = -1;
                int right = -1;
                int bottom = -1;
                std::for_each(iModel->rulesModel.begin(), iModel->rulesModel.end(),
                              [&](std::pair<std::string, std::unordered_set<std::string>> alpha) {
                                  if (alpha.first == "is_below") {
                                      std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                              TreeFormModel::getNode(root,
                                                                     HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                               "[:]")));
                                      if (theNode->isRegionDefined()) {
                                          if (top == -1 ||
                                              (top < theNode->getRegion().y + theNode->getRegion().height && top != -1))
                                              top = theNode->getRegion().y + theNode->getRegion().height;
                                      }
//                cout << "::" << theNode->id << endl;
//                cout << theNode->region << endl;
//                cout << (dynamic_pointer_cast<TextNode>(theNode))->textAssigned << endl;
                                  } else if (alpha.first == "is_above") {
                                      std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                              TreeFormModel::getNode(root,
                                                                     HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                               "[:]")));
                                      if (theNode->isRegionDefined()) {
                                          if (bottom == -1 || (bottom > theNode->getRegion().y && top != -1))
                                              bottom = theNode->getRegion().y;
                                      }
                                  } else if (alpha.first == "is_right_to") {
                                      std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                              TreeFormModel::getNode(root,
                                                                     HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                               "[:]")));
                                      if (theNode->isRegionDefined()) {
                                          if (left == -1 ||
                                              left < (theNode->getRegion().x + theNode->getRegion().width && top != -1))
                                              left = theNode->getRegion().x + theNode->getRegion().width;
                                      }
                                  } else if (alpha.first == "is_left_to") {
                                      std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                              TreeFormModel::getNode(root,
                                                                     HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                               "[:]")));
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

                cv::Rect rect(left, top, right - left, bottom - top);

                bool divisionRegionIdentified = false;
                cv::Rect dividedTextRegion;

                for (auto i:textDividedRegion) {
                    if (ptr->getId().find(i.first) != std::string::npos) {
                        dividedTextRegion = i.second;
                        divisionRegionIdentified = true;
                    }
                }


                if (divisionRegionIdentified) {
                    rect = rect & dividedTextRegion;
                }

                bool done = false;
                CCheckBox box;


                auto drawImage = getIterationOutputImage("checkboxes");

                cv::Scalar randomColor = cv::Scalar(((unsigned int) rng) % 255, ((unsigned int) rng) % 255,
                                                    ((unsigned int) rng) %
                                                    255);//randomColors[((unsigned int) rng) % 5];
                cv::rectangle(*drawImage, rect, randomColor, 3, 8, 0);


                /*
                 * First, check if the checkbox is a part of the text node above one level
                 */
                {
                    size_t index = iModel->getId().find_last_of(":");
                    if (index != std::string::npos) {
                        std::string parentId = iModel->getId().substr(0, index);
                        std::shared_ptr<TreeFormNodeInterface> parentUnCasted = TreeFormModel::getNode(root,
                                                                                                       HelperMethods::regexSplit(
                                                                                                               parentId,
                                                                                                               "[:]"));
                        if (parentUnCasted != nullptr) {
                            std::shared_ptr<TextTreeFormNode> parent = std::dynamic_pointer_cast<TextTreeFormNode>(
                                    parentUnCasted);
                            if (parent != nullptr) {
                                if (parent->isRegionDefined()) {
                                    for (auto i:checkboxes) {
                                        if (!i.validOuterBBox)
                                            continue;
                                        if ((i.outerBBox & parent->getRegion()).area() != 0) {
                                            iModel->setData(i.isFilled ? "True" : "False");
                                            iModel->setRegion(i.outerBBox);
                                            iModel->setRegionDefined(true);

                                            std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                                            cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                                            rectangle(*theImage, iModel->getRegion(), randomColor, 3, 8, 0);
                                            putText(*theImage, iModel->getData(), iModel->getRegion().br(), 1, 2,
                                                    randomColor, 2);

                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for (auto i:checkboxes) {
                    if ((i.outerBBox & rect).area() == i.outerBBox.area()) {
                        if (done) {
                            std::cout << "Warning: multiple checkboxes in region of " << iModel->getId() << std::endl;
                        }
                        done = true;
                        box = i;
                    }
                }

                if (!done) {
                    std::cout << "Warning: no checkbox found in the region of" << iModel->getId() << std::endl;
                } else {
                    iModel->setRegionDefined(true);
                    iModel->setRegion(box.outerBBox);
                    iModel->setData(box.isFilled ? "True" : "False");


                    std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                    cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                    rectangle(*theImage, iModel->getRegion(), randomColor, 3, 8, 0);
                    putText(*theImage, iModel->getData(), iModel->getRegion().br(), 1, 2, randomColor, 2);
                }

            }
        } else if (std::dynamic_pointer_cast<TableTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<TableTreeFormNode> tModel = std::dynamic_pointer_cast<TableTreeFormNode>(ptr);

            int left = -1;
            int top = -1;
            int right = -1;
            int bottom = -1;
            std::for_each(tModel->rulesModel.begin(), tModel->rulesModel.end(),
                          [&](std::pair<std::string, std::unordered_set<std::string>> alpha) {
                              if (alpha.first == "is_below") {
                                  std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                          TreeFormModel::getNode(root,
                                                                 HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                           "[:]")));
                                  if (theNode->isRegionDefined()) {
                                      if (top == -1 ||
                                          (top < theNode->getRegion().y + theNode->getRegion().height && top != -1))
                                          top = theNode->getRegion().y + theNode->getRegion().height;
                                  }
//                cout << "::" << theNode->id << endl;
//                cout << theNode->region << endl;
//                cout << (dynamic_pointer_cast<TextNode>(theNode))->textAssigned << endl;
                              } else if (alpha.first == "is_above") {
                                  std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                          TreeFormModel::getNode(root,
                                                                 HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                           "[:]")));
                                  if (theNode->isRegionDefined()) {
                                      if (bottom == -1 || (bottom > theNode->getRegion().y && top != -1))
                                          bottom = theNode->getRegion().y;
                                  }
                              } else if (alpha.first == "is_right_to") {
                                  std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                          TreeFormModel::getNode(root,
                                                                 HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                           "[:]")));
                                  if (theNode->isRegionDefined()) {
                                      if (left == -1 ||
                                          left < (theNode->getRegion().x + theNode->getRegion().width && top != -1))
                                          left = theNode->getRegion().x + theNode->getRegion().width;
                                  }
                              } else if (alpha.first == "is_left_to") {
                                  std::shared_ptr<BasicTreeFormNode> theNode = std::dynamic_pointer_cast<BasicTreeFormNode>(
                                          TreeFormModel::getNode(root,
                                                                 HelperMethods::regexSplit(*(alpha.second.begin()),
                                                                                           "[:]")));
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


            bool divisionRegionIdentified = false;
            cv::Rect dividedTextRegion;

            for (auto i:textDividedRegion) {
                if (ptr->getId().find(i.first) != std::string::npos) {
                    dividedTextRegion = i.second;
                    divisionRegionIdentified = true;
                }
            }

            cv::Rect r(left, top, right - left, bottom - top);


            if (divisionRegionIdentified) {
                r = r & dividedTextRegion;
            }

            std::regex hasAnAlphaNumericCharacter(".*([0-9-]|[A-Z]|[a-z]).*");

            std::unordered_map<TextualData, std::pair<int, int>> croppedTextualData;
            std::vector<TextualData> croppedTextualDataB;
            std::for_each(text.begin(), text.end(), [&](TextualData &d) {
                if ((r & d.getRect()).area() != 0 && std::regex_match(d.getText(), hasAnAlphaNumericCharacter)) {
                    croppedTextualData[d] = std::pair<int, int>(-1, -1);
                    croppedTextualDataB.push_back(d);
                }
            });

            // Collect all the children into a vector
            std::vector<std::shared_ptr<TreeFormNodeInterface>> tableChildrenNodes;
            tModel->getChildren(tableChildrenNodes);


            auto pairHash = [](std::pair<int, int> const &foo) {
                return foo.first + foo.second * 997;
            };

            std::unordered_set<std::pair<int, int>, decltype(pairHash)> takenCoordinates(0, pairHash);

            // Assign text to text nodes in the table (row and column headers)
            for (auto i:tableChildrenNodes) {
                if (std::dynamic_pointer_cast<TextTreeFormNode>(i) != nullptr) {
                    std::shared_ptr<TextTreeFormNode> textNode = std::dynamic_pointer_cast<TextTreeFormNode>(i);
                    int minIndex = findTextWithMinimumEditDistance(croppedTextualDataB, textNode->getText());
                    if (minIndex != -1) {
                        std::pair<int, int> coordinates = getCartesianCoordinateOfTableNode(i->getId());
                        if (coordinates.first != -1) {
                            croppedTextualData[croppedTextualDataB[minIndex]] = coordinates;
                            takenCoordinates.insert(coordinates);
                            textNode->setTextAssigned(croppedTextualDataB[minIndex].getText());
                            textNode->setRegion(croppedTextualDataB[minIndex].getRect());
                            textNode->setRegionDefined(true);
                        }
                    }
                }
            }

            // Assign row numbers wrt to textual nodes
            {
                std::unordered_set<TextualData> done;
                for (auto &i : croppedTextualData) {
                    if (done.find(i.first) != done.end())
                        continue;
                    if (i.second.first == -1 || i.second.second == -1)
                        continue;
                    done.insert(i.first);
                    for (auto &j:croppedTextualData) {
                        if (done.find(j.first) != done.end())
                            continue;
                        if (j.second.second != -1)
                            continue;
                        cv::Rect a = i.first.getRect();
                        cv::Rect b = j.first.getRect();
                        if (std::max(0, std::min(a.y + a.height, b.y + b.height) - std::max(a.y, b.y)) > 0) {
                            j.second.second = i.second.second;
                            done.insert(j.first);
                        }
                    }
                }
            }

            // Assign column numbers wrt to textual nodes
            {
                std::unordered_set<TextualData> done;
                for (auto &i : croppedTextualData) {
                    if (done.find(i.first) != done.end())
                        continue;
                    if (i.second.first == -1 || i.second.second == -1)
                        continue;
                    done.insert(i.first);
                    for (auto &j:croppedTextualData) {
                        if (done.find(j.first) != done.end())
                            continue;
                        if (j.second.first != -1)
                            continue;
                        cv::Rect a = i.first.getRect();
                        cv::Rect b = j.first.getRect();
                        if (std::max(0, std::min(a.x + a.width, b.x + b.width) - std::max(a.x, b.x)) > 0) {
                            j.second.first = i.second.first;

                            // If both coordinates are assigned
                            if (j.second.second != -1) {
                                takenCoordinates.insert(j.second);
                            }

                            done.insert(j.first);
                        }
                    }
                }
            }

            // TODO: Making one small assumption: every cell will have a row number assigned here (true for all K-1 forms) might have to modify later

            // Now give them column numbers (they shouldn't have been taken)
            {
                // Sort wrt x-coordinates
                std::vector<TextualData> xSorted;
                for (auto i:croppedTextualData) {
                    xSorted.push_back(i.first);
                }
                for (auto i:xSorted) {
                    if (croppedTextualData[i].first != -1)
                        continue;
                    for (int j = 0; j < tModel->getNumCols(); ++j) {
                        if (takenCoordinates.find(std::pair<int, int>(j, croppedTextualData[i].second)) !=
                            takenCoordinates.end())
                            continue;
//                        std::cout<<"Here"<<std::endl;
                        croppedTextualData[i].first = j;
                        takenCoordinates.insert(std::pair<int, int>(j, croppedTextualData[i].second));
                        break;

                    }
                }
            }

            // In the last stage, put the data in the table pointer
            for (auto i:croppedTextualData) {
                std::string coordinateString;

                coordinateString += "(";
                coordinateString += std::to_string(i.second.first);
                coordinateString += ",";
                coordinateString += std::to_string(i.second.second);
                coordinateString += ")";

                if (tModel->getChild(coordinateString) != nullptr) {
                    std::shared_ptr<TreeFormNodeInterface> tableChild = tModel->getChild(coordinateString);


                    if (std::dynamic_pointer_cast<InputTreeFormNode>(tableChild) != nullptr) {
                        std::shared_ptr<InputTreeFormNode> inputTableChild = std::dynamic_pointer_cast<InputTreeFormNode>(
                                tableChild);

                        inputTableChild->setData(inputTableChild->getData() + i.first.getText());
                        inputTableChild->setRegion(i.first.getRect());
                        inputTableChild->setRegionDefined(true);
                        std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                        cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                        rectangle(*theImage, inputTableChild->getRegion(), randomColor, 3, 8, 0);
                        putText(*theImage, inputTableChild->getData(), inputTableChild->getRegion().br(), 1, 2,
                                randomColor, 2);
                    }
                }
            }

            childrenDone = true;
        }
    } else if (iteration == 5) {
        if(numNotFoundTextNodes>3) {
            std::cout<<"Problem identifying text: Please send it for inspection"<<std::endl;
        }

        if (false) {
            std::function<float(cv::Rect a, cv::Rect b)> getDistanceSquared = [](cv::Rect a, cv::Rect b) -> float {
                cv::Point p1 = {(a.x + a.width / 2), (a.y + a.height / 2)};


                cv::Point p2 = {(b.x + b.width / 2), (b.y + b.height / 2)};

                return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
            };

            std::function<bool(std::shared_ptr<TextTreeFormNode> &left, std::shared_ptr<TextTreeFormNode> &right,
                               std::shared_ptr<TextTreeFormNode> &top, std::shared_ptr<TextTreeFormNode> &bottom,
                               const std::shared_ptr<TextTreeFormNode> &input)> findNeighbors = [&](
                    std::shared_ptr<TextTreeFormNode> &left, std::shared_ptr<TextTreeFormNode> &right,
                    std::shared_ptr<TextTreeFormNode> &bottom, std::shared_ptr<TextTreeFormNode> &top,
                    const std::shared_ptr<TextTreeFormNode> &input) -> bool {

//            std::shared_ptr<TextTreeFormNode> leftSmallest;
//            std::shared_ptr<TextTreeFormNode> leftSmallest;
//            std::shared_ptr<TextTreeFormNode> topNodes;
//            std::shared_ptr<TextTreeFormNode> bottomNodes;

                left = right = top = bottom = nullptr;

                float leftDistance;
                float rightDistance;
                float topDistance;
                float bottomDistance;
                cv::Rect b = input->getRegion();

                for (auto i:onlyTextNodes) {
                    if (i == input) {
                        continue;
                    }

                    cv::Rect a = i->getRegion();

                    float newDistance = getDistanceSquared(a, b);

                    if (b.width + b.x < a.x) {
                        if (left == nullptr) {
                            left = i;
                            leftDistance = getDistanceSquared(a, b);
                        } else if (newDistance < leftDistance) {
                            left = i;
                            leftDistance = newDistance;
                        }
                    }
                    if (b.x > a.x + a.width) {
                        if (right == nullptr) {
                            right = i;
                            rightDistance = getDistanceSquared(a, b);
                        } else if (newDistance < rightDistance) {
                            right = i;
                            rightDistance = newDistance;
                        }
                    }

                    if (b.height + b.y < a.y) {
                        if (top == nullptr) {
                            top = i;
                            topDistance = getDistanceSquared(a, b);
                        } else if (newDistance < topDistance) {
                            top = i;
                            topDistance = newDistance;
                        }
                    }

                    if (b.y > a.y + a.height) {
                        if (bottom == nullptr) {
                            bottom = i;
                            bottomDistance = getDistanceSquared(a, b);
                        } else if (newDistance < bottomDistance) {
                            bottom = i;
                            bottomDistance = newDistance;
                        }
                    }

                }

                return false;
            };


            for (auto i:onlyTextNodes) {
                std::shared_ptr<TextTreeFormNode> left;
                std::shared_ptr<TextTreeFormNode> right;
                std::shared_ptr<TextTreeFormNode> bottom;
                std::shared_ptr<TextTreeFormNode> top;

                findNeighbors(left, right, top, bottom, i);

                if (left != nullptr) {
                    std::cout << "RULE " << i->getId() << " IS_LEFT_TO " << left->getId() << std::endl;
                }
                if (right != nullptr) {
                    std::cout << "RULE " << i->getId() << " IS_RIGHT_TO " << right->getId() << std::endl;
                }
                if (top != nullptr) {
                    std::cout << "RULE " << i->getId() << " IS_BELOW " << top->getId() << std::endl;
                }
                if (bottom != nullptr) {
                    std::cout << "RULE " << i->getId() << " IS_ABOVE " << bottom->getId() << std::endl;
                }
            }
        }

        childrenDone = true;
    }

    return true;
}

void TreeFormNodeProcessor::setForm(const std::shared_ptr<RawFormInterface> &form) {
    form->getText(text);
    form->getRasterImage(image);
}


int TreeFormNodeProcessor::findTextWithMinimumEditDistanceMulti(std::vector<TextualData> &text, std::string textToFind,
                                                                 cv::Rect region, std::vector<TextualData> &result) {

    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    for (size_t i = 0; i < text.size(); i++) {
        if ((region & text[i].getRect()).area() == 0)
            continue;

        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;


        for (size_t i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
        }

        int newDistance = textDistanceFinder->calcualteDistance(dataCurrent2.c_str(),
                                                                textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            result.clear();
            result.push_back(text[i]);
        } else if (newDistance == minDistance) {
            result.push_back(text[i]);
        }
    }

    return minDistance;
}


int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> &text, std::string textToFind) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    for (size_t i = 0; i < text.size(); i++) {
        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;

        for (size_t i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
        }

        int newDistance = textDistanceFinder->calcualteDistance(dataCurrent2.c_str(),
                                                                textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
        }
    }

    return minIndex;
}


int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> &text, std::string textToFind,
                                                           int &numMatches) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    numMatches = 0;
    for (size_t i = 0; i < text.size(); i++) {
        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;

        for (size_t i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
        }

        int newDistance = textDistanceFinder->calcualteDistance(dataCurrent2.c_str(),
                                                                textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
            numMatches = 1;
        } else if (newDistance == minDistance) {
            ++numMatches;
        }
    }

    return minIndex;
}

int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> &text, std::string textToFind,
                                                           cv::Rect region, int &numMatches) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    numMatches = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if ((region & text[i].getRect()).area() == 0)
            continue;

        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;


        for (size_t i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
        }

        int newDistance = textDistanceFinder->calcualteDistance(dataCurrent2.c_str(),
                                                                textToFind.c_str());
        if (newDistance < minDistance) {
            minDistance = newDistance;
            minIndex = i;
            numMatches = 1;
        } else if (newDistance == minDistance) {
            ++numMatches;
        }
    }

    return minIndex;
}


int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> &text, std::string textToFind,
                                                           cv::Rect region) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    for (size_t i = 0; i < text.size(); i++) {
        if ((region & text[i].getRect()).area() == 0)
            continue;

        std::string dataCurrent = text[i].getText();
        std::string dataCurrent2;


        for (size_t i = 0; i < dataCurrent.size(); i++) {
            if (HelperMethods::isAlphaNumericNotSpace(dataCurrent[i]))
                dataCurrent2 += dataCurrent[i];
        }

        int newDistance = textDistanceFinder->calcualteDistance(dataCurrent2.c_str(),
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
                          text += (text.length()!=0?" ":"") + currentData.getText();

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

TreeFormNodeProcessor::TreeFormNodeProcessor(const cv::Mat &image, const std::vector<TextualData> &nonMergedWords,
                                             const std::shared_ptr<BasicTreeFormNode> &root) {
    TreeFormNodeProcessor::image = image;
    TreeFormNodeProcessor::unmergedText = nonMergedWords;
    TreeFormNodeProcessor::root = root;
    textDistanceFinder = std::shared_ptr<TextDistanceInterface>(
            new BasicLevenshteinDistance()); // Here we are making an concrete class without interface.
}

cv::Mat TreeFormNodeProcessor::getCheckboxesImage() {
    return getIterationOutputImage("checkboxes")->clone();
}

cv::Mat TreeFormNodeProcessor::getDivisionImage() {
    return getIterationOutputImage("division")->clone();
}

cv::Mat TreeFormNodeProcessor::getInputImage() {
    return getIterationOutputImage("inputs")->clone();
}

std::shared_ptr<cv::Mat> TreeFormNodeProcessor::getIterationOutputImage(std::string key) {
    auto iteratorImageFinder = images.find(key);
    std::shared_ptr<cv::Mat> theImage;
    if (iteratorImageFinder == images.end()) {
//        std::cout << "Making new copy of " << key << std::endl;
        if (key == "division") {
            cv::Mat binaryImage;
//            Preprocessor::binarizeShafait(image,binaryImage,100,0.5);
            cv::Mat image2;
            cv::cvtColor(image, image2, CV_BGR2GRAY);
            cv::threshold(image2, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

            cv::Mat image3;
            cv::cvtColor(binaryImage, image3, CV_GRAY2BGR);

            images[key] = theImage = std::shared_ptr<cv::Mat>(new cv::Mat(image3.clone()));
        } else {
            images[key] = theImage = std::shared_ptr<cv::Mat>(new cv::Mat(image.clone()));
        }
    } else {
        theImage = iteratorImageFinder->second;
    }
    return theImage;
}

void TreeFormNodeProcessor::mergeWordBoxes(const std::vector<TextualData> &words, std::vector<TextualData> &elemBoxes) {
    // Merge the words extracted from Tesseract to obtain text-lines. The logic used for text-line extraction
    // is to merge two consecutive words if they overlap along the y-axis, and the gap between them is smaller
    // than the height of the shorter word.
    std::shared_ptr<cv::Mat> drawImage = getIterationOutputImage("division");
    int nRects = words.size();
    bool newElem = true;
    TextualData elem, prevWord;
    cv::Mat binaryImage, image2;
    cv::cvtColor(image, image2, CV_BGR2GRAY);
    cv::threshold(image2, binaryImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    float totalHeight=0;

    cv::Mat testMat = image.clone();
    testMat = 255;
    for (int i = 0; i < nRects; i++) {
        TextualData currWord = words[i];
        totalHeight+=words[i].getRect().height;
        if (!newElem) {
            int hGap = currWord.getRect().x - prevWord.getRect().x - prevWord.getRect().width;
            int hGapThresh = std::min(currWord.getRect().height, prevWord.getRect().height);
            bool vOverlap = false;
            if (((currWord.getRect().y <= prevWord.getRect().y) &&
                 (currWord.getRect().y + currWord.getRect().height > prevWord.getRect().y)) ||
                ((prevWord.getRect().y <= currWord.getRect().y) &&
                 (prevWord.getRect().y + prevWord.getRect().height > currWord.getRect().y)))
                vOverlap = true;
            bool hasSeparation = false;

            {
                int x1 = prevWord.getRect().x + prevWord.getRect().width + 2;
                int y1 = prevWord.getRect().y + 2;

                int x2 = currWord.getRect().x - 2;
                int y2 = currWord.getRect().y + currWord.getRect().height - 2;

                // TODO: Add conditions, if +2 and -2 goes beyond image bounds
                if (x1 < x2 && y1 < y2) {
                    for (int i = x1; i <= x2; i++) {
                        for (int j = y1; j <= y2; j++) {
                            if (binaryImage.at<uchar>(j, i) == 0) {
                                hasSeparation = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (vOverlap && (hGap > 0) && (hGap < hGapThresh) && !hasSeparation) {
                elem = elem | currWord;
                prevWord = currWord;
            } else {
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

    float averageHeight=totalHeight/nRects;

    for(auto &i:elemBoxes) {
        cv::Rect rec=i.getRect();
        rec.x-=averageHeight;
        rec.width+=averageHeight;
        i.setRect(rec);
    }

//    std::cout<<"Merged data"<<std::endl;
//    for(auto i:elemBoxes) {
//        std::cout<<i.getText()<<std::endl;
//    }
}

void TreeFormNodeProcessor::setTextDistanceFinder(const std::shared_ptr<TextDistanceInterface> &textDistanceFinder) {
    TreeFormNodeProcessor::textDistanceFinder = textDistanceFinder;
}

std::pair<int, int> TreeFormNodeProcessor::getCartesianCoordinateOfTableNode(const std::string &fullNodeId) {
    std::string testString = fullNodeId;
    size_t myNameSeparator = testString.find_last_of(':');
    std::string b = testString.substr(myNameSeparator + 1, testString.size() - myNameSeparator - 1);
    std::regex pieces_regex("[(](\\d+)[,](\\d+)[)]");
    std::smatch pieces_match;
    int x = -1;
    int y = -1;
    if (std::regex_match(b, pieces_match, pieces_regex)) {
        x = std::stoi(pieces_match[1].str());
        y = std::stoi(pieces_match[2].str());
    }
    return std::pair<int, int>(x, y);
}