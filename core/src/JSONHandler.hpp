/*
 * XMLHandler.hpp
 *
 *  Created on: Sep 19, 2014
 *      Author: jusabiaga
 */

#ifndef JSONHANDLER_HPP_
#define JSONHANDLER_HPP_

// Global Includes
#include <boost/property_tree/ptree.hpp>    // ptree

namespace JU {

// XML Import and Export class
namespace JSONHandler
{
    bool load(const char* filename, boost::property_tree::ptree& pt);
    void save(const char* filename, const boost::property_tree::ptree&);
}

}
#endif /* JSONHANDLER_HPP_ */
