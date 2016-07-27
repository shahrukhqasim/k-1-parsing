//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "ModelBuilder.h"
#include "TableNode.h"


void ModelBuilder::start() {
    Node::lastId=0;
    ModelBuilder().execute();
}

void ModelBuilder::execute() {

    document=shared_ptr<Node>(new Node);
    header1=shared_ptr<Node>(new Node);
    header2=shared_ptr<Node>(new Node);
    part1=shared_ptr<Node>(new Node);
    part2=shared_ptr<Node>(new Node);
    part3=shared_ptr<Node>(new Node);

    buildHeader1();
    buildHeader2();
    buildPart1();
    buildPart2();
    buildPart3();

    document->subNodes.push_back(header1);
    document->subNodes.push_back(header2);
    document->subNodes.push_back(part1);
    document->subNodes.push_back(part2);
    document->subNodes.push_back(part3);



    Json::Value output;
    convertToJson(output, document);

    cout<<output<<endl;
}

string ModelBuilder::pointToString(const Point &p) {
    string s=string("(");
    s+=to_string(p.x);
    s+=",";
    s+=to_string(p.y);
    s+=")";
    return s;
}

void ModelBuilder::buildHeader1() {
    vector<shared_ptr<Node>>nodes;

    // For safety, empty the vector first
    this->header1->subNodes.clear();

    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("Schedule K-1"))); // 0
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("(Form 1065)"))); // 1
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("Department of Treasury"))); // 2
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("Internal Revenue Service"))); // 3
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's Share of Income, Deductions,"))); // 4
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("Credits, etc."))); // 5

    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("See back of form and sepeerate instructions"))); // 6
    this->header1->subNodes.push_back(shared_ptr<TextNode>(new TextNode("2013"))); // 7

    nodes.push_back(shared_ptr<TextNode>(new TextNode("For calendar year 2013, or tax"))); //0
    nodes.push_back(shared_ptr<TextNode>(new TextNode("year beginning"))); // 1
    nodes.push_back(shared_ptr<TextNode>(new TextNode(",2013"))); // 2
    nodes.push_back(shared_ptr<TextNode>(new TextNode("ending"))); // 3
    nodes.push_back(shared_ptr<TextNode>(new TextNode(",20"))); // 4
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 5
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 6
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 7

    shared_ptr<Node>x=shared_ptr<Node>(new Node);

    for_each(nodes.begin(),nodes.end(), [&](shared_ptr<Node>n) {
        x->subNodes.push_back(n);
    });
    this->header1->subNodes.push_back(x);

    // Add rules now
    isBelow(this->header1->subNodes[1],this->header1->subNodes[0]);
    isBelow(this->header1->subNodes[2],this->header1->subNodes[1]);
    isBelow(this->header1->subNodes[3],this->header1->subNodes[2]);
    isBelow(this->header1->subNodes[4],this->header1->subNodes[3]);
    isBelow(this->header1->subNodes[5],this->header1->subNodes[4]);
    isBelow(this->header1->subNodes[6],this->header1->subNodes[4]);
    isRightTo(this->header1->subNodes[7],this->header1->subNodes[0]);
    isRightTo(x,this->header1->subNodes[0]);
    isBelow(x,this->header1->subNodes[7]);

    isBelow(nodes[1],nodes[0]);
    isBelow(nodes[2],nodes[0]);
    isBelow(nodes[3],nodes[1]);
    isBelow(nodes[4],nodes[2]);
    isBelow(nodes[5],nodes[0]);
    isRightTo(nodes[5],nodes[1]);
    isBelow(nodes[6],nodes[1]);
    isRightTo(nodes[6],nodes[3]);
    isBelow(nodes[7],nodes[1]);
    isRightTo(nodes[7],nodes[4]);

}


void ModelBuilder::buildHeader2() {
    vector<shared_ptr<Node>>nodes;
    vector<shared_ptr<Node>>nodes2;

    nodes.push_back(shared_ptr<Node>(new Node)); // 0
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Final K-1"))); // 1
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 2
    nodes[0]->subNodes.push_back(nodes[1]);
    nodes[0]->subNodes.push_back(nodes[2]);

    nodes2.push_back(nodes[0]); //0

    nodes.push_back(shared_ptr<Node>(new Node)); // 3
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Amended K-1"))); // 4
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 5
    nodes[3]->subNodes.push_back(nodes[4]);
    nodes[3]->subNodes.push_back(nodes[5]);

    nodes2.push_back(nodes[3]); //1

    nodes2.push_back(shared_ptr<TextNode>(new TextNode("OMB No. 1545-0099"))); // 2
    nodes2.push_back(shared_ptr<TextNode>(new TextNode("651113"))); // 3

    for_each(nodes2.begin(),nodes2.end(), [&](shared_ptr<Node>n) {
        this->header2->subNodes.push_back(n);
    });

    // Add rules
    isRightTo(nodes[1],nodes[2]);
    isRightTo(nodes[4],nodes[5]);
    isBelow(nodes2[2],nodes2[3]);

    isRightTo(nodes2[1],nodes2[0]);

    isRightTo(nodes2[2],nodes2[1]);
}


void ModelBuilder::buildPart1() {
    vector<shared_ptr<Node>>nodes;
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Part I"))); // 0
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Information About the Partnership"))); // 1
    nodes.push_back(shared_ptr<TextNode>(new TextNode("A"))); // 2
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partnerhip's employer identification number"))); // 3
    nodes.push_back(shared_ptr<TextNode>(new TextNode("B"))); // 4
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partnership's name, address, city, state and ZIP code"))); // 5
    nodes.push_back(shared_ptr<TextNode>(new TextNode("C"))); // 6
    nodes.push_back(shared_ptr<TextNode>(new TextNode("IRS Center where partnership filed return"))); // 7
    nodes.push_back(shared_ptr<TextNode>(new TextNode("D"))); // 8
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Check if this is a publicly traded partnership (PTP)"))); // 9
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 10

    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 11
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 12
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 13

    vector<shared_ptr<Node>>nodes2;

    nodes.push_back(shared_ptr<Node>(new Node)); // 14
    nodes[14]->subNodes.push_back(nodes[0]);
    nodes[14]->subNodes.push_back(nodes[1]);
    nodes2.push_back(nodes[14]); //0

    nodes.push_back(shared_ptr<Node>(new Node)); // 15
    nodes[15]->subNodes.push_back(nodes[2]);
    nodes[15]->subNodes.push_back(nodes[3]);
    nodes[15]->subNodes.push_back(nodes[11]);
    nodes2.push_back(nodes[15]); //1

    nodes.push_back(shared_ptr<Node>(new Node)); // 16
    nodes[16]->subNodes.push_back(nodes[4]);
    nodes[16]->subNodes.push_back(nodes[5]);
    nodes[16]->subNodes.push_back(nodes[12]);
    nodes2.push_back(nodes[16]); //2

    nodes.push_back(shared_ptr<Node>(new Node)); // 17
    nodes[17]->subNodes.push_back(nodes[6]);
    nodes[17]->subNodes.push_back(nodes[7]);
    nodes[17]->subNodes.push_back(nodes[13]);
    nodes2.push_back(nodes[17]); //3

    nodes.push_back(shared_ptr<Node>(new Node)); // 18
    nodes[18]->subNodes.push_back(nodes[8]);
    nodes[18]->subNodes.push_back(nodes[9]);
    nodes[18]->subNodes.push_back(nodes[10]);
    nodes2.push_back(nodes[18]); //4


    for_each(nodes2.begin(),nodes2.end(), [&](shared_ptr<Node>n) {
        this->part1->subNodes.push_back(n);
    });

    // Add rules
    isRightTo(nodes[1],nodes[0]);
    isRightTo(nodes[3],nodes[2]);
    isRightTo(nodes[5],nodes[4]);
    isRightTo(nodes[7],nodes[6]);
    isRightTo(nodes[9],nodes[8]);
    isRightTo(nodes[10],nodes[8]);
    isRightTo(nodes[9],nodes[10]);

    isBelow(nodes[1],nodes[0]);
    isBelow(nodes[2],nodes[1]);
    isBelow(nodes[3],nodes[2]);
    isBelow(nodes[4],nodes[3]);

    isBelow(nodes[11],nodes[3]);
    isBelow(nodes[12],nodes[5]);
    isBelow(nodes[13],nodes[7]);

}

void ModelBuilder::buildPart2() {
    vector<shared_ptr<Node>>nodes2;
    vector<shared_ptr<Node>>nodes;
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Part II"))); // 0
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Information About the Partner"))); // 1
    nodes.push_back(shared_ptr<TextNode>(new TextNode("E"))); // 2
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's identifying number"))); // 3
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 4
    nodes.push_back(shared_ptr<TextNode>(new TextNode("F"))); // 5
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's name, address, city, state and ZIP code"))); // 6
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 7
    nodes.push_back(shared_ptr<TextNode>(new TextNode("G"))); // 8
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Gerneral partner or LLC"))); // 9
    nodes.push_back(shared_ptr<TextNode>(new TextNode("member-manager"))); // 10
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 11 - with general partner or llc
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Limited partner or other LLC"))); // 12
    nodes.push_back(shared_ptr<TextNode>(new TextNode("member"))); // 13
    nodes.push_back(shared_ptr<TextNode>(new TextNode("H"))); // 14
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 15 0 with limited partner or llc
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Domestic Partner"))); // 16
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 17
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Foreign partner"))); // 18
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 19
    nodes.push_back(shared_ptr<TextNode>(new TextNode("I1"))); // 20
    nodes.push_back(shared_ptr<TextNode>(new TextNode("What type of entity is this partner?"))); // 21
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 22
    nodes.push_back(shared_ptr<TextNode>(new TextNode("I2"))); // 23
    nodes.push_back(shared_ptr<TextNode>(new TextNode("If this partner is a retirement plan (IRA/SEP/Keogh/etc.), check here"))); // 24
    nodes.push_back(shared_ptr<TextNode>(new TextNode("(see instructions)"))); // 25
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 26
    nodes.push_back(shared_ptr<TextNode>(new TextNode("J"))); // 27
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's share of profit, loss, and capital (see instructions):"))); // 28

    shared_ptr<TableNode> theTable=shared_ptr<TableNode>(new TableNode);
    theTable->numRows=4;
    theTable->numCols=3;
    theTable->tableEntries[pointToString(Point(1,0))]=shared_ptr<TextNode>(new TextNode("Beginning"));
    theTable->tableEntries[pointToString(Point(2,0))]=shared_ptr<TextNode>(new TextNode("Ending"));
    theTable->tableEntries[pointToString(Point(0,1))]=shared_ptr<TextNode>(new TextNode("Profit"));
    theTable->tableEntries[pointToString(Point(0,2))]=shared_ptr<TextNode>(new TextNode("Loss"));
    theTable->tableEntries[pointToString(Point(0,3))]=shared_ptr<TextNode>(new TextNode("Capital"));
    theTable->tableEntries[pointToString(Point(1,1))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    theTable->tableEntries[pointToString(Point(2,1))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    theTable->tableEntries[pointToString(Point(1,2))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    theTable->tableEntries[pointToString(Point(2,2))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    theTable->tableEntries[pointToString(Point(1,3))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    theTable->tableEntries[pointToString(Point(2,3))]=shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC));
    nodes.push_back(theTable); // 29


    nodes.push_back(shared_ptr<TextNode>(new TextNode("K"))); // 30
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's share of liabilities at year end"))); // 31
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Nonrecourse"))); // 32
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 33
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Qualified nonrecourse financing"))); // 34
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 35
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Recourse"))); // 36
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 37

    nodes.push_back(shared_ptr<TextNode>(new TextNode("L"))); // 38
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's capital account analysis:"))); // 39
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Beinning capital account"))); // 40
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 41
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Capital conributed during the year"))); // 42
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 43
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Current year increase (decrease)"))); // 44
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 45
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Withdrawals & distributions"))); // 46
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 47
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Ending capital account"))); // 48
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 49
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Tax basis"))); // 50
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 51
    nodes.push_back(shared_ptr<TextNode>(new TextNode("GAAP"))); // 52
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 53
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Section 704(b) book"))); // 54
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 55
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Other (explain)"))); // 56
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 57

    nodes.push_back(shared_ptr<TextNode>(new TextNode("M"))); // 58
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Did the partner contribute property with a built-in gain or loss?"))); // 59
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Yes"))); // 60
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 61
    nodes.push_back(shared_ptr<TextNode>(new TextNode("No"))); // 62
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_CHECKBOX))); // 63
    nodes.push_back(shared_ptr<TextNode>(new TextNode("If \"Yes,\" attach statement (see instructions)"))); // 64

    shared_ptr<Node>x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[0]);
    x->subNodes.push_back(nodes[1]);
    nodes2.push_back(x); //0

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[2]);
    x->subNodes.push_back(nodes[3]);
    x->subNodes.push_back(nodes[4]);
    nodes2.push_back(x); //1

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[5]);
    x->subNodes.push_back(nodes[6]);
    x->subNodes.push_back(nodes[7]);
    nodes2.push_back(x); //2

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[8]);
    x->subNodes.push_back(nodes[9]);
    x->subNodes.push_back(nodes[10]);
    x->subNodes.push_back(nodes[11]);
    x->subNodes.push_back(nodes[12]);
    x->subNodes.push_back(nodes[13]);
    x->subNodes.push_back(nodes[15]);
    nodes2.push_back(x); //3

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[14]);
    x->subNodes.push_back(nodes[19]);
    x->subNodes.push_back(nodes[16]);
    x->subNodes.push_back(nodes[17]);
    x->subNodes.push_back(nodes[18]);
    nodes2.push_back(x); //4

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[20]);
    x->subNodes.push_back(nodes[21]);
    x->subNodes.push_back(nodes[22]);
    nodes2.push_back(x); //5

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[23]);
    x->subNodes.push_back(nodes[24]);
    x->subNodes.push_back(nodes[25]);
    x->subNodes.push_back(nodes[26]);
    nodes2.push_back(x); //6

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[27]);
    x->subNodes.push_back(nodes[28]);
    x->subNodes.push_back(nodes[29]);
    nodes2.push_back(x); //7

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[30]);
    x->subNodes.push_back(nodes[31]);
    x->subNodes.push_back(nodes[32]);
    x->subNodes.push_back(nodes[33]);
    x->subNodes.push_back(nodes[34]);
    x->subNodes.push_back(nodes[35]);
    x->subNodes.push_back(nodes[36]);
    x->subNodes.push_back(nodes[37]);
    nodes2.push_back(x); //8

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[38]);
    x->subNodes.push_back(nodes[39]);
    x->subNodes.push_back(nodes[40]);
    x->subNodes.push_back(nodes[41]);
    x->subNodes.push_back(nodes[42]);
    x->subNodes.push_back(nodes[43]);
    x->subNodes.push_back(nodes[44]);
    x->subNodes.push_back(nodes[45]);
    x->subNodes.push_back(nodes[46]);
    x->subNodes.push_back(nodes[47]);
    x->subNodes.push_back(nodes[48]);
    x->subNodes.push_back(nodes[49]);
    x->subNodes.push_back(nodes[50]);
    x->subNodes.push_back(nodes[51]);
    x->subNodes.push_back(nodes[52]);
    x->subNodes.push_back(nodes[53]);
    x->subNodes.push_back(nodes[54]);
    x->subNodes.push_back(nodes[55]);
    x->subNodes.push_back(nodes[56]);
    x->subNodes.push_back(nodes[57]);
    nodes2.push_back(x); //9

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[58]);
    x->subNodes.push_back(nodes[59]);
    x->subNodes.push_back(nodes[60]);
    x->subNodes.push_back(nodes[61]);
    x->subNodes.push_back(nodes[62]);
    x->subNodes.push_back(nodes[63]);
    x->subNodes.push_back(nodes[64]);
    nodes2.push_back(x); //10

    for_each(nodes2.begin(),nodes2.end(), [&](shared_ptr<Node>n) {
        this->part2->subNodes.push_back(n);
    });

    // Add rules
    isRightTo(nodes[1],nodes[0]);
    isBelow(nodes[4],nodes[3]);
    isRightTo(nodes[3],nodes[2]);
    isBelow(nodes[7],nodes[6]);
    isRightTo(nodes[3],nodes[2]);

    isRightTo(nodes[9],nodes[8]);
    isRightTo(nodes[12],nodes[9]);
    isRightTo(nodes[13],nodes[9]);
    isBelow(nodes[10],nodes[9]);
    isBelow(nodes[13],nodes[12]);
    isRightTo(nodes[9],nodes[11]);
    isRightTo(nodes[12],nodes[15]);
    isRightTo(nodes[9],nodes[11]);

    isRightTo(nodes[16],nodes[14]);
    isRightTo(nodes[18],nodes[16]);
    isRightTo(nodes[17],nodes[14]);
    isRightTo(nodes[16],nodes[17]);
    isRightTo(nodes[19],nodes[16]);
    isRightTo(nodes[18],nodes[19]);

    isRightTo(nodes[21],nodes[20]);
    isRightTo(nodes[22],nodes[21]);

    isRightTo(nodes[24],nodes[23]);
    isRightTo(nodes[26],nodes[23]);
    isBelow(nodes[25],nodes[24]);

    isBelow(nodes[29],nodes[28]);

    isRightTo(nodes[31],nodes[30]);
    isBelow(nodes[32],nodes[31]);
    isBelow(nodes[34],nodes[32]);
    isBelow(nodes[36],nodes[34]);
    isRightTo(nodes[33],nodes[32]);
    isAbove(nodes[33],nodes[34]);
    isRightTo(nodes[35],nodes[34]);
    isBelow(nodes[35],nodes[32]);
    isAbove(nodes[35],nodes[36]);
    isRightTo(nodes[37],nodes[36]);
    isBelow(nodes[37],nodes[34]);


    isRightTo(nodes[39],nodes[38]);
    isBelow(nodes[40],nodes[39]);
    isBelow(nodes[42],nodes[40]);
    isBelow(nodes[44],nodes[42]);
    isBelow(nodes[46],nodes[44]);
    isBelow(nodes[48],nodes[46]);
    isBelow(nodes[50],nodes[48]);
    isBelow(nodes[56],nodes[40]);
    isRightTo(nodes[52],nodes[50]);
    isRightTo(nodes[54],nodes[52]);
    isBelow(nodes[41],nodes[39]);
    isRightTo(nodes[41],nodes[40]);
    isAbove(nodes[41],nodes[42]);
    isBelow(nodes[43],nodes[40]);
    isRightTo(nodes[43],nodes[42]);
    isAbove(nodes[43],nodes[44]);
    isBelow(nodes[45],nodes[42]);
    isRightTo(nodes[45],nodes[44]);
    isAbove(nodes[45],nodes[46]);
    isBelow(nodes[47],nodes[44]);
    isRightTo(nodes[47],nodes[46]);
    isAbove(nodes[47],nodes[48]);
    isBelow(nodes[49],nodes[46]);
    isRightTo(nodes[49],nodes[48]);
    isAbove(nodes[49],nodes[50]);
    isRightTo(nodes[50],nodes[51]);
    isRightTo(nodes[52],nodes[53]);
    isRightTo(nodes[54],nodes[55]);
    isRightTo(nodes[56],nodes[57]);

    isRightTo(nodes[59],nodes[58]);
    isBelow(nodes[60],nodes[59]);
    isBelow(nodes[62],nodes[59]);
    isRightTo(nodes[62],nodes[60]);
    isAbove(nodes[60],nodes[64]);
    isRightTo(nodes[60],nodes[61]);
    isRightTo(nodes[62],nodes[63]);


    isBelow(nodes2[1],nodes2[0]);
    isBelow(nodes2[2],nodes2[1]);
    isBelow(nodes2[3],nodes2[2]);
    isBelow(nodes2[4],nodes2[3]);
    isBelow(nodes2[5],nodes2[4]);
    isBelow(nodes2[6],nodes2[5]);
    isBelow(nodes2[7],nodes2[6]);
    isBelow(nodes2[8],nodes2[7]);
    isBelow(nodes2[9],nodes2[8]);
    isBelow(nodes2[10],nodes2[9]);
}

void ModelBuilder::buildPart3() {
    vector<shared_ptr<Node>>nodes;
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Part III"))); // 0
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Partner's Share of Current Year Income, Deductions, Credits, and Other Items"))); // 1

    nodes.push_back(shared_ptr<TextNode>(new TextNode("1"))); // 2
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Ordinary business income (loss)"))); // 3
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 4
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 5

    nodes.push_back(shared_ptr<TextNode>(new TextNode("2"))); // 6
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Net rental income (loss)"))); // 7
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 8
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 9

    nodes.push_back(shared_ptr<TextNode>(new TextNode("3"))); // 10
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Other net rental income (loss)"))); // 11
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 12
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 13

    nodes.push_back(shared_ptr<TextNode>(new TextNode("4"))); // 14
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Guaranteed payments"))); // 15
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 16
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 17

    nodes.push_back(shared_ptr<TextNode>(new TextNode("5"))); // 18
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Interest income"))); // 19
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 20
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 21

    nodes.push_back(shared_ptr<TextNode>(new TextNode("6a"))); // 22
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Ordinary dividends"))); // 23
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 24
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 25

    nodes.push_back(shared_ptr<TextNode>(new TextNode("6b"))); // 26
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Qualified dividends"))); // 27
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 28
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 29

    nodes.push_back(shared_ptr<TextNode>(new TextNode("7"))); // 30
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Royalties"))); // 31
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 32
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 33

    nodes.push_back(shared_ptr<TextNode>(new TextNode("8"))); // 34
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Net short-term capital gain (loss)"))); // 35
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 36
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 37

    nodes.push_back(shared_ptr<TextNode>(new TextNode("9a"))); // 38
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Net long-term capital gain (loss)"))); // 39
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 40
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 41

    nodes.push_back(shared_ptr<TextNode>(new TextNode("9b"))); // 42
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Collectibles (28%) gain (loss)"))); // 43
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 44
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 45

    nodes.push_back(shared_ptr<TextNode>(new TextNode("9c"))); // 46
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Unrecaptured section 1250 gain"))); // 47
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 48
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 49

    nodes.push_back(shared_ptr<TextNode>(new TextNode("10"))); // 50
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Net section 1231 gain (loss)"))); // 51
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 52
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 53

    nodes.push_back(shared_ptr<TextNode>(new TextNode("11"))); // 54
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Other income (loss)"))); // 55
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 56
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 57

    nodes.push_back(shared_ptr<TextNode>(new TextNode("12"))); // 58
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Section 179 deduction"))); // 59
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 60
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 61

    nodes.push_back(shared_ptr<TextNode>(new TextNode("13"))); // 62
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Other deductions"))); // 63
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 64
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 65

    nodes.push_back(shared_ptr<TextNode>(new TextNode("14"))); // 66
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Self-employment earnings (loss)"))); // 67
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 68
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 69

    nodes.push_back(shared_ptr<TextNode>(new TextNode("15"))); // 70
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Credits"))); // 71
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 72
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 73

    nodes.push_back(shared_ptr<TextNode>(new TextNode("16"))); // 74
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Foreign transactions"))); // 75
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 76
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 77

    nodes.push_back(shared_ptr<TextNode>(new TextNode("17"))); // 78
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Alternative minimum tax (AMT) items"))); // 79
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 80
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 81

    nodes.push_back(shared_ptr<TextNode>(new TextNode("18"))); // 82
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Tax-exempt income"))); // 83
    nodes.push_back(shared_ptr<TextNode>(new TextNode("and nondeductable expenses"))); // 84
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 85
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 86

    nodes.push_back(shared_ptr<TextNode>(new TextNode("19"))); // 87
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Distributions"))); // 88
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 89
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 90

    nodes.push_back(shared_ptr<TextNode>(new TextNode("20"))); // 91
    nodes.push_back(shared_ptr<TextNode>(new TextNode("Other information"))); // 92
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_ALPHA_NUMERIC))); // 93
    nodes.push_back(shared_ptr<InputNode>(new InputNode(InputNode::INPUT_NUMERIC))); // 94

    vector<shared_ptr<Node>>nodes2;

    shared_ptr<Node>x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[0]);
    x->subNodes.push_back(nodes[1]);
    nodes2.push_back(x); //0

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[2]);
    x->subNodes.push_back(nodes[3]);
    x->subNodes.push_back(nodes[4]);
    x->subNodes.push_back(nodes[5]);
    nodes2.push_back(x); //1

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[6]);
    x->subNodes.push_back(nodes[7]);
    x->subNodes.push_back(nodes[8]);
    x->subNodes.push_back(nodes[9]);
    nodes2.push_back(x); //2

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[10]);
    x->subNodes.push_back(nodes[11]);
    x->subNodes.push_back(nodes[12]);
    x->subNodes.push_back(nodes[13]);
    nodes2.push_back(x); //3

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[14]);
    x->subNodes.push_back(nodes[15]);
    x->subNodes.push_back(nodes[16]);
    x->subNodes.push_back(nodes[17]);
    nodes2.push_back(x); //4

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[18]);
    x->subNodes.push_back(nodes[19]);
    x->subNodes.push_back(nodes[20]);
    x->subNodes.push_back(nodes[21]);
    nodes2.push_back(x); //5

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[22]);
    x->subNodes.push_back(nodes[23]);
    x->subNodes.push_back(nodes[24]);
    x->subNodes.push_back(nodes[25]);
    nodes2.push_back(x); //6

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[26]);
    x->subNodes.push_back(nodes[27]);
    x->subNodes.push_back(nodes[28]);
    x->subNodes.push_back(nodes[29]);
    nodes2.push_back(x); //7

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[30]);
    x->subNodes.push_back(nodes[31]);
    x->subNodes.push_back(nodes[32]);
    x->subNodes.push_back(nodes[33]);
    nodes2.push_back(x); //8

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[34]);
    x->subNodes.push_back(nodes[35]);
    x->subNodes.push_back(nodes[36]);
    x->subNodes.push_back(nodes[37]);
    nodes2.push_back(x); //9

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[38]);
    x->subNodes.push_back(nodes[39]);
    x->subNodes.push_back(nodes[40]);
    x->subNodes.push_back(nodes[41]);
    nodes2.push_back(x); //10

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[42]);
    x->subNodes.push_back(nodes[43]);
    x->subNodes.push_back(nodes[44]);
    x->subNodes.push_back(nodes[45]);
    nodes2.push_back(x); //11

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[46]);
    x->subNodes.push_back(nodes[47]);
    x->subNodes.push_back(nodes[48]);
    x->subNodes.push_back(nodes[49]);
    nodes2.push_back(x); //12

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[50]);
    x->subNodes.push_back(nodes[51]);
    x->subNodes.push_back(nodes[52]);
    x->subNodes.push_back(nodes[53]);
    nodes2.push_back(x); //13

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[54]);
    x->subNodes.push_back(nodes[55]);
    x->subNodes.push_back(nodes[56]);
    x->subNodes.push_back(nodes[57]);
    nodes2.push_back(x); //14

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[58]);
    x->subNodes.push_back(nodes[59]);
    x->subNodes.push_back(nodes[60]);
    x->subNodes.push_back(nodes[61]);
    nodes2.push_back(x); //15

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[62]);
    x->subNodes.push_back(nodes[63]);
    x->subNodes.push_back(nodes[64]);
    x->subNodes.push_back(nodes[65]);
    nodes2.push_back(x); //16

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[66]);
    x->subNodes.push_back(nodes[67]);
    x->subNodes.push_back(nodes[68]);
    x->subNodes.push_back(nodes[69]);
    nodes2.push_back(x); //17

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[70]);
    x->subNodes.push_back(nodes[71]);
    x->subNodes.push_back(nodes[72]);
    x->subNodes.push_back(nodes[73]);
    nodes2.push_back(x); //18

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[74]);
    x->subNodes.push_back(nodes[75]);
    x->subNodes.push_back(nodes[76]);
    x->subNodes.push_back(nodes[77]);
    nodes2.push_back(x); //19

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[78]);
    x->subNodes.push_back(nodes[79]);
    x->subNodes.push_back(nodes[80]);
    x->subNodes.push_back(nodes[81]);
    nodes2.push_back(x); //20

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[82]);
    x->subNodes.push_back(nodes[83]);
    x->subNodes.push_back(nodes[84]);
    x->subNodes.push_back(nodes[85]);
    x->subNodes.push_back(nodes[86]);
    nodes2.push_back(x); //21

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[87]);
    x->subNodes.push_back(nodes[88]);
    x->subNodes.push_back(nodes[89]);
    x->subNodes.push_back(nodes[90]);
    nodes2.push_back(x); //22

    x=shared_ptr<Node>(new Node);
    x->subNodes.push_back(nodes[91]);
    x->subNodes.push_back(nodes[92]);
    x->subNodes.push_back(nodes[93]);
    x->subNodes.push_back(nodes[94]);
    nodes2.push_back(x); //23

    nodes.push_back(shared_ptr<TextNode>(new TextNode("*See attached statement for additional information."))); // 95

    nodes2.push_back(nodes[95]); //24

    for_each(nodes2.begin(),nodes2.end(), [&](shared_ptr<Node>n) {
        this->part3->subNodes.push_back(n);
    });

    // Add rules
    isRightTo(nodes[1],nodes[0]);

    isRightTo(nodes[3],nodes[2]);
    isBelow(nodes[4],nodes[2]);
    isBelow(nodes[5],nodes[3]);
    isRightTo(nodes[5],nodes[4]);

    isRightTo(nodes[7],nodes[6]);
    isBelow(nodes[8],nodes[6]);
    isBelow(nodes[9],nodes[7]);
    isRightTo(nodes[9],nodes[8]);

    isRightTo(nodes[11],nodes[10]);
    isBelow(nodes[12],nodes[10]);
    isBelow(nodes[13],nodes[11]);
    isRightTo(nodes[13],nodes[12]);

    isRightTo(nodes[15],nodes[14]);
    isBelow(nodes[16],nodes[14]);
    isBelow(nodes[17],nodes[15]);
    isRightTo(nodes[17],nodes[16]);

    isRightTo(nodes[19],nodes[18]);
    isBelow(nodes[20],nodes[18]);
    isBelow(nodes[21],nodes[19]);
    isRightTo(nodes[21],nodes[20]);

    isRightTo(nodes[23],nodes[22]);
    isBelow(nodes[24],nodes[22]);
    isBelow(nodes[25],nodes[23]);
    isRightTo(nodes[25],nodes[24]);

    isRightTo(nodes[27],nodes[26]);
    isBelow(nodes[28],nodes[26]);
    isBelow(nodes[29],nodes[27]);
    isRightTo(nodes[29],nodes[28]);

    isRightTo(nodes[31],nodes[30]);
    isBelow(nodes[32],nodes[30]);
    isBelow(nodes[33],nodes[31]);
    isRightTo(nodes[33],nodes[32]);

    isRightTo(nodes[35],nodes[34]);
    isBelow(nodes[36],nodes[34]);
    isBelow(nodes[37],nodes[35]);
    isRightTo(nodes[37],nodes[36]);

    isRightTo(nodes[39],nodes[38]);
    isBelow(nodes[40],nodes[38]);
    isBelow(nodes[41],nodes[39]);
    isRightTo(nodes[41],nodes[40]);

    isRightTo(nodes[43],nodes[42]);
    isBelow(nodes[44],nodes[42]);
    isBelow(nodes[45],nodes[43]);
    isRightTo(nodes[45],nodes[44]);

    isRightTo(nodes[47],nodes[46]);
    isBelow(nodes[48],nodes[46]);
    isBelow(nodes[49],nodes[47]);
    isRightTo(nodes[49],nodes[48]);

    isRightTo(nodes[51],nodes[50]);
    isBelow(nodes[52],nodes[50]);
    isBelow(nodes[53],nodes[51]);
    isRightTo(nodes[53],nodes[52]);

    isRightTo(nodes[55],nodes[54]);
    isBelow(nodes[56],nodes[54]);
    isBelow(nodes[57],nodes[55]);
    isRightTo(nodes[57],nodes[56]);

    isRightTo(nodes[59],nodes[58]);
    isBelow(nodes[60],nodes[58]);
    isBelow(nodes[61],nodes[59]);
    isRightTo(nodes[61],nodes[60]);

    isRightTo(nodes[63],nodes[62]);
    isBelow(nodes[64],nodes[62]);
    isBelow(nodes[65],nodes[63]);
    isRightTo(nodes[65],nodes[64]);

    isRightTo(nodes[67],nodes[66]);
    isBelow(nodes[68],nodes[66]);
    isBelow(nodes[69],nodes[67]);
    isRightTo(nodes[69],nodes[68]);

    isRightTo(nodes[71],nodes[70]);
    isBelow(nodes[72],nodes[70]);
    isBelow(nodes[73],nodes[71]);
    isRightTo(nodes[73],nodes[72]);

    isRightTo(nodes[75],nodes[74]);
    isBelow(nodes[76],nodes[74]);
    isBelow(nodes[77],nodes[75]);
    isRightTo(nodes[77],nodes[76]);

    isRightTo(nodes[79],nodes[78]);
    isBelow(nodes[80],nodes[78]);
    isBelow(nodes[81],nodes[79]);
    isRightTo(nodes[81],nodes[80]);

    isRightTo(nodes[83],nodes[82]);
    isBelow(nodes[84],nodes[83]);
    isBelow(nodes[85],nodes[82]);
    isBelow(nodes[86],nodes[84]);
    isRightTo(nodes[86],nodes[85]);

    isRightTo(nodes[88],nodes[87]);
    isBelow(nodes[89],nodes[87]);
    isBelow(nodes[90],nodes[88]);
    isRightTo(nodes[90],nodes[89]);

    isRightTo(nodes[92],nodes[91]);
    isBelow(nodes[93],nodes[91]);
    isBelow(nodes[94],nodes[92]);
    isRightTo(nodes[94],nodes[93]);

    isBelow(nodes2[1],nodes2[0]);
    isBelow(nodes2[2],nodes2[1]);
    isBelow(nodes2[3],nodes2[2]);
    isBelow(nodes2[4],nodes2[3]);
    isBelow(nodes2[5],nodes2[4]);
    isBelow(nodes2[6],nodes2[5]);
    isBelow(nodes2[7],nodes2[6]);
    isBelow(nodes2[8],nodes2[7]);
    isBelow(nodes2[9],nodes2[8]);
    isBelow(nodes2[10],nodes2[9]);
    isBelow(nodes2[11],nodes2[10]);
    isBelow(nodes2[12],nodes2[11]);
    isBelow(nodes2[13],nodes2[12]);
    isBelow(nodes2[14],nodes2[13]);
    isBelow(nodes2[15],nodes2[14]);
    isBelow(nodes2[16],nodes2[15]);
    isBelow(nodes2[17],nodes2[16]);


    isBelow(nodes2[19],nodes2[18]);
    isBelow(nodes2[20],nodes2[19]);
    isBelow(nodes2[21],nodes2[20]);
    isBelow(nodes2[22],nodes2[21]);
    isBelow(nodes2[23],nodes2[22]);
    isBelow(nodes2[24],nodes2[23]);

    isRightTo(nodes2[18],nodes2[1]);

}


void ModelBuilder::convertToJson(Json::Value &jsonOutput, const shared_ptr<Node>&model) {
    if(dynamic_pointer_cast<TextNode>(model)!= nullptr) {
        shared_ptr<TextNode> tModel=dynamic_pointer_cast<TextNode>(model);
        jsonOutput["type"]="static_text";
        jsonOutput["text"]=tModel->text;
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

        unordered_set<int>value=it->second;

        string rr="";
        for_each(value.begin(),value.end(),[&] (const int&x){
            rr+=to_string(x)+",";
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
        for_each(model->subNodes.begin(), model->subNodes.end(), [&](shared_ptr<Node> current) {
            Json::Value jsonV;
            convertToJson(jsonV, current);
            subNodes[x] = jsonV;
            x++;
        });
        jsonOutput["sub_nodes"] = subNodes;
    }
}

void ModelBuilder::isBelow(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<int>>::const_iterator got = a->rulesModel.find ("is_below");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_below"] = unordered_set<int>();
    }
    a->rulesModel["is_below"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_above");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_above"] = unordered_set<int>();
    }
    b->rulesModel["is_above"].insert(a->id);

}


void ModelBuilder::isAbove(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<int>>::const_iterator got = a->rulesModel.find ("is_above");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_above"] = unordered_set<int>();
    }
    a->rulesModel["is_above"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_below");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_below"] = unordered_set<int>();
    }
    b->rulesModel["is_below"].insert(a->id);

}


void ModelBuilder::isRightTo(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<int>>::const_iterator got = a->rulesModel.find ("is_right_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_right_to"] = unordered_set<int>();
    }
    a->rulesModel["is_right_to"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_left_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_left_to"] = unordered_set<int>();
    }
    b->rulesModel["is_left_to"].insert(a->id);

}


void ModelBuilder::isLeftTo(shared_ptr<Node> a, shared_ptr<Node> b) {

    // Set the rule in node a
    unordered_map<string, unordered_set<int>>::const_iterator got = a->rulesModel.find ("is_left_to");
    if(got==a->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        a->rulesModel["is_left_to"] = unordered_set<int>();
    }
    a->rulesModel["is_left_to"].insert(b->id);


    // Set the rule in node b
    got = b->rulesModel.find ("is_right_to");
    if(got==b->rulesModel.end()) {
        // Did not find the rule. Create a new one.
        b->rulesModel["is_right_to"] = unordered_set<int>();
    }
    b->rulesModel["is_right_to"].insert(a->id);

}


void ModelBuilder::orderIds(shared_ptr<Node> node, int currentId) {
    node->id=currentId;

}