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
 * @brief GLLightInterface
 *
 * @details Interface to transfer light data to the shaders
 */
class GLLightInterface
{
    public:
        virtual ~GLLightInterface() {};
        virtual void transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const = 0;
};



/**
 * @brief Positional Light
 *
 * @details Data to represent a positional light (it emanates light in all directions). For example, an ideal bulb.
 */
struct LightPositional : public GLLightInterface
{
    // CONSTANTS
    static const std::string POSITION_STRING;
    static const std::string INTENSITY_STRING;

    LightPositional(glm::vec3 position, glm::vec3 intensity)
            : position_(position), intensity_(intensity) {}

    void transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const;

    glm::vec3 position_;
    glm::vec3 intensity_;
};



/**
 * @brief Directional Light
 *
 * @details Data to represent a directional light. For example, the sun.
 */
struct LightDirectional : public GLLightInterface
{
    // CONSTANTS
    static const std::string DIRECTION_STRING;
    static const std::string INTENSITY_STRING;

    LightDirectional(glm::vec3 direction, glm::vec3 intensity)
            : direction_(direction), intensity_(intensity) {}

    void transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const;

    glm::vec3 direction_;
    glm::vec3 intensity_;
};



/**
 * @brief SpotLight
 *
 * @details Data to represent a spotlight.
 */
struct LightSpotlight : public GLLightInterface
{
    // CONSTANTS
    static const std::string POSITION_STRING;
    static const std::string DIRECTION_STRING;
    static const std::string INTENSITY_STRING;
    static const std::string SHININESS_STRING;
    static const std::string CUTOFF_STRING;

    LightSpotlight(glm::vec3 position, glm::vec3 direction, glm::vec3 intensity, float shinniness, float cutoff)
            : position_(position), direction_(direction), intensity_(intensity), shininess_(shinniness), cutoff_(cutoff) {}

    void transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const;

    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 intensity_;
    float     shininess_;  //!< Shinniness coefficient
    float     cutoff_;      //!< Cutoff angle (between 0 and 90)
};



// TYPEDEFS
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
        static const std::string NUM_POSITIONAL_LIGHTS_STRING;
        static const std::string NUM_DIRECTIONAL_LIGHTS_STRING;
        static const std::string NUM_SPOTLIGHT_LIGHTS_STRING;
        static const std::string POSITIONAL_ARRAY_PREFIX_STRING;
        static const std::string DIRECTIONAL_ARRAY_PREFIX_STRING;
        static const std::string SPOTLIGHT_ARRAY_PREFIX_STRING;

    public:
        ~LightManager();

    public:
        void addPositionalLight(const std::string& name, const LightPositional* plight);
        void deletePositionalLight(const std::string& name);
        void addDirectionalLight(const std::string& name, const LightDirectional* plight);
        void deleteDirectionalLight(const std::string& name);
        void addSpotlight(const std::string& name, const LightSpotlight* plight);
        void deleteSpotlight(const std::string& name);

        void transferToShader(const GLSLProgram& glsl_program, const glm::mat4& transformation) const;

    private:
        LightPositionalMap  mPositional_;
        LightDirectionalMap mDirectional_;
        LightSpotlightMap   mSpotlight_;
};


typedef std::vector<LightPositional>                LightPositionalVector;
typedef std::vector<LightDirectional>               LightDirectionalVector;
typedef std::vector<LightSpotlight>                 LightSpotlightVector;
typedef LightPositionalVector::const_iterator       LightPositionalConstIterator;
typedef LightPositionalVector::iterator             LightPositionalIterator;
typedef LightDirectionalVector::const_iterator      LightDirectionalIterator;
typedef LightSpotlightVector::const_iterator        LightSpotlightIterator;


#endif /* LIGHTS_HPP_ */
