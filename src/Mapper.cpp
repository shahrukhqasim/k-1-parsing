//
// Created by shahrukhqasim on 7/28/16.
//

#include "Mapper.h"


Mapper::Mapper(const vector<TextualData> &data,shared_ptr<Node>node) {
    this->data=data;
    this->node=node;
}

void Mapper::execute() {
    recursiveCall(node);
}

int Mapper::findMinTextIndex(const vector<TextualData> &data, const string &textToFind) {
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

Rect Mapper::recursiveCall(shared_ptr<Node> node) {
    Rect rectangle;
    bool regionAssigned;

    if(dynamic_pointer_cast<TextNode>(node)!= nullptr) {
        shared_ptr<TextNode> tNode=dynamic_pointer_cast<TextNode>(node);
        string text=tNode->text;
        int minIndex=findMinTextIndex(data,text);
        node->region=data[minIndex].getRect();
        tNode->textAssigned=data[minIndex].getText();
        rectangle=node->region;
        taken.push_back(data[minIndex]);
        data.erase(data.begin()+minIndex);
        regionAssigned=true;

    }
    else if(dynamic_pointer_cast<InputNode>(node)!= nullptr) {
//        shared_ptr<InputNode> iModel=dynamic_pointer_cast<InputNode>(node);
//        string type="";
//        switch(iModel->inputType) {
//            case InputNode::INPUT_ALPHA_NUMERIC:
//                type="alpha_numeric";
//                break;
//            case InputNode::INPUT_NUMERIC:
//                type="numeric";
//                break;
//            case InputNode::INPUT_CHECKBOX:
//                type="boolean";
//                break;
//        }
    }
    else if(dynamic_pointer_cast<TableNode>(node)!= nullptr) {
//        shared_ptr<TableNode> tModel=dynamic_pointer_cast<TableNode>(node);
//
//        Json::Value tableEntries;
//        for_each(tModel->tableEntries.begin(),tModel->tableEntries.end(), [&] (pair<string,shared_ptr<Node>> kv) {
//            string keyy=kv.first;
//            shared_ptr<Node> v=kv.second;
//
//            Json::Value jsonV;
//
//            tableEntries[keyy]=jsonV;
//        });
    }

    for ( auto it = node->rulesModel.begin(); it != node->rulesModel.end(); ++it ) {
        string key=it->first;

        unordered_set<string>value=it->second;
        string rr="";
        for_each(value.begin(),value.end(),[&] (const string&x){
            rr+=x+",";
        });
        if(rr.length()!=0) {
            if (rr[rr.length() - 1] == ',') {
                rr=rr.substr(0,rr.length()-1);
            }
        }

    }


    if(node->subNodes.size()!=0) {
        Json::Value subNodes;
        for_each(node->subNodes.begin(), node->subNodes.end(), [&](pair<string,shared_ptr<Node>> current) {
            Rect newStuff=recursiveCall(current.second);
            if(regionAssigned) {
                rectangle=newStuff|rectangle;
            }
            else {
                rectangle=newStuff;
            }
        });
    }

    // Verify with rules over here

    return rectangle;
}