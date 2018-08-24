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

namespace JU { namespace XMLHandler {

bool load(const char* filename, boost::property_tree::ptree& pt)
{
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    // The "trim_whitespace" ensures that the tab-for-spaces do not create empty lines
    read_xml(filename, pt, boost::property_tree::xml_parser::trim_whitespace);
    return true;
}


void save(const char* filename, const boost::property_tree::ptree& pt)
{
    // Set the tab to four spaces
    //boost::property_tree::xml_parser::xml_writer_settings<char> settings(' ', 4);
    auto settings = boost::property_tree::xml_writer_make_settings<std::string> (' ', 4);
    // Write the property tree to the XML file.
    write_xml(filename, pt, std::locale(), settings);
}

} }   // namespace XMLHandler namespace JU
