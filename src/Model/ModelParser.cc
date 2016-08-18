
#include "ModelParser.h"
#include "TableNode.h"

#include "../../csv/CSVparser.h"
#include "../HelperMethods.h"
#include <fstream>
#include "RepeatInputNode.h"

using namespace std;

void ModelParser::runModelBuilderProgram(string path) {
    Node::lastId=0;
    shared_ptr<Node>document=ModelParser().execute(path);

    Json::Value output;
    convertToJson(output, document->subNodes["DOCUMENT"]);
    cout<<output;
}

shared_ptr<Node> ModelParser::execute(string path) {
    document=shared_ptr<Node>(new Node);

    ifstream theFile(path);
    string line;
    int lineNumber=1;
    // Line by line go through the model file
    while(getline(theFile, line)) {
        processStatement(line, lineNumber);
        lineNumber++;
    }


    return document;
}

void ModelParser::processStatement(string statement, int lineNumber) {
    vector<string>fields;
    string s="";
    bool withInField=false;
    bool escaped=false;
    for(char &c:statement) {
        if(withInField) {
            if(escaped) {
                s+=c;
                escaped=false;
            }
            else if(c=='\\'&&!escaped)
                escaped=true;
            else if(c=='\"')
                withInField=false;
            else
                s+=c;
        }
        else {
            if(c==' ') {
                fields.push_back(s);
                s="";
            }
            else if (c=='\"') {
                withInField=true;
            }
            else
                s+=c;
        }
    }
    if(s.length()!=0)
        fields.push_back(s);

    if(fields.size()==0) {
        return;
    }

    if(fields[0]=="DEFINE") {
        string id=fields[1];
        string type=fields[2];

        vector<string>hierarchy=HelperMethods::regexSplit(id,"[:]");

        if(type=="NODE") {
            // Just create the hierarchy
            createHierarchy(hierarchy);

        }
        else if(type=="STATIC_TEXT") {
            // Create parent hierarchy
            vector<string>hierarchy2=vector<string>(hierarchy.begin(),hierarchy.end()-1);
            shared_ptr<Node>parentNode=createHierarchy(hierarchy2);
            string myId=hierarchy[hierarchy.size()-1];
            shared_ptr<TextNode>newNode=shared_ptr<TextNode>(new TextNode(fields[3]));
            parentNode->subNodes[myId]=newNode;
            newNode->id=id;
        }
        else if(type=="INPUT") {
            // Create parent hierarchy
            vector<string>hierarchy2=vector<string>(hierarchy.begin(),hierarchy.end()-1);
            shared_ptr<Node>parentNode=createHierarchy(hierarchy2);
            string myId=hierarchy[hierarchy.size()-1];

            InputNode::InputType type;

            if(fields[3]=="NUMERIC") {
                type=InputNode::INPUT_NUMERIC;
            }
            else if(fields[3]=="ALPHA_NUMERIC") {
                type=InputNode::INPUT_ALPHA_NUMERIC;
            }
            else if(fields[3]=="BOOLEAN") {
                type=InputNode::INPUT_CHECKBOX;
            }

            shared_ptr<InputNode>newNode=shared_ptr<InputNode>(new InputNode(type));
            parentNode->subNodes[myId]=newNode;
            newNode->id=id;

            if(fields.size()==5)
                newNode->descriptiveName=fields[4];


        }
        else if(type=="REPEAT_INPUT") {
//            cout<<"Repeat created"<<endl;

            // Create parent hierarchy
            vector<string>hierarchy2=vector<string>(hierarchy.begin(),hierarchy.end()-1);
            shared_ptr<Node>parentNode=createHierarchy(hierarchy2);
            string myId=hierarchy[hierarchy.size()-1];

            InputNode::InputType type;

            if(fields[3]=="NUMERIC") {
                type=InputNode::INPUT_NUMERIC;
            }
            else if(fields[3]=="ALPHA_NUMERIC") {
                type=InputNode::INPUT_ALPHA_NUMERIC;
            }
            else if(fields[3]=="BOOLEAN") {
                type=InputNode::INPUT_CHECKBOX;
            }

            shared_ptr<InputNode>newNode=shared_ptr<InputNode>(new RepeatInputNode(type));
            parentNode->subNodes[myId]=newNode;
            newNode->id=id;

            if(fields.size()==5)
                newNode->descriptiveName=fields[4];

        }
        else if(type=="TABLE") {
            // Create parent hierarchy
            vector<string>hierarchy2=vector<string>(hierarchy.begin(),hierarchy.end()-1);
            shared_ptr<Node>parentNode=createHierarchy(hierarchy2);
            string myId=hierarchy[hierarchy.size()-1];

            int numCols=stoi(fields[3]);
            int numRows=stoi(fields[4]);

            shared_ptr<TableNode>newNode=shared_ptr<TableNode>(new TableNode());
            newNode->numRows=numRows;
            newNode->numCols=numCols;

            parentNode->subNodes[myId]=newNode;
            newNode->id=id;
        }
    }
    else if(fields[0]=="RULE") {
        string nodeAId=fields[1];
        string rule=fields[2];
        string nodeBId=fields[3];


        vector<string>hierarchyA=HelperMethods::regexSplit(nodeAId,"[:]");
        vector<string>hierarchyB=HelperMethods::regexSplit(nodeBId,"[:]");

        shared_ptr<Node>nodeA=findNode(hierarchyA);
        shared_ptr<Node>nodeB=findNode(hierarchyB);

        if(nodeA== nullptr) {
            cout<<"Node A not found at line: "<<lineNumber<<endl;
//            assert(false);
        }

        if(nodeB== nullptr) {
            cout<<"Node B not found at line: "<<lineNumber<<endl;
//            assert(false);
        }
        if(nodeA!= nullptr&&nodeB!= nullptr) {

            if (rule == "IS_BELOW") {
                isBelow(nodeA, nodeB);
            }
            else if (rule == "IS_ABOVE") {
                isAbove(nodeA, nodeB);
            }
            else if (rule == "IS_LEFT_TO") {
                isLeftTo(nodeA, nodeB);
            }
            else if (rule == "IS_RIGHT_TO") {
                isRightTo(nodeA, nodeB);
            }
        }
    }
    else if(fields[0]=="ADD_TO_TABLE") {
        if(fields.size()!=5) {
            cout<<"Error at line "<<lineNumber<<": "<<endl;
        }

        string nodeId=fields[1];
        string tableId=fields[2];

        int x=stoi(fields[3]);
        int y=stoi(fields[4]);

        string key="(";
        key+=to_string(x);
        key+=",";
        key+=to_string(y);
        key+=")";

        vector<string>hierarchyNode=HelperMethods::regexSplit(nodeId,"[:]");
        vector<string>hierarchyTable=HelperMethods::regexSplit(tableId,"[:]");

        shared_ptr<Node>node=findNode(hierarchyNode);
        shared_ptr<TableNode>table=dynamic_pointer_cast<TableNode>(findNode(hierarchyTable));

        if(table!= nullptr) {
            table->tableEntries[key]=node;
        }
        else {
            cerr<<"Error at line "<<lineNumber<<": Table not found"<<endl;
        }
    }
    else if(fields[0]=="BIND") {
        if(fields.size()!=3) {
            cout<<"Error at line "<<lineNumber<<": "<<endl;
        }

        string nodeId=fields[1];
        string bindedName=fields[2];


        vector<string>hierarchyNode=HelperMethods::regexSplit(nodeId,"[:]");
        shared_ptr<InputNode>node=dynamic_pointer_cast<InputNode>(findNode(hierarchyNode));


//        node->bindedGroundTruthEntries.push_back(shared_ptr<pair<string,bool>>(new pair<string,bool>(bindedName,false)));
        node->bindedGroundTruthEntries.push_back(bindedName);

    }
}

shared_ptr<Node> ModelParser::findNode(const vector<string> &hierarchy) {
    shared_ptr<Node>current=document;
    for(string s:hierarchy) {
        unordered_map<string, shared_ptr<Node>>::iterator iterator1=current->subNodes.find(s);
        if(iterator1!=current->subNodes.end()) {
            current=iterator1->second;
        }
        else {
            return nullptr;
        }
    }
    return current;
}

shared_ptr<Node> ModelParser::findNode(const vector<string> &hierarchy,shared_ptr<Node>theNode) {
    shared_ptr<Node>current=theNode;
    for(string s:hierarchy) {
        unordered_map<string, shared_ptr<Node>>::iterator iterator1=current->subNodes.find(s);
        if(iterator1!=current->subNodes.end()) {
            current=iterator1->second;
        }
        else {
            return nullptr;
        }
    }
    return current;
}


shared_ptr<Node> ModelParser::createHierarchy(vector<string>&hierarchy) {
    shared_ptr<Node>current=document;

    string absoluteId="";
//    for_each(hierarchy.begin(),hierarchy.end(),[&] (string&s2){
//        absoluteId+=s2+":";
//    });
//    if(absoluteId.length()!=0) {
//        absoluteId=absoluteId.substr(0,absoluteId.length()-1);
//    }
//
//    cout<<absoluteId<<endl;

    absoluteId="";
    int index=0;
    for(string s:hierarchy) {
        unordered_map<string, shared_ptr<Node>>::iterator iterator1=current->subNodes.find(s);
        if(iterator1!=current->subNodes.end()) {
            current=iterator1->second;
            index++;
            absoluteId+=s+":";
        }
        else {
            break;
        }
    }


    for(int i=index;i<hierarchy.size();i++) {
        shared_ptr<Node>newNode=shared_ptr<Node>(new Node);
        string s=hierarchy[i];
        absoluteId+=s;
        newNode->id=absoluteId;
        current=(current->subNodes[s]=newNode);

        absoluteId+=":";
    }
    return current;
}

void ModelParser::convertToJson(Json::Value &jsonOutput, const shared_ptr<Node>&model) {
    if(dynamic_pointer_cast<TextNode>(model)!= nullptr) {
        shared_ptr<TextNode> tModel=dynamic_pointer_cast<TextNode>(model);
        jsonOutput["type"]="static_text";
        jsonOutput["text"]=tModel->text;
        if(tModel->textAssigned.size()!=0) {
            jsonOutput["textAssigned"]=tModel->textAssigned;

        }
    }
    else if(dynamic_pointer_cast<RepeatInputNode>(model)!= nullptr) {
        shared_ptr<RepeatInputNode> rModel=dynamic_pointer_cast<RepeatInputNode>(model);
        jsonOutput["type"]="input";
        jsonOutput["value"]=rModel->data;
        string type="";
        switch(rModel->inputType) {
            case InputNode::INPUT_ALPHA_NUMERIC:
                type="alpha_numeric";
                break;
            case InputNode::INPUT_NUMERIC:
                type="numeric";
                break;
            case InputNode::INPUT_CHECKBOX:
                type="boolean";
                break;
        }

        jsonOutput["input_type"]=type;


        Json::Value tableEntries;
        for_each(rModel->repeatInputSubNodes.begin(),rModel->repeatInputSubNodes.end(), [&] (pair<string,shared_ptr<Node>> kv) {
            string keyy=kv.first;
            shared_ptr<Node> v=kv.second;

            Json::Value jsonV;
            convertToJson(jsonV,v);

            tableEntries[keyy]=jsonV;
        });
        jsonOutput["repeat_input_sub_nodes"]=tableEntries;
    }
    else if(dynamic_pointer_cast<InputNode>(model)!= nullptr) {
        shared_ptr<InputNode> iModel=dynamic_pointer_cast<InputNode>(model);
        jsonOutput["type"]="input";
        jsonOutput["value"]=iModel->data;
        string type="";
        switch(iModel->inputType) {
            case InputNode::INPUT_ALPHA_NUMERIC:
                type="alpha_numeric";
                break;
            case InputNode::INPUT_NUMERIC:
                type="numeric";
                break;
            case InputNode::INPUT_CHECKBOX:
                type="boolean";
                break;
        }

        jsonOutput["input_type"]=type;
    }
    else if(dynamic_pointer_cast<TableNode>(model)!= nullptr) {
        shared_ptr<TableNode> tModel=dynamic_pointer_cast<TableNode>(model);
        jsonOutput["type"]="tabular";
        jsonOutput["num_rows"]=tModel->numRows;
        jsonOutput["num_cols"]=tModel->numCols;
        jsonOutput["data"]=tModel->data;

        Json::Value tableEntries;
        for_each(tModel->tableEntries.begin(),tModel->tableEntries.end(), [&] (pair<string,shared_ptr<Node>> kv) {
            string keyy=kv.first;
            shared_ptr<Node> v=kv.second;

            Json::Value jsonV;
            convertToJson(jsonV,v);

            tableEntries[keyy]=jsonV;
        });
        jsonOutput["table_entries"]=tableEntries;
    }

    jsonOutput["id"]=model->id;
    for ( auto it = model->rulesModel.begin(); it != model->rulesModel.end(); ++it ) {
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

        jsonOutput[key]=rr;
    }


    if(model->subNodes.size()!=0) {
        Json::Value subNodes;
        int x = 0;
        for_each(model->subNodes.begin(), model->subNodes.end(), [&](pair<string,shared_ptr<Node>> current) {
            Json::Value jsonV;
            convertToJson(jsonV, current.second);
            subNodes[x] = jsonV;
            x++;
        });
        jsonOutput["sub_nodes"] = subNodes;
    }
}

void ModelParser::isBelow(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<string>>::const_iterator got = a->rulesModel.find ("is_below");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_below"] = unordered_set<string>();
    }
    a->rulesModel["is_below"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_above");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_above"] = unordered_set<string>();
    }
    b->rulesModel["is_above"].insert(a->id);

}


void ModelParser::isAbove(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<string>>::const_iterator got = a->rulesModel.find ("is_above");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_above"] = unordered_set<string>();
    }
    a->rulesModel["is_above"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_below");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_below"] = unordered_set<string>();
    }
    b->rulesModel["is_below"].insert(a->id);

}


void ModelParser::isRightTo(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<string>>::const_iterator got = a->rulesModel.find ("is_right_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_right_to"] = unordered_set<string>();
    }
    a->rulesModel["is_right_to"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_left_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_left_to"] = unordered_set<string>();
    }
    b->rulesModel["is_left_to"].insert(a->id);

}


void ModelParser::isLeftTo(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<string>>::const_iterator got = a->rulesModel.find ("is_left_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_left_to"] = unordered_set<string>();
    }
    a->rulesModel["is_left_to"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_right_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_right_to"] = unordered_set<string>();
    }
    b->rulesModel["is_right_to"].insert(a->id);

}


//void ModelParser::orderIds(shared_ptr<Node> node, int currentId) {
//    node->id=currentId;
//
//}