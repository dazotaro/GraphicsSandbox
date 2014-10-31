/*
 * main.cpp
 *
 *  Created on: May 14, 2013
 *      Author: jusabiaga
 */

#include "test_xml.cpp"
#include "test_ptree.cpp"
#include "test_hash.cpp"

/* This file is here just to choose among the three different drivers.
    The drivers are hpp files to avoid Eclipse from automatically compiling them causing name collisions */


int main(void)
{
    test_hash();
	//test_ptree();
	//test_xml();

	return 0;
}
