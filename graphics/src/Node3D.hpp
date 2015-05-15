/*
 * Node3D.hpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#ifndef NODE3D_HPP_
#define NODE3D_HPP_

#include <vector>                   // std::vector
#include <JU/core/Object3D.hpp>     // Object3D
#include "DrawInterface.hpp"    // DrawInterface

// FORWARD DECLARATIONS
class Node3D;

typedef std::vector<Node3D *> NodePointerList;
typedef NodePointerList::const_iterator NodePointerListIterator;

/**
 * @brief      Node of a Complex 3D Object (ie. SceneGraph)
 *
 * @details    It needs to:
 * + Draw itself
 * + Draw all its children
 */
class Node3D : public Object3D, public DrawInterface
{
    public:
        Node3D(const Object3D &object3d,
               const DrawInterface *node_drawable,
               bool visible = true);
        virtual ~Node3D();

        void addChild(Node3D* node);

        virtual void draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const;

    private:
        const DrawInterface *node_drawable_;    //!< Pointer to the 'drawable' data of this node
        NodePointerList children_;              //!< All the children below this level
        bool visible_;                          //!< To draw or not draw, that is the question
};

#endif /* NODE3D_HPP_ */
