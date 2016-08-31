#include <vector>
#include <unordered_map>
#include "TreeFormModel.h"
#include "TextTreeFormNode.h"
#include "InputTreeFormNode.h"
#include "RepeatInputTreeFormNode.h"
#include "TableTreeFormNode.h"
#include "DivisionRuleWithReference.h"

std::shared_ptr<TreeFormNodeInterface> TreeFormModel::constructRoot() {
    if(processMdl()) {
        return root;
    }
    else
        return nullptr;
}

std::shared_ptr<TreeFormNodeInterface>
TreeFormModel::getNode(std::shared_ptr<TreeFormNodeInterface> root, std::vector<std::string> hierarchy) {
    std::shared_ptr<TreeFormNodeInterface> current = root;
    for (std::string s:hierarchy) {
        auto iterator1 = current->getChild(s);
        if (iterator1 != nullptr) {
            current = iterator1;
        } else {
            return nullptr;
        }
    }
    return current;
}

int TreeFormModel::getIterations() {
    return 5;
}

TreeFormModel::TreeFormModel() {
}

bool TreeFormModel::processMdl() {
    bool hasError = false;
    std::stringstream stringStream(mdlData);
    std::string line;
    int lineNumber = 1;
    root=std::shared_ptr<BasicTreeFormNode>(new BasicTreeFormNode());
    while (getline(stringStream, line)) {
        if (processLine(line, lineNumber)) {
        } else {
            std::cout<<"Error at line "<<lineNumber<<": "<<line<<std::endl;
            hasError = true;
        }
        ++lineNumber;
    }

    return !hasError;
}

void TreeFormModel::divideLine(const std::string &line, std::vector<std::string>& fields) {
    fields.clear();
    std::string s = "";
    bool withInField = false;
    bool escaped = false;
    for (char c:line) {
        if (withInField) {
            if (escaped) {
                s += c;
                escaped = false;
            } else if (c == '\\' && !escaped)
                escaped = true;
            else if (c == '\"')
                withInField = false;
            else
                s += c;
        } else {
            if (c == ' ') {
                fields.push_back(s);
                s = "";
            } else if (c == '\"') {
                withInField = true;
            } else
                s += c;
        }
    }
    if (s.length() != 0)
        fields.push_back(s);
}

std::shared_ptr<TreeFormNodeInterface> TreeFormModel::createHierarchy(std::vector<std::string> &hierarchy) {
    std::shared_ptr<TreeFormNodeInterface> current = root;

    std::string absoluteId = "";

    absoluteId = "";
    int index = 0;
    for (std::string s:hierarchy) {
        auto iterator1 = current->getChild(s);
        if (iterator1 != nullptr) {
            index++;
            absoluteId += s + ":";
            current = iterator1;
        } else {
            break;
        }
    }

    for (size_t i = index; i < hierarchy.size(); i++) {
        std::shared_ptr<BasicTreeFormNode> newNode = std::shared_ptr<BasicTreeFormNode>(new BasicTreeFormNode);
        std::string s = hierarchy[i];
        absoluteId += s;
        newNode->setId(absoluteId);
        current = current->addChild(s, newNode);

        absoluteId += ":";
    }
    return current;
}

bool TreeFormModel::processLine(const std::string &line, int lineNumber) {
    std::vector<std::string> fields;
    divideLine(line, fields);

    if (fields.size() == 0) {
        return true;
    }

    bool hasError = false;

    if (fields[0] == "DEFINE") {
        std::string id = fields[1];
        std::string type = fields[2];

        std::vector<std::string> hierarchy = HelperMethods::regexSplit(id, "[:]");

        if (type == "NODE") {
            if (fields.size() != 3) {
                std::cerr << "Error at line: " << lineNumber << ": DEFINE NODE has extra arguments" << std::endl;
                hasError = true;
            }
            // Just create the hierarchy
            createHierarchy(hierarchy);

        } else if (type == "STATIC_TEXT") {
            if (fields.size() != 4) {
                std::cerr << "Error at line: " << lineNumber << ": DEFINE STATIC_TEXT has extra or not enough arguments"
                          << std::endl;
                hasError = true;
            }
            std::vector<std::string> hierarchy2 = std::vector<std::string>(hierarchy.begin(), hierarchy.end() - 1);
            std::shared_ptr<TreeFormNodeInterface> parentNode = createHierarchy(hierarchy2);
            std::string myId = hierarchy[hierarchy.size() - 1];

            std::shared_ptr<TextTreeFormNode> newNode = std::shared_ptr<TextTreeFormNode>(
                    new TextTreeFormNode(fields[3]));
            parentNode->addChild(myId, newNode);
            newNode->setId(id);
        } else if (type == "INPUT") {
            if (!(fields.size() == 4 || fields.size() == 5)) {
                std::cerr << "Error at line: " << lineNumber << ": DEFINE INPUT has extra or not enough arguments"
                          << std::endl;
                hasError = true;
            }
            // Create parent hierarchy
            std::vector<std::string> hierarchy2 = std::vector<std::string>(hierarchy.begin(), hierarchy.end() - 1);
            std::shared_ptr<TreeFormNodeInterface> parentNode = createHierarchy(hierarchy2);
            std::string myId = hierarchy[hierarchy.size() - 1];

            InputTreeFormNode::InputType type;

            if (fields[3] == "NUMERIC") {
                type = InputTreeFormNode::INPUT_NUMERIC;
            } else if (fields[3] == "ALPHA_NUMERIC") {
                type = InputTreeFormNode::INPUT_ALPHA_NUMERIC;
            } //else if (fields[3] == "BOOLEAN") {
            else{
            type = InputTreeFormNode::INPUT_CHECKBOX;
            }

            std::shared_ptr<InputTreeFormNode> newNode = std::shared_ptr<InputTreeFormNode>(
                    new InputTreeFormNode(type));
            parentNode->addChild(myId, newNode);
            newNode->setId(id);

            if (fields.size() == 5)
                newNode->setDescriptiveName(fields[4]);
        } else if (type == "REPEAT_INPUT") {
            if (!(fields.size() == 4 || fields.size() == 5)) {
                std::cerr << "Error at line: " << lineNumber
                          << ": DEFINE REPEAT_INPUT has extra or not enough arguments" << std::endl;
                hasError = true;
            }
            // Create parent hierarchy
            std::vector<std::string> hierarchy2 = std::vector<std::string>(hierarchy.begin(), hierarchy.end() - 1);
            std::shared_ptr<TreeFormNodeInterface> parentNode = createHierarchy(hierarchy2);
            std::string myId = hierarchy[hierarchy.size() - 1];

            InputTreeFormNode::InputType type;

            if (fields[3] == "NUMERIC") {
                type = InputTreeFormNode::INPUT_NUMERIC;
            } else if (fields[3] == "ALPHA_NUMERIC") {
                type = InputTreeFormNode::INPUT_ALPHA_NUMERIC;
            } //else if (fields[3] == "BOOLEAN") {
            else{
            type = InputTreeFormNode::INPUT_CHECKBOX;
            }

            std::shared_ptr<RepeatInputTreeFormNode> newNode = std::shared_ptr<RepeatInputTreeFormNode>(
                    new RepeatInputTreeFormNode(type));
            parentNode->addChild(myId, newNode);
            newNode->setId(id);

            if (fields.size() == 5)
                newNode->setDescriptiveName(fields[4]);
        } else if (type == "TABLE") {
            if (!(fields.size() == 5)) {
                std::cerr << "Error at line: " << lineNumber << ": DEFINE TABLE has extra or not enough arguments"
                          << std::endl;
                hasError = true;
            }

            // Create parent hierarchy
            std::vector<std::string> hierarchy2 = std::vector<std::string>(hierarchy.begin(), hierarchy.end() - 1);
            std::shared_ptr<TreeFormNodeInterface> parentNode = createHierarchy(hierarchy2);
            std::string myId = hierarchy[hierarchy.size() - 1];

            int numCols = stoi(fields[3]);
            int numRows = stoi(fields[4]);

            std::shared_ptr<TableTreeFormNode> newNode = std::shared_ptr<TableTreeFormNode>(
                    new TableTreeFormNode(numCols, numRows));

            parentNode->addChild(myId, newNode);
            newNode->setId(id);
        }
    } else if (fields[0] == "RULE") {
        if (!(fields.size() == 4)) {
            std::cerr << "Error at line: " << lineNumber << ": RULE has extra or not enough arguments" << std::endl;
            hasError = true;
        }
        std::string rule = fields[2];
        std::string nodeAId=fields[1];
        std::string nodeBId=fields[3];
        if (!(rule == "IS_BELOW" || rule == "IS_ABOVE" || rule == "IS_LEFT_TO" || rule == "IS_RIGHT_TO" ||
              rule == "HAS_HORIZONTAL_OVERLAP_WITH" || rule == "HAS_VERTICAL_OVERLAP_WITH")) {
            hasError = true;
        }


        std::vector<std::string>hierarchyA=HelperMethods::regexSplit(nodeAId,"[:]");
        std::vector<std::string>hierarchyB=HelperMethods::regexSplit(nodeBId,"[:]");

        std::shared_ptr<BasicTreeFormNode>nodeA=std::dynamic_pointer_cast<BasicTreeFormNode>(getNode(root,hierarchyA));
        std::shared_ptr<BasicTreeFormNode>nodeB=std::dynamic_pointer_cast<BasicTreeFormNode>(getNode(root,hierarchyB));

        if(nodeA== nullptr) {
            std::cerr<<"Node A not found at line: "<<lineNumber<<std::endl;
            hasError=true;
//            assert(false);
        }

        if(nodeB== nullptr) {
            std::cerr<<"Node B not found at line: "<<lineNumber<<std::endl;
            hasError=true;
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
            else if (rule == "HAS_HORIZONTAL_OVERLAP_WITH") {
                hasHorizontalOverlapWith(nodeA, nodeB);
            }
            else if (rule == "HAS_VERTICAL_OVERLAP_WITH") {
                hasVerticalOverlapWith(nodeA, nodeB);
            }
            else {
                std::cerr<<"Error at line "<<lineNumber<<std::endl;
                hasError=true;
            }
        }


    }else if (fields[0] == "RULE_DIVIDE") {
        if (!(fields.size() == 4 || fields.size() == 3)) {
            std::cerr << "Error at line: " << lineNumber << ": RULE has extra or not enough arguments" << std::endl;
            hasError = true;
        }
        std::string rule = fields[2];

        bool reference;
        if ((rule == "IS_BELOW" || rule == "IS_ABOVE" || rule == "IS_LEFT_TO" || rule == "IS_RIGHT_TO" || rule == "IS_BELOW_INCLUSIVE"|| rule == "IS_ABOVE_INCLUSIVE")) {

            reference = true;
        }
        else if((rule == "IS_LEFT_HALF" || rule == "IS_RIGHT_HALF" )) {
            reference=false;
        }
        else {
            std::cerr<<"Not recognized rule at line"<<lineNumber<<std::endl;
            hasError = true;
        }

        if(!hasError) {
            if(reference) {
                std::string nodeAId = fields[1];
                std::string nodeBId = fields[3];


                std::vector<std::string> hierarchyA = HelperMethods::regexSplit(nodeAId, "[:]");
                std::vector<std::string> hierarchyB = HelperMethods::regexSplit(nodeBId, "[:]");

                std::shared_ptr<BasicTreeFormNode> nodeA = std::dynamic_pointer_cast<BasicTreeFormNode>(
                        getNode(root, hierarchyA));
                std::shared_ptr<BasicTreeFormNode> nodeB = std::dynamic_pointer_cast<BasicTreeFormNode>(
                        getNode(root, hierarchyB));

                if (nodeA == nullptr) {
                    std::cerr << "Node A not found at line: " << lineNumber << std::endl;
                    hasError = true;
//            assert(false);
                }

                if (nodeB == nullptr) {
                    std::cerr << "Node B not found at line: " << lineNumber << std::endl;
                    hasError = true;
//            assert(false);
                }
                if (nodeA != nullptr && nodeB != nullptr) {
                    std::transform(rule.begin(), rule.end(), rule.begin(), ::tolower);
                    addDivisionRuleWithReference(rule,nodeA,nodeB);
                }
            }
            else {

                std::string nodeAId = fields[1];

                std::vector<std::string> hierarchyA = HelperMethods::regexSplit(nodeAId, "[:]");

                std::shared_ptr<BasicTreeFormNode> nodeA = std::dynamic_pointer_cast<BasicTreeFormNode>(
                        getNode(root, hierarchyA));

                if (nodeA == nullptr) {
                    std::cerr << "Node A not found at line: " << lineNumber << std::endl;
                    hasError = true;
//            assert(false);
                }

                if (nodeA != nullptr) {
                    std::transform(rule.begin(), rule.end(), rule.begin(), ::tolower);
                    addDivisionRule(rule,nodeA);
                }
            }
        }


    } else if (fields[0] == "ADD_TO_TABLE") {
        if (fields.size() != 5) {
            std::cerr << "Error at line " << lineNumber << ": ADD_TO_TABLE has extra or not enough arguments"
                      << std::endl;
            hasError = true;
        }

        std::string nodeId = fields[1];
        std::string tableId = fields[2];

        int x = stoi(fields[3]);
        int y = stoi(fields[4]);

        std::string key = "(";
        key += std::to_string(x);
        key += ",";
        key += std::to_string(y);
        key += ")";

        std::vector<std::string> hierarchyNode = HelperMethods::regexSplit(nodeId, "[:]");
        std::vector<std::string> hierarchyTable = HelperMethods::regexSplit(tableId, "[:]");

        std::shared_ptr<TreeFormNodeInterface> node = getNode(root, hierarchyNode);
        std::shared_ptr<TableTreeFormNode> table = std::dynamic_pointer_cast<TableTreeFormNode>(
                getNode(root, hierarchyTable));

        if (table != nullptr) {
//            table->tableEntries[key]=node;
            table->addChild(key, node);
        } else {
            std::cerr << "Error at line " << lineNumber << ": Table not found or error in table" << std::endl;
        }
    } else if (fields[0] == "BIND") {
        // TODO: Skipping this right now
    } else if (line[0] == '#') {
        // It is a comment
    } else {
        std::cerr << "Error at line " << lineNumber << ": Undefined operation" << std::endl;
        hasError = true;
    }

    return !hasError;
}

std::shared_ptr<TreeFormModel> TreeFormModel::constructFormModel(std::string mdl) {

    std::shared_ptr<TreeFormModel>model=std::shared_ptr<TreeFormModel>(new TreeFormModel());
    model->mdlData=mdl;
    if(model->processMdl())
        return model;
    else
        return nullptr;
}

std::shared_ptr<TreeFormModel> TreeFormModel::constructFormModel(std::ifstream mdlFile) {
    std::shared_ptr<TreeFormModel>model=std::shared_ptr<TreeFormModel>(new TreeFormModel());
    std::stringstream stringStream;
    stringStream << mdlFile.rdbuf();
    model->mdlData=stringStream.str();

    if(model->processMdl())
        return model;
    else {
        return nullptr;
    }
}

void TreeFormModel::isBelow(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {
    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_below");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_below"] = std::unordered_set<std::string>();
    }
    a->rulesModel["is_below"].insert(b->getId());


    // Set the rule in node b
    got = b->rulesModel.find ("is_above");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_above"] = std::unordered_set<std::string>();
    }
    b->rulesModel["is_above"].insert(a->getId());
}

void TreeFormModel::isAbove(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {

    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_above");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_above"] = std::unordered_set<std::string>();
    }
    a->rulesModel["is_above"].insert(b->getId());


    // Set the rule in node b
    got = b->rulesModel.find ("is_below");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_below"] = std::unordered_set<std::string>();
    }
    b->rulesModel["is_below"].insert(a->getId());
}

void TreeFormModel::isRightTo(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {

    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_right_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_right_to"] = std::unordered_set<std::string>();
    }
    a->rulesModel["is_right_to"].insert(b->getId());


    // Set the rule in node b
    got = b->rulesModel.find ("is_left_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_left_to"] = std::unordered_set<std::string>();
    }
    b->rulesModel["is_left_to"].insert(a->getId());


}

void TreeFormModel::isLeftTo(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {

    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_left_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_left_to"] = std::unordered_set<std::string>();
    }
    a->rulesModel["is_left_to"].insert(b->getId());


    // Set the rule in node b
    got = b->rulesModel.find ("is_right_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_right_to"] = std::unordered_set<std::string>();
    }
    b->rulesModel["is_right_to"].insert(a->getId());

}

void TreeFormModel::hasVerticalOverlapWith(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {
    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_left_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["has_vertical_overlap_with"] = std::unordered_set<std::string>();
    }
    a->rulesModel["has_vertical_overlap_with"].insert(b->getId());

}

void
TreeFormModel::hasHorizontalOverlapWith(std::shared_ptr<BasicTreeFormNode> a, std::shared_ptr<BasicTreeFormNode> b) {
    // Set the rule in node a
    std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator got = a->rulesModel.find ("is_left_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["has_horizontal_overlap_with"] = std::unordered_set<std::string>();
    }
    a->rulesModel["has_horizontal_overlap_with"].insert(b->getId());

}

void TreeFormModel::addDivisionRule(std::string keyWord, std::shared_ptr<BasicTreeFormNode> a) {
    std::shared_ptr<DivisionRule>divisionRule=std::shared_ptr<DivisionRule>(new DivisionRule(keyWord,a->getId()));
    a->divisionRules.push_back(divisionRule);

}

void TreeFormModel::addDivisionRuleWithReference(std::string keyWord, std::shared_ptr<BasicTreeFormNode> a,
                                                 std::shared_ptr<BasicTreeFormNode> b) {
    std::shared_ptr<DivisionRule>divisionRule=std::shared_ptr<DivisionRule>(new DivisionRuleWithReference(keyWord,a->getId(),b->getId()));
    a->divisionRules.push_back(divisionRule);

}
