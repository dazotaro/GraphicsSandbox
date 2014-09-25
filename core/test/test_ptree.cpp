/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"
#include "XMLHandler.hpp"



int main(void)
{
    const char* filename = "data/ptree.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(filename, pt);

    BaseClass object;
    object.importFromPropertyTree(pt);
    object.exportToPropertyTree(pt2);

    JU::XMLHandler::save("ptree_after.xml", pt2);

    return 0;
}
