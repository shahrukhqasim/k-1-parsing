
#include "Node.h"


using namespace std;

int Node::lastId = 0;

Node::~Node() {
}

Node::Node(bool incrementId) {
    if(incrementId) {
        lastId++;
        this->id=to_string(lastId);
    }

}