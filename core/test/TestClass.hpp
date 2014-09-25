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
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        int     integer_;
        float   floating_;
        bool    boolean_;
};



class DerivedClass
{
    public:
        virtual ~DerivedClass() {}

    public:
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
};



class AggregationClass : public JU::PropertyTree
{
    public:
        virtual ~AggregationClass() {}

    public:
        virtual bool importFromPropertyTree(const boost::property_tree::ptree& pt);
        virtual bool exportToPropertyTree(boost::property_tree::ptree& pt) const;

    public:
        std::string string_;
        BaseClass   base_class_;
};

#endif /* TESTCLASS_HPP_ */
