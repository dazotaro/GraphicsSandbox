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



bool BaseClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    boost::property_tree::ptree& node = pt.add(BaseClass::getTag(), "");

    node.put("int",   integer_);
    node.put("float", floating_);
    node.put("bool",  boolean_);

    return true;
}



bool DerivedClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("string");

    boost::property_tree::ptree base_ptree = pt.get_child(BaseClass::getTag());
    BaseClass::importFromPropertyTree(base_ptree);

    return true;
}



bool DerivedClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    boost::property_tree::ptree& node (pt.add(DerivedClass::getTag(), ""));

    BaseClass::exportToPropertyTree(node);
    node.put("string", string_);

    return true;
}



bool AggregationClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("string");

    boost::property_tree::ptree base_ptree = pt.get_child(base_class_.getTag());
    base_class_.importFromPropertyTree(base_ptree);

    return true;
}



bool AggregationClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    boost::property_tree::ptree& node (pt.add(AggregationClass::getTag(), ""));

    node.put("string", string_);
    base_class_.exportToPropertyTree(node);

    return true;
}



bool ContainerClass::importFromPropertyTree(const boost::property_tree::ptree& pt)
{
    string_ = pt.get<std::string>("string");

    boost::property_tree::ptree children = pt.get_child("bases");
    for (boost::property_tree::ptree::const_iterator node_iter = children.begin(); node_iter != children.end(); ++node_iter)
    {
        BaseClass object;
        object.importFromPropertyTree(node_iter->second);
        vBase_.push_back(object);
    }


    return true;
}



bool ContainerClass::exportToPropertyTree(boost::property_tree::ptree& pt) const
{
    boost::property_tree::ptree& node = pt.add(ContainerClass::getTag(), "");

    node.put("string", string_);

    // Add node for "bases"
    boost::property_tree::ptree& new_node = node.add("bases", "");

    for (std::vector<BaseClass>::const_iterator iter = vBase_.begin(); iter != vBase_.end(); ++iter)
    {
        iter->exportToPropertyTree(new_node);
    }


    return true;
}


