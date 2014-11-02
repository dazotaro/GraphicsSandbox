/*
 * main.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "XMLHandler.hpp"

// Global includes
#include <cstdio>	// std::printf

void printTree(boost::property_tree::ptree pt)
{

    for (boost::property_tree::ptree::const_iterator node_iter = pt.begin(); node_iter != pt.end(); ++node_iter)
    {
    	std::printf("Key = %s\n", node_iter->first.c_str());
    	std::printf("Key = %s\n", node_iter->second.get_value<std::string>().c_str());

    	printTree(node_iter->second);
    }


}

int test_hash(void)
{
    const char* filename = "data/ptree_container.xml";

    boost::property_tree::ptree pt;

    JU::XMLHandler::load(filename, pt);

    printTree(pt);

    // Traverse the tree and compute the hash value of the tree

    return 0;
}
