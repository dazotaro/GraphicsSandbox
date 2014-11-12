/*
 * PropertyTree.hpp
 *
 *  Created on: Sep 23, 2014
 *      Author: jusabiaga
 */

#ifndef PROPERTYTREEINTERFACE_HPP_
#define PROPERTYTREEINTERFACE_HPP_

// Global Includes
#include <boost/property_tree/ptree.hpp>    // ptree


namespace JU
{

class PropertyTreeInterface
{
    public:
        virtual ~PropertyTreeInterface() {}

    public:
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt) = 0;
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const = 0;
};

} /* namespace JU */
#endif /* PROPERTYTREEINTERFACE_HPP_ */
