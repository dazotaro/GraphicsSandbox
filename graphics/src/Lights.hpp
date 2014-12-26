/*
 * Lights.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef LIGHTS_HPP_
#define LIGHTS_HPP_

// Global includes
#include <glm/glm.hpp>          // glm::vec3, glm::vec4
#include <vector>               // std::vector
#include <string>               // std::string
#include <map>                  // std::map


// FORWARD DECLARATIONS
// --------------------
class GLSLProgram;


/**
 * @brief Positional Light
 *
 * @details Data to represent a positional light (it emanates light in all directions). For example, an ideal bulb.
 */
struct LightPositional
{
    LightPositional(glm::vec3 position, glm::vec3 intensity)
            : position_(position), intensity_(intensity) {}

    glm::vec3 position_;
    glm::vec3 intensity_;
};



/**
 * @brief Directional Light
 *
 * @details Data to represent a directional light. For example, the sun.
 */
struct LightDirectional
{
    LightDirectional(glm::vec3 direction, glm::vec3 intensity)
            : direction_(direction), intensity_(intensity) {}

    glm::vec3 direction_;
    glm::vec3 intensity_;
};



/**
 * @brief SpotLight
 *
 * @details Data to represent a spotlight.
 */
struct LightSpotlight
{
    LightSpotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 intensity, float cutoff)
            : position_(position), direction_(direction), intensity_(intensity), cutoff_(cutoff) {}

    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 intensity_;
    float     cutoff_;      //!< Cutoff angle (between 0 and 90)
};



// TYPEDEFS
typedef std::vector<LightPositional>                LightPositionalVector;
typedef std::vector<LightDirectional>               LightDirectionalVector;
typedef std::vector<LightSpotlight>                 LightSpotlightVector;

typedef std::map<std::string, const LightPositional*>     LightPositionalMap;
typedef LightPositionalMap::const_iterator                LightPositionalMapConstIterator;
typedef LightPositionalMap::iterator                      LightPositionalMapIterator;
typedef std::map<std::string, const LightDirectional*>    LightDirectionalMap;
typedef LightDirectionalMap::const_iterator               LightDirectionalMapConstIterator;
typedef LightDirectionalMap::iterator                     LightDirectionalMapIterator;
typedef std::map<std::string, const LightSpotlight*>      LightSpotlightMap;
typedef LightSpotlightMap::const_iterator                 LightSpotlightMapConstIterator;
typedef LightSpotlightMap::iterator                       LightSpotlightMapIterator;


class LightManager
{
    public:
		// ENUMERATION DEFINITIONS
		enum LightType
		{
			POSITIONAL,
			DIRECTIONAL,
			SPOTLIGHT,
		};


/*
        ~LightManager();

    public:
        void addPositionalLight(const std::string& name, const LightPositional* plight);
        void deletePositionalLight(const std::string& name);
        void addDirectionalLight(const std::string& name, const LightDirectional* plight);
        void deleteDirectionalLight(const std::string& name);
        void addSpotlight(const std::string& name, const LightSpotlight* plight);
        void deleteSpotlight(const std::string& name);

        void transferToShader(const GLSLProgram& glsl_program, const glm::mat4& transform) const;

    private:
        LightPositionalMap  mPositional_;
        LightDirectionalMap mDirectional_;
        LightSpotlightMap   mSpotlight_;
        */
};


#endif /* LIGHTS_HPP_ */
