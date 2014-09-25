/*
 * TestClass.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"

bool BaseClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    integer_  = pt.get<int>("base.int");
    floating_ = pt.get<float>("base.float");
    boolean_  = pt.get<bool>("base.bool");

    return true;
}



bool BaseClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    pt.put("base.int",   integer_);
    pt.put("base.float", floating_);
    pt.put("base.bool",  boolean_);

    return true;
}



bool DerivedClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    return true;
}



bool DerivedClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    return true;
}



bool AggregationClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("other.string");

    boost::property_tree::ptree base_ptree = pt.get_child("other.base");
    base_class_.importFromPropertyTree(base_ptree);

    return true;
}



bool AggregationClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    pt.put("other.string", string_);
    base_class_.exportToPropertyTree(pt);

    return true;
}

