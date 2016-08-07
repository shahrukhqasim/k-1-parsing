//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "Node.h"


int Node::lastId = 0;

Node::~Node() {
}

Node::Node(bool incrementId) {
    if(incrementId) {
        lastId++;
        this->id=to_string(lastId);
    }

}