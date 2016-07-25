//
// Created by Shah Rukh Qasim on 7/25/16.
//

#include "ModelBuilder.h"


void ModelBuilder::start() {
    ModelBuilder().execute();
}

void ModelBuilder::execute() {
    document=new Node;
    header1=new Node;
    header2=new Node;

    document->subNodes.push_back(header1);
    document->subNodes.push_back(header2);
}

void ModelBuilder::buildHeader1() {
    vector<shared_ptr<Node>>nodes;

    nodes.push_back(new TextNode("Schedule K-1"));
    nodes.push_back(new TextNode("(Form 1065)"));
    nodes.push_back(new TextNode("Department of Treasury"));
    nodes.push_back(new TextNode("Internal Revenue Service"));
    nodes.push_back(new TextNode("Partner's Share of Income, Deductions,"));
    nodes.push_back(new TextNode("Credits, etc."));

    nodes.push_back(new TextNode("See back of form and sepeerate instructions"));
    nodes.push_back(new TextNode("2013"));

    nodes.push_back(new TextNode("For calendar year 2013, or tax"));
    nodes.push_back(new TextNode("year beginning"));
    nodes.push_back(new TextNode(",2013"));
    nodes.push_back(new TextNode("ending"));
    nodes.push_back(new TextNode(",20"));
    nodes.push_back(new InputNode(InputNode::INPUT_NUMERIC));
    nodes.push_back(new InputNode(InputNode::INPUT_NUMERIC));
    nodes.push_back(new InputNode(InputNode::INPUT_NUMERIC));
}


void ModelBuilder::buildHeader2() {
    vector<shared_ptr<Node>>nodes;

    nodes.push_back(new TextNode("Final K-1"));
    nodes.push_back(new TextNode("Ammended K-1"));
    nodes.push_back(new TextNode("OMB No. 1545-0099"));
    nodes.push_back(new TextNode("651113"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
}


void ModelBuilder::buildPart1() {
    vector<shared_ptr<Node>>nodes;
    nodes.push_back(new TextNode("Part I"));
    nodes.push_back(new TextNode("Information About the Partnership"));
    nodes.push_back(new TextNode("A"));
    nodes.push_back(new TextNode("Partnerhip's employer identification number"));
    nodes.push_back(new TextNode("B"));
    nodes.push_back(new TextNode("Partnership's name, address, city, state and ZIP code"));
    nodes.push_back(new TextNode("C"));
    nodes.push_back(new TextNode("IRS Center where partnership filed return"));
    nodes.push_back(new TextNode("D"));
    nodes.push_back(new TextNode("Check if this is a publicly traded partnership (PTP)"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));

}

void ModelBuilder::buildPart2() {
    vector<shared_ptr<Node>>nodes;
    nodes.push_back(new TextNode("Part II"));
    nodes.push_back(new TextNode("Information About the Partner"));
    nodes.push_back(new TextNode("E"));
    nodes.push_back(new TextNode("Partner's identifying number"));
    nodes.push_back(new TextNode("F"));
    nodes.push_back(new TextNode("Partner's name, address, city, state and ZIP code"));
    nodes.push_back(new TextNode("G"));
    nodes.push_back(new TextNode("Gerneral partner or LLC"));
    nodes.push_back(new TextNode("member-manager"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new TextNode("Limited partner or other LLC"));
    nodes.push_back(new TextNode("member"));
    nodes.push_back(new TextNode("H"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new TextNode("Domestic Partner"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new TextNode("Foreign partner"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new TextNode("I1"));
    nodes.push_back(new TextNode("What type of entity is this partner?"));
    nodes.push_back(new InputNode(InputNode::INPUT_ALPHA_NUMERIC));
    nodes.push_back(new TextNode("I2"));
    nodes.push_back(new TextNode("If this partner is a retirement plan (IRA/SEP/Keogh/etc.), check here"));
    nodes.push_back(new TextNode("(see instructions)"));
    nodes.push_back(new InputNode(InputNode::INPUT_CHECKBOX));
    nodes.push_back(new TextNode("J"));
    nodes.push_back(new TextNode("Partner's share of profit, loss, and capital (see instructions):"));
}