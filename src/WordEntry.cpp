//
// Created by shahrukhqasim on 6/24/16.
//

#include "WordEntry.h"

bool WordEntry::compare(WordEntry &expectedOutput){

    int matched=0;

    // Jc = (Box1 intersection Box2)/(Box1 union Box2)

    float Jc =float((this->boundingBox & expectedOutput.getRect()).area())/( this->boundingBox | expectedOutput.getRect() ).area() ;


    // TODO: Fix this
    if(Jc>0.1){
        if(HelperMethods::nearEqualComparison(this->word ,expectedOutput.getWord()) )
            return 1;
    }

    return 0;

}

bool WordEntry::equals(WordEntry &expectedOutput) {
    if(this->getRect()==expectedOutput.getRect()&&this->getWord()==expectedOutput.getWord())
        return true;
    else
        return false;
}

Rect WordEntry::getRect(){
    return this->boundingBox;
}

string WordEntry::getWord(){
    return this->word;
}

void WordEntry::setRect(Rect input){
    boundingBox=input;
}

void WordEntry::setString(string input){
    word=input;
}