/*
 * GLScene.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef GLSCENE_HPP_
#define GLSCENE_HPP_

#include <map>              // std::map
#include <string>           // std::string
#include "GLSLProgram.hpp"  // GLSLProgram

// TYPEDEFS
typedef std::map<std::string, GLSLProgram> GLSLProgramMap;
typedef GLSLProgramMap::const_iterator GLSLProgramMapIter;

/*
 * @brief Scene class
 *
 * \todo Maybe unnecessary class
 */
class GLScene
{
    public:
        GLScene(int width, int height);
        virtual ~GLScene();

        // PURE VIRTUAL
        virtual void init() = 0;
        virtual void update(float time) = 0;
        virtual void render(void) = 0;
        virtual void keyboard(unsigned char key, int x, int y) = 0;
        virtual void mouseClick(int button, int state, int x, int y) = 0;
        virtual void mouseMotion(int x, int y) = 0;
        // VIRTUAL
        virtual void resize(int width, int height);

        // DEBUG
        const char* getGLSLCurrentProgramString() const;

    protected:
        virtual void cleanup(void) = 0;
        GLSLProgram compileAndLinkShader(const char* vertex, const char* fragment);
        GLSLProgram compileAndLinkShader(const char* vertex, const char* geometry, const char* fragment);

    protected:
        GLSLProgramMap     glsl_program_map_;      //!< Map of GLSLProgram objects used by this GLScene
        GLSLProgramMapIter current_program_iter_;  //!< Current GLSLProgram in use
        int width_;                                //!< Width of the window
        int height_;                               //!< Height of the window
};

#endif /* GLSCENE_HPP_ */
