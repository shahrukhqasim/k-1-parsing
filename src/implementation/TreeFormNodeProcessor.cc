
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

std::shared_ptr<cv::Mat> TreeFormNodeProcessor::getIterationOutputImage(std::string key) {
    auto iteratorImageFinder=images.find(key);
    std::shared_ptr<cv::Mat> theImage;
    if(iteratorImageFinder==images.end()) {
        std::cout<<"Making new copy of "<<key<<std::endl;
        images[key]=theImage=std::shared_ptr<cv::Mat>(new cv::Mat(image.clone()));
    }
    else {
        theImage=iteratorImageFinder->second;
    }
    return theImage;
}

bool TreeFormNodeProcessor::process(std::shared_ptr<TreeFormNodeInterface> ptr,
                                    std::shared_ptr<TreeFormModelInterface> model, int iteration,
                                    bool &childrenDone) {
    childrenDone=false;
    // Pre-processing iteration
    if(iteration==0) {

        CDetectCheckBoxes checkboxesDetector;

        cv::Mat imageGrey;
        cv::cvtColor(image, imageGrey, CV_BGR2GRAY);
        checkboxesDetector.detectCheckBoxes(imageGrey, checkboxes);

        childrenDone=true;

        auto drawImage=getIterationOutputImage("checkboxes");

        for(auto i:checkboxes) {
            if(i.validOuterBBox){
                cv::rectangle(*drawImage, i.outerBBox, cv::Scalar(0, 255, 0), 3, 8, 0);
            } else if (i.validInnerBBox){
//                cv::rectangle(*drawImage, i.innerBBox, cv::Scalar(0, 255, 0), 3, 8, 0);
            }
        }

        return true;
    }
    // Text division iteration
    else if (iteration == 1) {

        std::shared_ptr<cv::Mat>drawImage=getIterationOutputImage("division");
        for (int i = 0; i < text.size(); i++) {
            cv::rectangle(*drawImage, text[i].getRect(), cv::Scalar(0, 255, 255), 3, 8, 0);
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

            int left=0;
            int top=0;
            int right=image.cols;
            int bottom=image.rows;


            if (isLeft || isRight) {
                cv::Mat vProjection(1, image.cols, CV_16U);
                vProjection = 0;
                for (int i = 0; i < text.size(); i++) {
                    cv::Rect r = text[i].getRect();
                    for (int j = r.x; j < r.x + r.width; j++)
                        vProjection.at<short>(0, j) += r.height;
                }


                std::shared_ptr<cv::Mat>drawImage=getIterationOutputImage("division");
                for (int i = 0; i < vProjection.cols; i++) {
                    line(*drawImage,cv::Point(i,0),cv::Point(i,vProjection.at<short>(0,i)),cv::Scalar(255,0,0));
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
                    right=shallowProjectionIndex;
                    textB = leftBoxes;
                } else if (isRight) {
                    left=shallowProjectionIndex;
                    textB = rightBoxes;
                }
            }

            for (auto i:bNode->divisionRules) {
                std::cout<<i->getRuleKeyword()<<std::endl;
                if (std::dynamic_pointer_cast<DivisionRuleWithReference>(i) != nullptr) {
                    std::cout<<"Reference"<<std::endl;
                    std::shared_ptr<DivisionRuleWithReference> castedRule = std::dynamic_pointer_cast<DivisionRuleWithReference>(
                            i);

                    std::shared_ptr<TreeFormNodeInterface> nodeF=TreeFormModel::getNode(root, HelperMethods::regexSplit(castedRule->getOtherNodeId(),":"));

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
                        if(bottom>foundText.getRect().y)
                            bottom=foundText.getRect().y;
                    } else if (castedRule->getRuleKeyword() == "is_below") {
                        if(top<foundText.getRect().y+foundText.getRect().height)
                            top=foundText.getRect().y+foundText.getRect().height;
                    } else if (castedRule->getRuleKeyword() == "is_left_to") {
                        if(right>foundText.getRect().x)
                            right=foundText.getRect().x;
                    } else if (castedRule->getRuleKeyword() == "is_right_to") {
                        if(left<foundText.getRect().x+foundText.getRect().width)
                            left=foundText.getRect().x+foundText.getRect().width;
                    } else if (castedRule->getRuleKeyword() == "is_above_inclusive") {
                        if(bottom>foundText.getRect().y+foundText.getRect().height)
                            bottom=foundText.getRect().y+foundText.getRect().height;
                    } else if (castedRule->getRuleKeyword() == "is_below_inclusive") {
                        if(top<foundText.getRect().y)
                            top=foundText.getRect().y;
                    } else {
                        std::cerr << "Division rule not identified" << std::endl;
                        return false;
                    }
                }
            }

            textDividedRegion[bNode->getId()]=cv::Rect(left,top,right-left,bottom-top);
            std::shared_ptr<cv::Mat>drawImage=getIterationOutputImage("division");

            std::cout<<"Division rect "<<bNode->getId()<<": "<<cv::Rect(left,top,right-left,bottom-top)<<std::endl;

            cv::Scalar color(0,0,255);
            if(colorBucket.size()!=0) {
                color = colorBucket[0];
                colorBucket.erase(colorBucket.begin());
            }


           // cv::rectangle(*drawImage, cv::Rect(left,top,right-left,bottom-top), color, 3, 8, 0);

        }
    }
        // Minimum text iteration
    else if (iteration == 2) {
        bool divisionRegionIdentified=false;
        cv::Rect dividedTextRegion;

        for(auto i:textDividedRegion) {
            if(ptr->getId().find(i.first)!=std::string::npos) {
                dividedTextRegion=i.second;
                divisionRegionIdentified=true;
            }
        }

        cv::Rect rectangle;
        if (std::dynamic_pointer_cast<TextTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<TextTreeFormNode> tNode = std::dynamic_pointer_cast<TextTreeFormNode>(ptr);
            std::string text = tNode->getText();

            int minIndex;
            if(divisionRegionIdentified) {
                minIndex = findTextWithMinimumEditDistance(TreeFormNodeProcessor::text,text,dividedTextRegion);
            }
            else {
                minIndex = findTextWithMinimumEditDistance(TreeFormNodeProcessor::text,text);
            }


            if (minIndex != -1) {
                tNode->setRegion(TreeFormNodeProcessor::text[minIndex].getRect());
                tNode->setTextAssigned(TreeFormNodeProcessor::text[minIndex].getText());
                rectangle = tNode->getRegion();
                tNode->setRegionDefined(true);
                takenText.push_back(TreeFormNodeProcessor::text[minIndex]);
                TreeFormNodeProcessor::text.erase(TreeFormNodeProcessor::text.begin() + minIndex);
            }
        }
        else
        if (std::dynamic_pointer_cast<TableTreeFormNode>(ptr) != nullptr) {
            childrenDone=true;
        }
    }
        // Input node iteration
    else if (iteration == 3) {
        if (std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr) != nullptr) {
            std::shared_ptr<RepeatInputTreeFormNode> rModel = std::dynamic_pointer_cast<RepeatInputTreeFormNode>(ptr);

//        cout << "Repeat here" << endl;
            std::vector<std::function<bool(const TextualData &n)>> functionalRules;
            convertRulesToFunctions(rModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);


            bool divisionRegionIdentified=false;
            cv::Rect dividedTextRegion;

            for(auto i:textDividedRegion) {
                if(ptr->getId().find(i.first)!=std::string::npos) {
                    dividedTextRegion=i.second;
                    divisionRegionIdentified=true;
                }
            }

            if(divisionRegionIdentified) {
                functionalRules.push_back([=](const TextualData &third) -> bool {
                    return (third.getRect() & dividedTextRegion).area()!=0;
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

            std::unordered_set<TextualData>done;

            for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> alpha) {
                if(done.find(alpha.first)==done.end()) {
                    done.insert(alpha.first);
                    TextualData t = alpha.first;
                    for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                        if ((cv::Rect(beta.first.getRect().y, 0, beta.first.getRect().height, 10) &
                             cv::Rect(alpha.first.getRect().y, 0, alpha.first.getRect().height, 10)).area() != 0 &&
                            !(alpha.first == beta.first)) {
                            t = t | beta.first;
                            done.insert(beta.first);
                        }
                    });
                    dataVector.push_back(t);
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

                    rModel->addChild(std::to_string(number), newDynamicNode);
                    std::shared_ptr<BasicTreeFormNode> nx = std::dynamic_pointer_cast<BasicTreeFormNode>(
                            rModel->getChild(std::to_string(number)));
                    if (std::dynamic_pointer_cast<InputTreeFormNode>(nx) != nullptr) {
                        std::shared_ptr<InputTreeFormNode> nx2 = std::dynamic_pointer_cast<InputTreeFormNode>(nx);
//                    cout<<"SETTING "<<coordinateString<<" to "<<alpha.first.getText()<<endl;
                        nx2->setData(nx2->getData() + x.getText());
                        nx2->setRegion(x.getRect());
                        nx2->setRegionDefined(true);
                        nx2->setDescriptiveName(rModel->getDescriptiveName());

                        std::shared_ptr<cv::Mat>theImage=getIterationOutputImage("inputs");
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

            childrenDone=true;


        } else if (std::dynamic_pointer_cast<InputTreeFormNode>(ptr) != nullptr) {
//        cout << "Running on: " << node->id << endl;
            std::shared_ptr<InputTreeFormNode> iModel = std::dynamic_pointer_cast<InputTreeFormNode>(ptr);
            if (iModel->getInputType() == InputTreeFormNode::INPUT_ALPHA_NUMERIC ||
                iModel->getInputType() == InputTreeFormNode::INPUT_NUMERIC) {

                bool divisionRegionIdentified=false;
                cv::Rect dividedTextRegion;

                for(auto i:textDividedRegion) {
                    if(ptr->getId().find(i.first)!=std::string::npos) {
                        dividedTextRegion=i.second;
                        divisionRegionIdentified=true;
                    }
                }

                std::vector<std::function<bool(const TextualData &n)>> functionalRules;
                if(divisionRegionIdentified) {
                    functionalRules.push_back([=](const TextualData &third) -> bool {
                        return (third.getRect() & dividedTextRegion).area()!=0;
                    });
                }
                convertRulesToFunctions(iModel, std::dynamic_pointer_cast<TreeFormModel>(model), root, functionalRules);
                std::pair<std::string, cv::Rect> oup = findTextFromFunctionalRules(functionalRules);

                std::string textExtracted = oup.first;
                iModel->setData(textExtracted);
                iModel->setRegion(oup.second);
                iModel->setRegionDefined(oup.first.size() == 0 ? false : true);

                if(oup.first.size()!=0) {
                    std::shared_ptr<cv::Mat>theImage=getIterationOutputImage("inputs");
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

                cv::Rect rect(left,top,right-left,bottom-top);

                bool divisionRegionIdentified=false;
                cv::Rect dividedTextRegion;

                for(auto i:textDividedRegion) {
                    if(ptr->getId().find(i.first)!=std::string::npos) {
                        dividedTextRegion=i.second;
                        divisionRegionIdentified=true;
                    }
                }


                if(divisionRegionIdentified) {
                    rect=rect&dividedTextRegion;
                }

                bool done=false;
                CCheckBox box;


                auto drawImage=getIterationOutputImage("checkboxes");

                cv::Scalar randomColor = cv::Scalar(((unsigned int) rng) % 255,((unsigned int) rng) % 255,((unsigned int) rng) % 255);//randomColors[((unsigned int) rng) % 5];
                cv::rectangle(*drawImage, rect, randomColor, 3, 8, 0);


                /*
                 * First, check if the checkbox is a part of the text node above one level
                 */
                {
                    size_t index=iModel->getId().find_last_of(":");
                    if(index!=std::string::npos) {
                        std::string parentId=iModel->getId().substr(0,index);
                        std::shared_ptr<TreeFormNodeInterface> parentUnCasted= TreeFormModel::getNode(root,HelperMethods::regexSplit(parentId,"[:]"));
                        if(parentUnCasted!= nullptr) {
                            std::shared_ptr<TextTreeFormNode> parent = std::dynamic_pointer_cast<TextTreeFormNode>(
                                    parentUnCasted);
                            if(parent!= nullptr) {
                                if(parent->isRegionDefined()) {
                                    for (auto i:checkboxes) {
                                        if(!i.validOuterBBox)
                                            continue;
                                        if ((i.outerBBox & parent->getRegion()).area()!=0) {
                                            iModel->setData(i.isFilled?"True":"False");
                                            iModel->setRegion(i.outerBBox);
                                            iModel->setRegionDefined(true);
                                            std::cout<<"Found a region from parent"<<iModel->getId()<<" for checkbox "<<iModel->getRegion()<<" "<<iModel->isRegionDefined()<<std::endl;
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(auto i:checkboxes) {
                    if((i.outerBBox&rect).area()!=0) {
                        if(done) {
                            std::cout<<"Warning: multiple checkboxes in region of "<<iModel->getId()<<std::endl;
                        }
                        done=true;
                        box=i;
                    }
                }

                if(!done) {
                    std::cout<<"Warning: no checkbox found in the region of"<<iModel->getId()<<std::endl;
                }
                else {
                    iModel->setRegionDefined(true);
                    iModel->setRegion(box.outerBBox);
                    iModel->setData(box.isFilled?"True":"False");
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


            bool divisionRegionIdentified=false;
            cv::Rect dividedTextRegion;

            for(auto i:textDividedRegion) {
                if(ptr->getId().find(i.first)!=std::string::npos) {
                    dividedTextRegion=i.second;
                    divisionRegionIdentified=true;
                }
            }


//        cout << left << " " << top << " " << right << " " << bottom << endl;
//        cout << width << " " << height << endl;
            cv::Rect r(left, top, right - left, bottom - top);





            if(divisionRegionIdentified) {
                r=r&dividedTextRegion;
            }
            {
                std::shared_ptr<cv::Mat> theImage = getIterationOutputImage("inputs");
                cv::Scalar randomColor(255,0,0);// = randomColors[((unsigned int) rng) % 5];
             //   rectangle(*theImage, r, randomColor, 3, 8, 0);
            }

//        cout << "R=" << r << endl;

            std::regex hasAnAlphaNumericCharacter(".*([0-9-]|[A-Z]|[a-z]).*");

            std::vector<TextualData> croppedTextualData;
            std::for_each(text.begin(), text.end(), [&](TextualData &d) {
                if ((r & d.getRect()).area() != 0 &&std::regex_match(d.getText(),hasAnAlphaNumericCharacter)) {
//                cout << d.getText() << endl;
                    croppedTextualData.push_back(d);
                    std::cout<<"Cropped : "<<d.getText()<<std::endl;
                }
            });

            std::cout<<"Cropped size: "<<croppedTextualData.size()<<std::endl;

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
            std::for_each(dataX.begin(), dataX.end(), [&](std::pair<TextualData, std::pair<int, int>> beta) {
                std::cout<<beta.first.getText()<<": ("<<beta.second.first<<","<<beta.second.second<<")"<<std::endl;
            });

            std::for_each(tModel->getStartIterator(), tModel->getEndIterator(),
                          [&](std::pair<std::string, std::shared_ptr<TreeFormNodeInterface>> alpha) {
                              if (std::dynamic_pointer_cast<TextTreeFormNode>(alpha.second) != nullptr) {
                                  std::vector<std::string> splits = HelperMethods::regexSplit(
                                          alpha.first.substr(1, alpha.first.length() - 2), ",");
                                  int x = std::stoi(splits[0]);
                                  int y = std::stoi(splits[1]);

                                  int pos = findTextWithMinimumEditDistance(croppedTextualData,
                                                                            std::dynamic_pointer_cast<TextTreeFormNode>(
                                                                                    alpha.second)->getText());
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
                        nx2->setData(nx2->getData() + alpha.first.getText());
                        nx2->setRegion(alpha.first.getRect());
                        nx2->setRegionDefined(true);


                        std::shared_ptr<cv::Mat>theImage=getIterationOutputImage("inputs");
                        cv::Scalar randomColor = randomColors[((unsigned int) rng) % 5];
                        rectangle(*theImage, nx2->getRegion(), randomColor, 3, 8, 0);
                        putText(*theImage, nx2->getData(), nx2->getRegion().br(), 1, 2, randomColor, 2);
                    }
                }

            });


//            tModel->data = dx;
            childrenDone=true;
        }
    }
    else if(iteration==4) {

        childrenDone=true;
    }

    return true;
}

void TreeFormNodeProcessor::setForm(const std::shared_ptr<RawFormInterface> &form) {
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

int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> text, std::string textToFind) {
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

int TreeFormNodeProcessor::findTextWithMinimumEditDistance(std::vector<TextualData> data, std::string textToFind,
                                                           cv::Rect region) {
    // TODO: FUNCTION PERFORMANCE OPTIMIZATION NEEDED
    int minDistance = 99999999;
    int minIndex = -1;
    for (int i = 0; i < text.size(); i++) {
        if((region&text[i].getRect()).area()==0)
            continue;

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

TreeFormNodeProcessor::TreeFormNodeProcessor(const cv::Mat &image, const std::vector<TextualData> &mergedWords,
                                             const std::shared_ptr<BasicTreeFormNode> &root) {
    TreeFormNodeProcessor::image=image;
    TreeFormNodeProcessor::text=mergedWords;
    TreeFormNodeProcessor::root=root;
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
