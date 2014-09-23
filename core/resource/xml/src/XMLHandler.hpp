/*
 * XMLHandler.hpp
 *
 *  Created on: Sep 19, 2014
 *      Author: jusabiaga
 */

#ifndef XMLHANDLER_HPP_
#define XMLHANDLER_HPP_

// Global Includes
#include <boost/property_tree/ptree.hpp>    // ptree

namespace JU {

// XML Import and Export class
namespace XMLHandler
{
    bool load(const char* filename, boost::property_tree::ptree& pt);
    void save(const char* filename, const boost::property_tree::ptree&);
}

}
#endif /* XMLHANDLER_HPP_ */
