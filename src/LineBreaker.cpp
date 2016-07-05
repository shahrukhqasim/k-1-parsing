//
// Created by Talha Rehman on 7/1/16.
//

#include "LineBreaker.h"



void LineBreaker::projectionProfile(Mat& input, vector<int>& P){
    for(int i=0; i<input.rows; i++){
        for(int j=0; j<input.cols; j++){

            if(input.at<int>(i,j)==0){
                P[i]++;

            }
        }
    }
}




double LineBreaker::calculateMean(vector<int>value) {
    double sum = 0;
    for(int i = 0; i < value.size(); i++)
        sum += value[i];
    return (sum / value.size());
}



//crop bounding boxes from parent image and copy them into new image
void LineBreaker::cropImage(Mat& input, Mat& output, vector<Rect>& rboxes){
    for(int i=0; i<rboxes.size(); i++){
        for(int k=0;k<rboxes[i].height;k++) {
            for(int l=0;l<rboxes[i].width;l++) {
                output.at<uchar>(rboxes[i].y+l,rboxes[i].x+k)=input.at<uchar>(rboxes[i].y+l,rboxes[i].x+k);
            }
        }
    }
}



void LineBreaker::distanceBwBoxes(vector<Rect>& rboxes, vector<int>& dist){

    for(int i=0; i<rboxes.size()-1; i++) {

        dist.push_back( rboxes[i+1].x - rboxes[i ].x -rboxes[i].width);

    }

}




void LineBreaker::cleanBoxes(vector<Rect>& rboxes, vector<int>& areaofCC){

    int Tarea=100;

    vector<Rect> rboxes_1;
    vector<int> areaofCC_1;


    for(int i=0; i<areaofCC.size(); i++){

        if(areaofCC[i]> Tarea){

            areaofCC_1.push_back(areaofCC[i]);
            rboxes_1.push_back(rboxes[i]);

        }

    }

    rboxes=rboxes_1;
    areaofCC=areaofCC_1;



}



void LineBreaker::sortBoxes(vector<Rect>& output){
    //implements algo
    for (int i = output.size() - 1; i > 0; i--){
        for (int j = 0; j <= i - 1; j++){
            if (output[j].x > output[j + 1].x)
                swap(output[j], output[j + 1]);
        }
    }

}






void LineBreaker::mergeBoxes(vector<Rect>& rboxes, vector<int>& dist ){
    float meanDistance=calculateMean(dist);
//    cout<<meanDistance;
    vector<Rect> words=rboxes;
    Rect temp;

    for(int i=0; i<dist.size(); ){


        if(dist[i]< 1.1*meanDistance ){

            temp=Rect(words[i].x,min(words[i].y,words[i+1].y),(words[i].width+words[i+1].width)+meanDistance, max(words[i].height,words[i+1].height)),
                    words.erase(words.begin()+i);
            dist.erase(dist.begin()+i);
            words[i]=temp;
        }
        else
            i++;
    }

    rboxes=words;

}

void LineBreaker::conCompFast(cv::Mat &img, std::vector<cv::Rect> &rboxes, int type,std::vector<int>& areaofConComp){

    cv::Mat labelImg = cv::Mat::zeros(img.rows, img.cols, CV_64F);
    int label = 0;
    CUnionFind *uf = new CUnionFind(img.rows*img.cols);
    int l1, l2;
    for (int y=0, Y=img.rows; y<Y; y++){
        for (int x=0, X=img.cols ; x<X; x++){
            if (img.at<uchar>(y,x)==0){
                // adapt from left neighbor
                if (x>0 && img.at<uchar>(y,x-1)==0){
                    labelImg.at<double>(y,x)=label;
                } else {
                    labelImg.at<double>(y,x)=++label;
                } // top neighbor
                if (y>0 && labelImg.at<double>(y-1,x)!=0){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x>0 && labelImg.at<double>(y-1,x-1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x-1));
                    if (l1 != l2) uf->set(l1,l2);
                }
                else if (y>0 && x<X-1 && labelImg.at<double>(y-1,x+1)!=0 && type==8){
                    l1 = uf->find(label);
                    l2 = uf->find(labelImg.at<double>(y-1,x+1));
                    if (l1 != l2) uf->set(l1,l2);
                }
            }
        }
    }



    for (int y=0; y<img.rows; y++)
        for (int x=0; x<img.cols; x++)
            labelImg.at<double>(y,x) = uf->find(labelImg.at<double>(y,x));



//    cout<<label-1<<endl;
    if(label!=0) {
        for(int i=0;i<label-1;i++)
            areaofConComp.push_back(0);
        //areaofConComp = std::vector<int>(label - 1);
    }



    // Init Bboxes and Seeds
    std::vector<cv::Rect> bb(label+1, cv::Rect());
    std::vector<bool> empty(label+1, true);

    vector<int> vec;


    int l;
    cv::Rect b;
    for (int y=0, Y=img.rows; y<Y; y++){
        for (int x=0, X=img.cols; x<X; x++){
            if (img.at<uchar>(y,x) == 0){
                l = uf->find(labelImg.at<double>(y,x));
                //calculate area of cc
                if(label!=0)
                    areaofConComp[l]++;
                cv::Rect pt(x,y,1,1);
                if(empty[l]){
                    bb[l] = pt;

                    empty[l] = false;
                } else {
                    bb[l] |= pt;

                }

            }
        }
    }


    cv::Rect imgDim(0,0,img.cols-1,img.rows-1);
    for (int i=1; i<=label; i++){
        cv::Rect rc = bb[i];
        rc &= imgDim;
        rboxes.push_back(rc);
    }

    //rboxes bounding around each cc
    //areaofConCompComp


}

void LineBreaker::run() {
    rboxes.clear();

    //rescaling the image
    float m_ReceiptWidth=1200;
    int imWidth = img.cols;
    Mat rescaledImg;
    double scale = m_ReceiptWidth / imWidth;
    resize(img, rescaledImg, Size(), scale, scale, INTER_LANCZOS4);
    img=rescaledImg;

    //-------Binarization--------------------------
    Mat imgMATgray, imgMATbin;

//    Preprocessor::binarizeShafait(imgMATgray, imgMATbin, 30, 0.05);
    //--------connected componenets----------------
    vector<int> areaofCC;
    vector<int> dist;
    //area of cc

//    cout<<rboxes.size()<<endl;
    conCompFast(img, rboxes,  8,areaofCC);

    if(rboxes.size()>=2) {
        areaofCC.erase (areaofCC.begin());
//    cleanBoxes(rboxes, areaofCC);

        sortBoxes(rboxes);
        distanceBwBoxes(rboxes, dist);


        mergeBoxes(rboxes, dist);
    }
}

LineBreaker::LineBreaker(const Mat &image) {
    this->img=image.clone();
}

void LineBreaker::getResult(vector<Rect> &result) {
    result.insert(result.end(),rboxes.begin(),rboxes.end());
}