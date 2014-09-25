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
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt) = 0;
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const = 0;
};

} /* namespace JU */
#endif /* PROPERTYTREE_HPP_ */
