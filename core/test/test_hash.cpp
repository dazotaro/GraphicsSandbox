/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

#include "XMLHandler.hpp"

int test_hash(void)
{
    const char* filename = "data/debug.xml";

    boost::property_tree::ptree pt;

    JU::XMLHandler::load(filename, pt);

    // DEBUG: traverse the tree and output all the data

    // Traverse the tree and compute the hash value of the tree

    return 0;
}
