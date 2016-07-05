//
// Created by shahrukhqasim on 6/24/16.
//

void convertPdfs() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/files.txt");
    std::string str;
    while (getline(inputStream, str)) {
        std::string withoutExtension=HelperMethods::removeFileExtension(str);

        std::string inputFileName="\"/home/shahrukhqasim/Desktop/bb/All/";
        inputFileName+=str;
        inputFileName+='"';

        std::string outputFileName="\"/home/shahrukhqasim/Desktop/bb/All/";
        outputFileName+=withoutExtension;
        outputFileName+=".png\"";
        std::string command="convert -density 300 "+inputFileName;
        command+=" -quality 100 ";
        command+=outputFileName;
        system(command.c_str());
    }
}

void runBatchJob() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/programInput/files.txt");
    std::string str;

    string inputFolder="/home/shahrukhqasim/Desktop/bb/All/programInput";
    string outputFolder="/home/shahrukhqasim/Desktop/bb/All/programOutput";
    while (getline(inputStream, str)) {
        cout<<"Running on "<<str<<endl;
        Program runner(str,inputFolder,outputFolder);
        runner.run();
        cout<<"Run complete on "<<str<<endl;
    }
}

void temp() {
    vector<OcrResult>data3;

    // Remove leading and trailing whitespaces
    for(OcrResult& result : data) {
        result.text=regex_replace(result.text,regex("(^\\s+)|(\\s|\\\\n)+$"), "");
        vector<string>elements=HelperMethods::regexSplit(result.text);
        if(elements.size()>1) {
            double valueSum=0;
            int size=0;
            Mat vector(1,result.p2.x-result.p1.x+1,CV_8UC1);
            for(int i=result.p1.x;i<=result.p2.x;i++) {
                int value;
                for(int j=result.p1.y;j<result.p2.y;j++) {
                    if(cleanedImage.at<uchar>(i,j)==0)
                        value++;
                }
                if(valueSum!=0) {
                    valueSum += value;
                    size++;
                }
                vector.at<uchar>(1,i)=value;
            }
            double average=valueSum/size;

            bool current=true;
            vector<Point>whitePoints;
            Point lastPoint;
            for(int i=0;i<vector.cols;i++) {
                bool on=vector.at<uchar>(1,i)>average/3;
                if(on!=current) {
                    if(current) {
                        lastPoint.x=i+result.p1.x;
                    }
                    else {
                        lastPoint.y=i+result.p1.x;
                    }
                    current=on;
                }
            }
            if(whitePoints.size()+1>=elements.size()) {
                sort(whitePoints.begin(), whitePoints.end(), greater<SmallDistance>());
                whitePoints.erase(whitePoints.begin()+elements.size()-1,whitePoints.end());
                sort(whitePoints.begin(), whitePoints.end(), greater<XFirst>());

                vector<OcrResult>splitWords;
                int start=result.p1.x;
                for(int i=0;i<elements.size();i++) {
                    splitWords.push_back({Point(start,result.p1.y),Point(),""});
                }
            }
        }
        else {
            data3.push_back(result);
        }
    }
}

void OcrProgram::runOcr() {
    // First run OCR on the segments
    TesseractFinder finder1(outputFolder+"/"+cleanedImageFileName,segments);
    finder1.setIteratorLevel(tesseract::RIL_WORD);
    finder1.run();

    Mat imageWithoutSegments=cleanedImage.clone();

    // Then remove the segments and run OCR again
    Scalar color(255, 255, 255);
    for (int i = 0; i < segments.size(); i++) {
        Rect box = segments[i];
        rectangle(imageWithoutSegments, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), color, CV_FILLED, 8,0);
    }

    string withoutSegmentsImageFileName=HelperMethods::removeFileExtension(inputFileName)+"_withoutSegments.png";
    HelperMethods::outputImage(imageWithoutSegments,outputFolder+"/"+withoutSegmentsImageFileName);

    TesseractFinder finder2(outputFolder+"/"+withoutSegmentsImageFileName);
    finder1.setIteratorLevel(tesseract::RIL_WORD);
    finder2.run();

    // Now merge the results of both runs
    data=finder1.getRecognizedData();
    vector<OcrResult>data2=finder2.getRecognizedData();
    data.insert(data.end(), data2.begin(), data2.end());


    vector<OcrResult> data3;
    // Remove leading and trailing whitespaces
    for(OcrResult& result : data) {
        result.text = regex_replace(result.text, regex("(^\\s+)|(\\s|\\\\n)+$"), "");
        vector<string>elements=HelperMethods::regexSplit(result.text);
        if(elements.size()>1) {
//            cout<<Rect(result.p1,result.p2)<<endl;
            Mat subImage=cleanedImage(Rect(result.p1,result.p2)).clone();

            LineBreaker segmentor(subImage);
            segmentor.run();
            vector<Rect> subRectangles;
            segmentor.getResult(subRectangles);
            cout<<subRectangles.size()<<" "<<elements.size()<<endl;
            for(int i=0;i<elements.size()<subRectangles.size()?elements.size():subRectangles.size();i++) {
                OcrResult subResult;
                subResult.text=elements[i];
                subResult.p1=subRectangles[i].tl() + result.p1;
                subResult.p2=subRectangles[i].br() + result.p1;
                data3.push_back(subResult);
            }
        }
        else {
            data3.push_back(result);
        }
    }
    data=data3;
}


void drawRectangles() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/bb/All/expectedOutput2/files.txt");

    std::string str;

    // TODO: To run extensive testing on single image
    while(getline(inputStream, str)) {
        cout<<"Running on "<<str<<endl;
        string withoutExtension=HelperMethods::removeFileExtension(str);
        string imageFile="/home/shahrukhqasim/Desktop/bb/All/programInput/"+withoutExtension+".png";
        Mat a=imread(imageFile,1);
        ifstream jsonStream("/home/shahrukhqasim/Desktop/bb/All/expectedOutput2/"+str);
        Json::Value root;
        jsonStream>>root;
        Scalar blue(255,0,0);
        for(int i=0;i<root.size();i++) {
            Json::Value word=root[i];
            string value = word["word"].asString();

            float t=stof(word["ymin"].asString());
            float l=stof(word["xmin"].asString());
            float b=stof(word["ymax"].asString());
            float r=stof(word["xmax"].asString());


            cout<<t<<" "<<b<<" "<<l<<" "<<r<<endl;
            t=t*a.rows/792;
            b=b*a.rows/792;
            l=l*a.cols/612;
            r=r*a.cols/612;

            cout<<t<<" "<<b<<" "<<l<<" "<<r<<endl;

            WordEntry entry;
//            entry.setRect(Rect(200.1,200.2,400.3,400.4));
            entry.setRect(Rect(l,t,r-l,b-t));
            entry.setString(value);

            rectangle(a,entry.getRect(),blue,3,8,0);
        }

        HelperMethods::outputImage(a,"/home/shahrukhqasim/Desktop/bb/All/plottedDataGiven2/"+withoutExtension+".png");

        cout<<"Run complete on "<<str<<endl;
    }
}

void toobaTest() {
    ifstream inputStream("/home/shahrukhqasim/Desktop/tooba/programInput/files.txt");
    ifstream expectedJsonStream("/home/shahrukhqasim/Desktop/tooba/expectedOutput/files.txt");
    ifstream toobaFiles("/home/shahrukhqasim/Desktop/tooba/programOutput/files.txt");

    std::string str;
    std::string str2;
    std::string str3;
    double accuracySum=0;
    int quantities=0;

    // TODO: To run extensive testing on single image
    while(getline(inputStream, str)) {
        getline(expectedJsonStream, str2);
        getline(toobaFiles, str3);
        cout<<"Running on "<<str<<endl;

        try {
            string programFilePath = "/home/shahrukhqasim/Desktop/tooba/programOutput/" + str3;
            string expectedFilePath = "/home/shahrukhqasim/Desktop/tooba/expectedOutput/" + str2;
            string inputImagePath = "/home/shahrukhqasim/Desktop/tooba/programInput/" + str;
            string plotImagePath = "/home/shahrukhqasim/Desktop/tooba/plottedDataComparison/" + str;
//            cout << programFilePath << endl;
//            cout << expectedFilePath << endl;
//            cout << inputImagePath << endl;
//            cout << plotImagePath << endl;
            float newAccuracy = AccuracyProgram(programFilePath, expectedFilePath, inputImagePath, plotImagePath).run();

            if (newAccuracy != 0) {
                accuracySum += newAccuracy;
                quantities++;
            }
            else {
                cout << "Error in " << str << endl;
            }
        }
        catch(...) {
            cout << "Error in " << str << endl;
        }

        cout<<"Run complete on "<<str<<endl;
    }

    cout<<endl;
    cout<<"Average accuracy is: "<<accuracySum/quantities<<endl;
    cout<<"Receipts run: "<<quantities<<endl;
}

void compareThem() {
    string folderA="/home/shahrukhqasim/Desktop/compareBoth/cleaned/";
    string folderB="/home/shahrukhqasim/Desktop/compareBoth/segmented/";
    string folderInput="/home/shahrukhqasim/Desktop/compareBoth/programInput/";
    string folderOutput="/home/shahrukhqasim/Desktop/compareBoth/difference/";


    ifstream streamA("/home/shahrukhqasim/Desktop/compareBoth/cleaned/jsons.txt");

    Scalar colorRed(0,0,255);
    Scalar colorBlue(255,0,0);

    std::string stringA;
    while(getline(streamA, stringA)) {
        vector<WordEntry> vectorA;
        vector<WordEntry> vectorB;
        string onlyName=HelperMethods::removeFileExtension(stringA);
        Json::Value valuesA;
        ifstream streamValuesA(folderA+stringA);
        streamValuesA>>valuesA;
        Json::Value valuesB;
        ifstream streamValuesB(folderB+stringA);
        streamValuesB>>valuesB;

        AccuracyProgram::getWords(valuesA,vectorA);
        AccuracyProgram::getWords(valuesB,vectorB);

        Mat imageA=imread(folderInput+onlyName+".png",0);
        Mat image;
        cvtColor(imageA,image,CV_GRAY2RGB);

        for(int i=0;i<vectorA.size();i++) {
            WordEntry e=vectorA[i];
            rectangle(image,e.getRect(),colorRed,3,8,0);
        }
        for(int i=0;i<vectorB.size();i++) {
            WordEntry e=vectorB[i];
            rectangle(image,e.getRect(),colorBlue,3,8,0);
        }

        HelperMethods::outputImage(image,folderOutput+onlyName+".png");
    }
}