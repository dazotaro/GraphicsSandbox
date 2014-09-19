/*
 * XMLHandler.hpp
 *
 *  Created on: Sep 19, 2014
 *      Author: jusabiaga
 */

#ifndef XMLHANDLER_HPP_
#define XMLHANDLER_HPP_

// Global Includes
#include <boost/property_tree/ptree.hpp>                                // ptree

// XML Import and Export class
class XMLHandler
{
    public:
        XMLHandler() {}
        virtual ~XMLHandler() {}

    public:
        bool load(const char* filename, boost::property_tree::ptree& pt);
        void save(const char* filename, const boost::property_tree::ptree&) const;
};
#endif /* XMLHANDLER_HPP_ */
