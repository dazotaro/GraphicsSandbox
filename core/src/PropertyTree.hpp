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
        virtual bool exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const = 0;

    protected:
        // Utility function to build the path to use for export.
        // For example: "this" class' tag is 'rotation', and 'path' is "scene.sphere.",
        // it returns a string with "scene.sphere.rotation."
        // IMPORTANT: Make sure that the classes that use this method pass a 'suffix' that it is obtained with
        // their own 'getTag()' function and not 'this->getTag(), as that could call the method of a derived/children class
        // (see examples in TestClass)
        virtual std::string buildLocalPath(const std::string& prefix_path, const std::string& suffix_path) const
        {
            return std::string (prefix_path + suffix_path + ".");
        }
};

} /* namespace JU */
#endif /* PROPERTYTREE_HPP_ */
