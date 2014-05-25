/*
 * Lights.hpp
 *
 *  Created on: May 21, 2013
 *      Author: jusabiaga
 */

#ifndef LIGHTS_HPP_
#define LIGHTS_HPP_

#include <glm/glm.hpp>          // glm::vec3, glm::vec4
#include <vector>               // std::vector

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
    LightSpotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 intensity, float shinniness, float cutoff)
            : position_(position), direction_(direction), intensity_(intensity), shinniness_(shinniness), cutoff_(cutoff) {}

    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 intensity_;
    float     shinniness_;
    float     cutoff_;
};

// TYPEDEFS
typedef std::vector<LightPositional>   LightPositionalVector;
typedef std::vector<LightDirectional>  LightDirectionalVector;
typedef std::vector<LightSpotlight>    LightSpotlightVector;
typedef LightPositionalVector::const_iterator   LightPositionalConstIterator;
typedef LightPositionalVector::iterator         LightPositionalIterator;
typedef LightDirectionalVector::const_iterator  LightDirectionalIterator;
typedef LightSpotlightVector::const_iterator    LightSpotlightIterator;


#endif /* LIGHTS_HPP_ */
