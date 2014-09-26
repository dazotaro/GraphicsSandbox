/*
 * PropertyTree.hpp
 *
 *  Created on: Sep 23, 2014
 *      Author: jusabiaga
 */

#ifndef PROPERTYTREE_HPP_
#define PROPERTYTREE_HPP_

// Global Includes
#include <boost/property_tree/ptree.hpp>    // ptree


namespace JU
{

class PropertyTree
{
    public:
        virtual ~PropertyTree() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const = 0;
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt) = 0;
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const = 0;
};

} /* namespace JU */
#endif /* PROPERTYTREE_HPP_ */
