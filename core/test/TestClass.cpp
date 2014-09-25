/*
 * TestClass.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"

bool BaseClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    integer_  = pt.get<int>("test.int");
    floating_ = pt.get<float>("test.float");
    boolean_  = pt.get<bool>("test.bool");

    return true;
}



bool BaseClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    pt.put("test.int",   integer_);
    pt.put("test.float", floating_);
    pt.put("test.bool",  boolean_);

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



bool OtherClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    return true;
}



bool OtherClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    return true;
}

