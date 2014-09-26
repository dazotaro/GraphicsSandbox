/*
 * TestClass.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: jusabiaga
 */

#include "TestClass.hpp"

bool BaseClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    integer_  = pt.get<int>("int");
    floating_ = pt.get<float>("float");
    boolean_  = pt.get<bool>("bool");

    return true;
}



bool BaseClass::exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const
{
    std::string local_path (buildLocalPath(path, BaseClass::getTag()));

    pt.put(local_path + "int",   integer_);
    pt.put(local_path + "float", floating_);
    pt.put(local_path + "bool",  boolean_);

    return true;
}



bool DerivedClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("string");

    boost::property_tree::ptree base_ptree = pt.get_child("base");
    BaseClass::importFromPropertyTree(base_ptree);

    return true;
}



bool DerivedClass::exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const
{
    std::string local_path (buildLocalPath(path, DerivedClass::getTag()));

    BaseClass::exportToPropertyTree(local_path, pt);
    pt.put(local_path + "string", string_);

    return true;
}



bool AggregationClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("string");

    boost::property_tree::ptree base_ptree = pt.get_child("base");
    base_class_.importFromPropertyTree(base_ptree);

    return true;
}



bool AggregationClass::exportToPropertyTree(const std::string& path, boost::property_tree::ptree& pt) const
{
    std::string local_path (buildLocalPath(path, AggregationClass::getTag()));

    pt.put(local_path + "string", string_);
    base_class_.exportToPropertyTree(local_path, pt);

    return true;
}

