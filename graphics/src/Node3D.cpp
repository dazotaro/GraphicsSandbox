/*
 * Node3D.cpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#include "Node3D.hpp"
#include "GLSLProgram.hpp"  // GLSLProgram

/**
* @brief Default Constructor
*
* @detail Perform intentional shallow copies of pointers. It is the responsability of the caller to delete dynamically allocated parameters
*
* @param object         Object3D containing the position and orientation of this node in the parent coordinate system
* @param node_drawable  Pointer to the object with the info to draw this node
* @param children       Vector with pointers to the children nodes
* @param visible        Even if the node_drawable pointer is not null, it exits the possibility that, at some point, we might not want to draw this node
*/
Node3D::Node3D(const Object3D &object,
               const DrawInterface *node_drawable,
               bool visible) :
               Object3D(object), node_drawable_(node_drawable), visible_(visible)
{
}

/**
* @brief Destructor
*/
Node3D::~Node3D()
{
    // Delete all dynamically allocated children
    for (NodePointerListIterator iter = children_.begin(); iter != children_.end(); ++iter)
    {
        delete *iter;
    }
}



/**
* @brief Add new child node
*
* @param node  New node
*/
void Node3D::addChild(Node3D* node)
{
	children_.push_back(node);
}



/**
* @brief Draw this node
*
* @detail Draw this node and all its parts (if more than one), and then draw all its children
*
* @param shader_program     Handle to the shader program
* @param model              Model matrix
* @param view               View matrix
* @param projection         Projection matrix
*/
void Node3D::draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const
{
    glm::mat4 new_model = model * getTransformToParent();

    if (visible_)
    {
        node_drawable_->draw(program, new_model, view, projection);
    }

    // Draw the children
    for(NodePointerListIterator iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->draw(program, new_model, view, projection);
    }
}

