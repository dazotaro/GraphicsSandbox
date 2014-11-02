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
		if (node_iter->second.size())
		{
				printTree(node_iter->second);
		}
		else
			std::printf("Key = %s\n", node_iter->second.get_value<std::string>().c_str());
	}
}

std::size_t computeHash(boost::property_tree::ptree pt, bool shift_left)
{
	static std::hash<std::string> hasher;
	std::size_t hash_value = 0;

	for (boost::property_tree::ptree::const_iterator node_iter = pt.begin(); node_iter != pt.end(); ++node_iter)
	{
		hash_value = hash_value ^ hasher(node_iter->first.c_str());
		if (node_iter->second.size())
		{
			if (shift_left)
				hash_value = hash_value ^ (computeHash(node_iter->second, !shift_left) << 1);
		}
		else
			hash_value = hash_value ^ hasher(node_iter->second.get_value<std::string>().c_str());
	}

	return hash_value;
}

int test_hash(void)
{
    const char* filename0 = "data/hash00.xml";
    boost::property_tree::ptree pt0;
    JU::XMLHandler::load(filename0, pt0);
    std::printf("%s hash value = %li\n", filename0, computeHash(pt0, true));

    const char* filename1 = "data/hash01.xml";
    boost::property_tree::ptree pt1;
    JU::XMLHandler::load(filename1, pt1);
    std::printf("%s hash value = %li\n", filename1, computeHash(pt1, true));

    const char* filename2 = "data/hash02.xml";
    boost::property_tree::ptree pt2;
    JU::XMLHandler::load(filename2, pt2);
    std::printf("%s hash value = %li\n", filename2, computeHash(pt2, true));


    // Traverse the tree and compute the hash value of the tree

    return 0;
}
