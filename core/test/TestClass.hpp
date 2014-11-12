/*
 * TestClass.hpp
 *
 *  Created on: Sep 25, 2014
 *      Author: jusabiaga
 */

#ifndef TESTCLASS_HPP_
#define TESTCLASS_HPP_

// Local includes
#include "PropertyTreeInterface.hpp"

// Global includes
#include <string>           // std::string



class BaseClass : public JU::PropertyTreeInterface
{
    public:
        virtual ~BaseClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "base" ;}
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

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
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
};



class AggregationClass : public JU::PropertyTreeInterface
{
    public:
        virtual ~AggregationClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "aggregate"; }
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
        BaseClass   base_class_;
};



class ContainerClass : public JU::PropertyTreeInterface
{
    public:
        virtual ~ContainerClass() {}

    public:
        // It returns the [XML,JSON] tag of object
        virtual const char* getTag() const { return "container"; }
        // Given a property tree, it imports its contents into the object
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        // It exports the contents of the object to a property tree
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
        std::vector<BaseClass> vBase_;
};

#endif /* TESTCLASS_HPP_ */
