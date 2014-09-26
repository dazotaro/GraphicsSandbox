/*
 * TestClass.hpp
 *
 *  Created on: Sep 25, 2014
 *      Author: jusabiaga
 */

#ifndef TESTCLASS_HPP_
#define TESTCLASS_HPP_

// Local includes
#include "PropertyTree.hpp"

// Global includes
#include <string>           // std::string



class BaseClass : public JU::PropertyTree
{
    public:
        virtual ~BaseClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "base" ;}
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const;

    public:
        int     integer_;
        float   floating_;
        bool    boolean_;
};



class DerivedClass : public BaseClass
{
    public:
        virtual ~DerivedClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "derived"; }
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
};



class AggregationClass : public JU::PropertyTree
{
    public:
        virtual ~AggregationClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "aggregate"; }
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
        BaseClass   base_class_;
};

#endif /* TESTCLASS_HPP_ */
