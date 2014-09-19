/*
 * XMLHandler.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "XMLHandler.hpp"
// Global includes
#include <boost/property_tree/xml_parser.hpp>                           // read_xml(), write_xml()
#include <boost/property_tree/detail/xml_parser_writer_settings.hpp>    // xml_parser_writer_settings
#include <locale>                                                       // std::locale()

bool XMLHandler::load(const char* filename, boost::property_tree::ptree& pt)
{
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    read_xml(filename, pt);

    return true;
}


void XMLHandler::save(const char* filename, const boost::property_tree::ptree& pt) const
{

}

