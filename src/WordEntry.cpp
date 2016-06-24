//
// Created by shahrukhqasim on 6/24/16.
//

#include "WordEntry.h"

bool WordEntry::compare(WordEntry &expectedOutput){

    int matched=0;

    // Jc = (Box1 intersection Box2)/(Box1 union Box2)

    float Jc =float((this->boundingBox & expectedOutput.getRect()).area())/( this->boundingBox | expectedOutput.getRect() ).area() ;


    // TODO: Fix this
    if(true){
        if(this->word == expectedOutput.getWord())
            return 1;
    }

    return 0;

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