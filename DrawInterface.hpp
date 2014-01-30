/*
 * DrawInterface.hpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#ifndef DRAWINTERFACE_HPP_
#define DRAWINTERFACE_HPP_

#include <glm/glm.hpp>      // glm::mat4
#include <vector>           // std::vector

// FORWARD DECLARATIONS
class GLSLProgram;

/**
 * @brief      Pure Virtual Class to draw objects
 *
 * @details    All derived classes will need to implement the 'draw' function
 */
class DrawInterface
{
    public:
        virtual ~DrawInterface() {}
        virtual void draw(const GLSLProgram &program, const glm::mat4 & model, const glm::mat4 &view, const glm::mat4 &projection) const = 0;
};

typedef std::vector<DrawInterface *> DrawList;
typedef DrawList::const_iterator DrawListIterator;

#endif /* DRAWINTERFACE_HPP_ */
