//
// Created by shahrukhqasim on 7/17/16.
//

#include "Processor.h"
#include "AccuracyProgram.h"
#include <functional>


Processor::Processor(const string &imageFilePath, const string &textFilePath, const string &groundTruthFilePath,
                     const string &outputFolder, const string &outputFileName) {
    this->imageFilePath = imageFilePath;
    this->textFilePath = textFilePath;
    this->groundTruthFilePath = groundTruthFilePath;
    this->outputFolder = outputFolder;
    this->outputFileName = outputFileName;
}


void Processor::execute() {
    readData();
    divideIntoParts();
    processPart1();
    processPart2();
    processPart3();
}

bool Processor::isBelow(const Rect &a, const Rect &b) {
    if (a.y > b.y + b.height) {
        return true;
    }
    return false;
}

bool Processor::isAbove(const Rect &a, const Rect &b) {
    if (a.y + a.height < b.y) {
        return true;
    }
    return false;
}


bool Processor::isLeftTo(const Rect &a, const Rect &b) {
    if (a.x + a.width < b.x) {
        return true;
    }
    return false;
}

bool Processor::isRightTo(const Rect &a, const Rect &b) {
    if (a.x > b.x + b.width) {
        return true;
    }
    return false;
}

void Processor::processPart1() {
    Mat image = this->image.clone();

    // TODO: Remove textual data
    TextualData vData1 = extractTextualDataType1("Partnership's employer identification number");
    TextualData vData2 = extractTextualDataType1("Partnership's name, address city state and ZIP code");
    TextualData vData3 = extractTextualDataType1("IRS Cener where partnership filed return");
    TextualData vData4 = extractTextualDataType1("Check if this is a publicly traded partnership (PTP)");

    vector<TextualData> part1Keys;
    part1Keys.push_back(vData1);
    part1Keys.push_back(vData2);
    part1Keys.push_back(vData3);
    part1Keys.push_back(vData4);

    // Extract first
    {
        string result="";
        // All that is below 1 and above 2
        for (int i = 0; i < part1Data.size(); i++) {
            TextualData d = part1Data[i];
            Rect r = d.getRect();
            if (isBelow(r, vData1.getRect()) && isAbove(r, vData2.getRect())) {
                if (!(d.getText() == "A" || d.getText() == "B")) {
                    rectangle(image, r, Scalar(0, 255, 255), 3, 8, 0);
                    result+=" "+d.getText();
                }
            }
        }
        cout<<"Partnerhip's employer identification number: "<<result<<endl;
    }

    // Extract second
    {
        string result="";
        // All that is below 2 and above 3
        for (int i = 0; i < part1Data.size(); i++) {
            TextualData d = part1Data[i];
            Rect r = d.getRect();
            if (isBelow(r, vData2.getRect()) && isAbove(r, vData3.getRect())) {
                if (!(d.getText() == "B" || d.getText() == "C")) {
                    rectangle(image, r, Scalar(255, 255, 0), 3, 8, 0);
                    result+=" "+d.getText();
                }
            }
        }
        cout<<"Partnerhip's name, address, city , state and ZIP code: "<<result<<endl;
    }

    // Extract third
    {
        string result="";
        // All that is below 3 and above 4
        for (int i = 0; i < part1Data.size(); i++) {
            TextualData d = part1Data[i];
            Rect r = d.getRect();
            if (isBelow(r, vData3.getRect()) && isAbove(r, vData4.getRect())) {
                if (!(d.getText() == "C" || d.getText() == "D")) {
                    rectangle(image, r, Scalar(255, 0, 255), 3, 8, 0);
                    result+=" "+d.getText();
                }
            }
        }
        cout<<"IRS Center where partnership filed return: "<<result<<endl;
    }

    // Extract fourth
    {
        // TODO: Check if length is not zero
        if (vData4.getText()[0] == 'X' || vData4.getText()[0] == 'x')
            cout << "Publicly traded partnership=true" << endl;
        else {
            bool found = false;
            for (int i = 0; i < part1Data.size(); i++) {
                TextualData d = part1Data[i];
                Rect r = d.getRect();
                if (isLeftTo(r, vData4.getRect()) && isBelow(r, vData3.getRect())) {
                    if (d.getText() == "X" || d.getText() == "x")
                        found = true;
                }
            }

            if (found) {
                cout << "Publicly traded partnership=false" << endl;
            }
            else {
                cout << "Publicly traded partnership=false" << endl;
            }
        }


    }

    drawBoxes(image, part1Keys, Scalar(0, 0, 255));


    // Output the file
    string fileName = outputFileName + "_fields1.png";
    imwrite(outputFolder + fileName, image);
}

string Processor::findTextWithRules(vector<std::function<bool(const TextualData&)>> rules, const vector<TextualData> &data) {

    string text="";
    for_each(data.begin(), data.end(), [&](const TextualData &currentData) {
        bool ruleMatched = true;
        for_each(rules.begin(), rules.end(), [&](std::function<bool(const TextualData&)> currentRule) {
            ruleMatched = ruleMatched & currentRule(currentData);
        });
        if(ruleMatched) {
            text+=" "+currentData.getText();
        }
    });

    return text;
}

string Processor::findTextWithRulesOnlyRightMost(vector<std::function<bool(const TextualData &)>> rules,
                                                 const vector<TextualData> &data) {

    bool init=false;
    TextualData text;
    for_each(data.begin(), data.end(), [&](const TextualData &currentData) {
        bool ruleMatched = true;
        for_each(rules.begin(), rules.end(), [&](std::function<bool(const TextualData&)> currentRule) {
            ruleMatched = ruleMatched & currentRule(currentData);
        });
        if(ruleMatched) {
            if(text.getRect().y>currentData.getRect().y||!init) {
                text = currentData;
                init=true;
            }
        }
    });

    return text.getText();
}

void Processor::processPart2() {
    vector<TextualData> fieldKeys;

    part2Data.erase(std::remove_if(part2Data.begin(), part2Data.end(), [] (const TextualData& d) -> bool{
        string t=d.getText();

        if(t=="E"||t=="F"||t=="G"||t=="H"||t=="I1"||t=="I2"||t=="J"||t=="K"||t=="L"||t=="M")
            return true;
        else
            return false;

    }), part2Data.end());

    fieldKeys.push_back(extractTextualDataType2("Partner's identifying number")); // 0
    fieldKeys.push_back(extractTextualDataType2("Partner's name, address, city, state and ZIP code")); // 1
    fieldKeys.push_back(extractTextualDataType2("General partner or LLC")); // 2
    fieldKeys.push_back(extractTextualDataType2("Limited partner or other LLC")); // 3
    fieldKeys.push_back(extractTextualDataType2("Domestic partner")); // 4
    fieldKeys.push_back(extractTextualDataType2("Foreign partner")); // 5
    fieldKeys.push_back(extractTextualDataType2("What type of entity is this partner")); // 6
    fieldKeys.push_back(
            extractTextualDataType2("If this partner is a retirement plan (IRA/SEP/Keogh/etc.), check here")); // 7
    fieldKeys.push_back(extractTextualDataType2("Partner's share of profit, loss and capital (see instructions):")); // 8
    fieldKeys.push_back(extractTextualDataType2("Beginning")); // 9
    fieldKeys.push_back(extractTextualDataType2("Ending")); // 10
    fieldKeys.push_back(extractTextualDataType2("Profit")); // 11
    fieldKeys.push_back(extractTextualDataType2("Loss")); // 12
    fieldKeys.push_back(extractTextualDataType2("Capital")); // 13
    fieldKeys.push_back(extractTextualDataType2("Partner's share of liabilities at year end")); // 14
    fieldKeys.push_back(extractTextualDataType2("Nonrecourse")); // 15
    fieldKeys.push_back(extractTextualDataType2("Qualified nonrecourse financing")); // 16
    fieldKeys.push_back(extractTextualDataType2("Recourse")); // 17
    fieldKeys.push_back(extractTextualDataType2("Partner's capital account analysis")); // 18
    fieldKeys.push_back(extractTextualDataType2("Beginning capital account")); // 19
    fieldKeys.push_back(extractTextualDataType2("Capital contributed during the year")); // 20
    fieldKeys.push_back(extractTextualDataType2("Current year increase (decrease)")); // 21
    fieldKeys.push_back(extractTextualDataType2("Withdrawals & distributions")); // 22
    fieldKeys.push_back(extractTextualDataType2("Ending capital account")); // 23
    fieldKeys.push_back(extractTextualDataType2("Tax basis")); // 24
    fieldKeys.push_back(extractTextualDataType2("GAAP")); // 25
    fieldKeys.push_back(extractTextualDataType2("Section 704(b) book")); // 26
    fieldKeys.push_back(extractTextualDataType2("Other (explain)")); // 27
    fieldKeys.push_back(extractTextualDataType2("Did the partner contribute propertly with a built-in grain or loss?")); // 28
    fieldKeys.push_back(extractTextualDataType2("Yes")); // 29
    fieldKeys.push_back(extractTextualDataType2("No")); // 30
    fieldKeys.push_back(extractTextualDataType2("If \"Yes,\" attach statement (see instructions)")); // 31

    Mat image=this->image.clone();



    drawBoxes(image,fieldKeys,Scalar(255,0,0));


    // To find Partner's identifying number
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[0].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[1].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Partner's identifying number: "<<result<<endl;

    }

    // Partner's name, address, city, state and ZIP code
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[1].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[2].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Partner's name, address, city, state and ZIP code "<<result<<endl;

    }

    // What type of entity is the partner?
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[5].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[6].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[7].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"What type of entity is the partner?: "<<result<<endl;
    }


    // Partner profit beginning
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[9].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[11].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[10].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Partner profit beginning "<<result<<endl;
    }

    // Partner loss beginning
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[11].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[10].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Partner loss beginning "<<result<<endl;
    }

    // Partner capital beginning
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[10].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Partner capital beginning "<<result<<endl;
    }


    // Partner profit ending
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[9].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[11].getRect());
        });

        string result=findTextWithRulesOnlyRightMost(rules,part2Data);
        cout<<"Partner profit ending "<<result<<endl;
    }

    // Partner loss ending
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[11].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[12].getRect());
        });

        string result=findTextWithRulesOnlyRightMost(rules,part2Data);
        cout<<"Partner loss ending "<<result<<endl;
    }

    // Partner capital ending
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[13].getRect());
        });

        string result=findTextWithRulesOnlyRightMost(rules,part2Data);
        cout<<"Partner capital ending "<<result<<endl;
    }

    // Nonrecourse
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[16].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[15].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Nonrecourse: "<<result<<endl;
    }

    // Qualified nonrecourse financing
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[15].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[17].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[16].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Qualified nonrecourse financing: "<<result<<endl;
    }

    // Recourse
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[16].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[18].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[17].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Recourse: "<<result<<endl;
    }

    // Beginning capital account
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[18].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[19].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Beginning capital account: "<<result<<endl;
    }

    // Capital contributed during the year
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[21].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[20].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Capital contributed during the year: "<<result<<endl;
    }

    // Current year increase (decrease)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[22].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[21].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Current year increase (decrease): "<<result<<endl;
    }


    // Withdrawal and distributions
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[21].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[23].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[22].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Withdrawal and distributions: "<<result<<endl;
    }

    // Ending capital account
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[22].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[24].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[23].getRect());
        });

        string result=findTextWithRules(rules,part2Data);
        cout<<"Ending capital account: "<<result<<endl;
    }


    // Output the file
    string fileName = outputFileName + "_fields2.png";
    imwrite(outputFolder + fileName, image);


}

void Processor::processPart3() {
    vector<TextualData> fieldKeys;

    part3Data.erase(std::remove_if(part3Data.begin(), part3Data.end(), [] (const TextualData& d) -> bool{
        string t=d.getText();

        if(t=="1"||t=="2"||t=="3"||t=="4"||t=="5"||t=="6a"||t=="6b"||t=="7"||t=="8"||t=="9a"||t=="9b"||t=="9c"||t=="10"||t=="11"||t=="12"||t=="13"||t=="14"||t=="15"||t=="16"||t=="17"||t=="18"||t=="19"||t=="20")
            return true;
        else
            return false;

    }), part3Data.end());

    fieldKeys.push_back(extractTextualDataType3("Ordinary business income (loss)")); // 0
    fieldKeys.push_back(extractTextualDataType3("Net rental real estate income (loss)")); // 1
    fieldKeys.push_back(extractTextualDataType3("Other net rental income (loss)")); // 2
    fieldKeys.push_back(extractTextualDataType3("Guaranteed payments")); // 3
    fieldKeys.push_back(extractTextualDataType3("Interest income")); // 4
    fieldKeys.push_back(extractTextualDataType3("Ordinary dividends")); // 5
    fieldKeys.push_back(extractTextualDataType3("Qualified dividends")); // 6
    fieldKeys.push_back(
            extractTextualDataType3("Royalties")); // 7
    fieldKeys.push_back(extractTextualDataType3("Net short-term capital gain (loss)")); // 8
    fieldKeys.push_back(extractTextualDataType3("Net long-term capital gain (loss)")); // 9
    fieldKeys.push_back(extractTextualDataType3("Collectibles (28%) gain (loss)")); // 10
    fieldKeys.push_back(extractTextualDataType3("Unrecaptured sectoin 1250 gain")); // 11
    fieldKeys.push_back(extractTextualDataType3("Net section 1231 gain (loss)")); // 12
    fieldKeys.push_back(extractTextualDataType3("Other income (loss)")); // 13
    fieldKeys.push_back(extractTextualDataType3("Secion 179 deduction")); // 14
    fieldKeys.push_back(extractTextualDataType3("Other deductions")); // 15
    fieldKeys.push_back(extractTextualDataType3("Self employment earnings (loss)")); // 16
    fieldKeys.push_back(extractTextualDataType3("See attached statement for additional information")); // 17
    fieldKeys.push_back(extractTextualDataType3("Credits")); // 18
    fieldKeys.push_back(extractTextualDataType3("Foreign transactions")); // 19
    fieldKeys.push_back(extractTextualDataType3("Alternative minimum tax (AMT) items")); // 20
    fieldKeys.push_back(extractTextualDataType3("Tax-exempt income and")); // 21
    fieldKeys.push_back(extractTextualDataType3("nondeductable expenses")); // 22
    fieldKeys.push_back(extractTextualDataType3("Distributions")); // 23
    fieldKeys.push_back(extractTextualDataType3("Other information")); // 24

    Mat image=this->image.clone();

    Mat vProjection(1, image.cols-xDivisionCoordinate, CV_16U);
    vProjection = 0;
    for (int i = 0; i < mergedWords.size(); i++) {
        Rect r = mergedWords[i].getRect();
        for (int j = r.x; j < r.x + r.width; j++) {
            if(j-xDivisionCoordinate>0)
                vProjection.at<short>(0, j - xDivisionCoordinate) += r.height;
        }
    }

    int quarter = vProjection.cols / 5;

    int shallowProjectionIndex = quarter*2;
    for (int i = quarter * 2; i < quarter * 3; i++) {
        if (vProjection.at<short>(0, i) < vProjection.at<short>(0, shallowProjectionIndex))
            shallowProjectionIndex = i;
    }

    int part3XSeperation=shallowProjectionIndex+xDivisionCoordinate;

    // Draw the seperation line on the image
    line(image,Point(part3XSeperation,0),Point(part3XSeperation,image.rows-1),Scalar(0,0,255),7,8,0);
    line(image,Point(xDivisionCoordinate,0),Point(xDivisionCoordinate,image.rows-1),Scalar(0,0,255),7,8,0);


    drawBoxes(image,fieldKeys,Scalar(0,255,0));



    // Ordinary business income (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[0].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[1].getRect());
        });
//        rules.push_back([&] (const TextualData &d) -> bool {
//            return isLeftTo(d.getRect(),fieldKeys[18].getRect());
//        });

        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });



        string result=findTextWithRules(rules,part3Data);
        cout<<"Ordinary business income (loss): "<<result<<endl;

    }

    // Net rental real estate income (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[1].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[2].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Net rental real estate income (loss): "<<result<<endl;
    }

    // Other net rental income (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[2].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[3].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Other net rental income (loss): "<<result<<endl;
    }

    // Guaranteed payments
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[3].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[4].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Guaranteed payments: "<<result<<endl;

    }

    // Interest income
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[4].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[5].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Interest income: "<<result<<endl;
    }

    // Ordinary dividends
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[5].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[6].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Ordinary dividends: "<<result<<endl;
    }

    // Qualified dividends
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[6].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[7].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Qualified dividends: "<<result<<endl;
    }

    // Royalties
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[7].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[8].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Royalties: "<<result<<endl;
    }

    // Net short-term capital gain (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[8].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[9].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Net short-term capital gain (loss): "<<result<<endl;
    }

    // Net long-term capital gain (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[9].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[10].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Net long-term capital gain (loss): "<<result<<endl;
    }

    // Collectibles (28%) gain (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[10].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[11].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Collectibles (28%) gain (loss): "<<result<<endl;
    }

    // Unrecaptured section 1250 gain
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[11].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Unrecaptured sectoin 1250 gain: "<<result<<endl;
    }

    // Net section 1231 gain (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[12].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[21].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Net section 1231 gain (loss): "<<result<<endl;
    }

    // Other income (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[23].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Other income (loss): "<<result<<endl;
    }

    // Section 179 deduction
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[15].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[23].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Section 179 deduction: "<<result<<endl;
    }

    // Other deductions
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[15].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[16].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[24].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Other deductions: "<<result<<endl;
    }

    // Self employment earnings (loss)
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[16].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[17].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isLeftTo(d.getRect(),fieldKeys[24].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x<part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Self employment earnings (loss): "<<result<<endl;
    }

    // Credits
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[18].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[0].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Credits: "<<result<<endl;
    }

    // Foreign transactions
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[19].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[3].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Foreign transactions: "<<result<<endl;
    }

    // Alternative minimum tax (AMT) items
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[20].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[21].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[10].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Alternative minimum tax (AMT) items: "<<result<<endl;
    }

    // Tax-exempt income and
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[21].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[23].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[13].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Tax-exempt income and: "<<result<<endl;
    }

    // Distribution
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[23].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[24].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Distribution: "<<result<<endl;
    }

    // Other information
    {
        vector<std::function<bool(const TextualData&)>> rules;

        rules.push_back([&] (const TextualData &d) -> bool {
            return isBelow(d.getRect(),fieldKeys[24].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isAbove(d.getRect(),fieldKeys[17].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return isRightTo(d.getRect(),fieldKeys[14].getRect());
        });
        rules.push_back([&] (const TextualData &d) -> bool {
            return d.getRect().x>=part3XSeperation;
        });

        string result=findTextWithRules(rules,part3Data);
        cout<<"Other information: "<<result<<endl;
    }


    // Output the file
    string fileName = outputFileName + "_fields3.png";
    imwrite(outputFolder + fileName, image);
}

TextualData Processor::extractTextualDataType1(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part1Data, key);
    TextualData d = part1Data[index];
    part1Data.erase(part1Data.begin() + index);
    return d;
}

TextualData Processor::extractTextualDataType2(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part2Data, key);
    TextualData d = part2Data[index];
    part2Data.erase(part2Data.begin() + index);
    return d;
}

TextualData Processor::extractTextualDataType3(string key) {
    // TODO: Add 'A' etc to it as well
    int index = findMinTextIndex(part3Data, key);
    TextualData d = part3Data[index];
    part3Data.erase(part3Data.begin() + index);
    return d;
}


void Processor::divideIntoParts() {
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
        line(image,Point(i,0),Point(i,vProjection.at<short>(0,i)),Scalar(255,0,0));
    }

    int quarter = vProjection.cols / 6;

    int shallowProjectionIndex = quarter;
    for (int i = quarter * 2; i < quarter * 4; i++) {
        if (vProjection.at<short>(0, i) < vProjection.at<short>(0, shallowProjectionIndex))
            shallowProjectionIndex = i;
    }

    xDivisionCoordinate=shallowProjectionIndex;

    // Draw the separation line
    line(image,Point(shallowProjectionIndex,0),Point(shallowProjectionIndex,image.rows-1),Scalar(0,255,0),7,8,0);

    vector<TextualData> leftBoxes;
    vector<TextualData> rightBoxes;

    for (int i = 0; i < mergedWords.size(); i++) {
        TextualData box = mergedWords[i];
        if (shallowProjectionIndex - box.getRect().x > box.getRect().width / 2) {
            leftBoxes.push_back(box);
        }
        else {
            rightBoxes.push_back(box);
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
        }

        // Search in right boxes which are below indexPart3: Add them to part 3
        for (int i = 0; i < rightBoxes.size(); i++) {
            TextualData current = rightBoxes[i];
            if (current.getRect().y >= part3TextualData.getRect().y) {
                part3Data.push_back(current);
                part3Rect = part2Rect | current.getRect();
            }
        }

        drawBoxes(image, part1Data, Scalar(255, 0, 0));
        drawBoxes(image, part2Data, Scalar(0, 255, 0));
        drawBoxes(image, part3Data, Scalar(0, 0, 255));

        // \u00e2\u0096\u00a1
        // Plot checkboxes here TODO remove it later
//        for_each(mergedWords.begin(),mergedWords.end(),[&] (const TextualData&w){
//            if(strstr(w.getText().c_str(),"\u00e2\u0096\u00a1")!= nullptr) {
//                rectangle(image, w.getRect(), Scalar(0,0,255), 5, 8, 0);
//            }
//        });
    }

    // Output the file
    string fileName = outputFileName + "_checks.png";
    imwrite(outputFolder + fileName, image);

}

void Processor::getFieldValues(Json::Value root, vector<TextualData> &outputVector) {

    root = root["Pages"][0];

    Json::Value words = root["Fields"];
    for (int i = 0; i < words.size(); i++) {
        cout << "I am here" << endl;
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

void Processor::readData() {
    // Read the input image
    image = imread(imageFilePath, 1);
    if (!image.data)
        cerr << "Error reading form";

    // Read the OCR result
    Json::Value jsonWords;
    ifstream jsonOcrWordsStream(textFilePath);
    jsonOcrWordsStream >> jsonWords;
    AccuracyProgram::getWords(jsonWords, words);


    // Read the ground truth field values
    Json::Value jsonFields;
    ifstream jsonFieldsStream(groundTruthFilePath);
    jsonFieldsStream >> jsonFields;
    AccuracyProgram::getWords(jsonFields, fields);
}

void Processor::mergeWordBoxes(const vector<TextualData> &words, vector<TextualData> &elemBoxes) {
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
            int hGapThresh = min(currWord.getRect().height, prevWord.getRect().height);
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

int Processor::findMinTextIndex(const vector<TextualData> &data, const string &textToFind) {
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

void Processor::drawBoxes(Mat &image, const vector<TextualData> &data, const Scalar &color) {
    for (int i = 0; i < data.size(); i++) {
        rectangle(image, data[i].getRect(), color, 3, 8, 0);
    }
}

void Processor::runProcessorProgram(string parentPath) {
    if (parentPath[parentPath.length() - 1] != '/')
        parentPath = parentPath + "/";

    cout << "Running on " << parentPath << endl;

    fstream streamImageFilesList(parentPath + "images/files.txt");
    fstream streamJsonFilesList(parentPath + "text/files.txt");
    fstream streamGroundTruthFilesList(parentPath + "groundTruth/files.txt");

    string imageFile;
    string jsonFile;
    string groundTruthFile;

    string outputFolder = parentPath + "output/";

    while (getline(streamImageFilesList, imageFile)) {
        getline(streamJsonFilesList, jsonFile);
        getline(streamGroundTruthFilesList, groundTruthFile);

        string workFile = HelperMethods::removeFileExtension(imageFile);

        imageFile = parentPath + "images/" + imageFile;
        jsonFile = parentPath + "text/" + jsonFile;
        groundTruthFile = parentPath + "groundTruth/" + groundTruthFile;

        cout << imageFile << endl;

        Processor(imageFile, jsonFile, groundTruthFile, outputFolder, workFile).execute();
        cout<<endl;

//        run(imageFile, jsonFile, directory + "/output/", workFile);

    }

}