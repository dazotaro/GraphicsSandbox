/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"
#include "XMLHandler.hpp"


void testBaseClass()
{
    const char* filename = "data/ptree_base.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(filename, pt);

    BaseClass object;
    object.importFromPropertyTree(pt);
    object.exportToPropertyTree(pt2);

    JU::XMLHandler::save("ptree_base_after.xml", pt2);
}


void testAggregationClass()
{
    const char* filename = "data/ptree_aggregation.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(filename, pt);

    AggregationClass object;
    object.importFromPropertyTree(pt);
    object.exportToPropertyTree(pt2);

    JU::XMLHandler::save("ptree_aggregation_after.xml", pt2);
}


int main(void)
{
    testBaseClass();

    return 0;
}
