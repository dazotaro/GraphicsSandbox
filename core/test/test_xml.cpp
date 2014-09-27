/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

#include "XMLHandler.hpp"

int test_xml(void)
{
    const char* filename = "data/debug.xml";

    boost::property_tree::ptree pt;

    JU::XMLHandler::load(filename, pt);
    JU::XMLHandler::save("after.xml", pt);

    return 0;
}
