/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"
#include "XMLHandler.hpp"
#include "JSONHandler.hpp"


void testBaseClass()
{
    const char* input_filename = "data/ptree_base.xml";
    const char* output_filename = "ptree_base_after.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(input_filename, pt);

    BaseClass object;
    pt = pt.get_child(object.getTag());

    object.importFromPropertyTree(pt);
    object.exportToPropertyTree("", pt2);

    JU::XMLHandler::save(output_filename, pt2);
}



void testAggregationClass()
{
    const char* input_filename  = "data/ptree_aggregation.xml";
    const char* output_filename = "ptree_aggregation_after.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(input_filename, pt);

    AggregationClass object;
    pt = pt.get_child(object.getTag());

    object.importFromPropertyTree(pt);
    object.exportToPropertyTree("", pt2);

    JU::XMLHandler::save(output_filename, pt2);
}



void testDerivedClass()
{
    const char* input_filename  = "data/ptree_derived.xml";
    const char* output_filename = "ptree_derived_after.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(input_filename, pt);

    DerivedClass object;
    pt = pt.get_child(object.getTag());

    object.importFromPropertyTree(pt);
    object.exportToPropertyTree("", pt2);

    JU::XMLHandler::save(output_filename, pt2);
}



void testXML2JSON()
{
    const char* input_filename = "data/ptree_aggregation.xml";
    const char* output_filename = "ptree_xml2json_after.json";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::XMLHandler::load(input_filename, pt);

    AggregationClass object;
    pt = pt.get_child(object.getTag());

    object.importFromPropertyTree(pt);
    object.exportToPropertyTree("", pt2);

    JU::JSONHandler::save(output_filename, pt2);
}



void testJSON2XML()
{
    const char* input_filename = "data/ptree_aggregation.json";
    const char* output_filename = "ptree_json2xml_after.xml";

    boost::property_tree::ptree pt;
    boost::property_tree::ptree pt2;

    JU::JSONHandler::load(input_filename, pt);

    AggregationClass object;
    pt = pt.get_child(object.getTag());

    object.importFromPropertyTree(pt);
    object.exportToPropertyTree("", pt2);

    JU::XMLHandler::save(output_filename, pt2);
}



int main(void)
{
    testBaseClass();
    testAggregationClass();
    testDerivedClass();
    testXML2JSON();
    testJSON2XML();

    return 0;
}
