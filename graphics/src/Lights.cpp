/*
 * Lights.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "Lights.hpp"       // Header class for this source file
#include "GLSLProgram.hpp"  // GLSLProgram


// STATIC CONST variable initialization
const std::string LightManager::NUM_POSITIONAL_LIGHTS_STRING    ("num_pos_lights");
const std::string LightManager::NUM_DIRECTIONAL_LIGHTS_STRING   ("num_dir_lights");
const std::string LightManager::NUM_SPOTLIGHT_LIGHTS_STRING     ("num_spot_lights");
const std::string LightManager::POSITIONAL_ARRAY_PREFIX_STRING  ("light_pos");
const std::string LightManager::DIRECTIONAL_ARRAY_PREFIX_STRING ("light_dir");
const std::string LightManager::SPOTLIGHT_ARRAY_PREFIX_STRING   ("light_spot");
const std::string LightPositional::POSITION_STRING              ("position");
const std::string LightPositional::INTENSITY_STRING             ("intensity");
const std::string LightDirectional::DIRECTION_STRING            ("direction");
const std::string LightDirectional::INTENSITY_STRING            ("intensity");
const std::string LightSpotlight::POSITION_STRING               ("position");
const std::string LightSpotlight::DIRECTION_STRING              ("direction");
const std::string LightSpotlight::INTENSITY_STRING              ("intensity");
const std::string LightSpotlight::SHININESS_STRING              ("shininess");
const std::string LightSpotlight::CUTOFF_STRING                 ("cutoff");




/**
 * @brief Send uniform data to the shaders
 *
 * @param glsl_program      GLSLProgram active
 * @param uniform_prefix    Shaders' uniform variable prefix
 *
 */
void LightPositional::transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const
{
    // LOAD UNIFORMS
    glsl_program.setUniform((uniform_prefix + POSITION_STRING).c_str(), position_);
    glsl_program.setUniform((uniform_prefix + INTENSITY_STRING).c_str(), intensity_);
}



/**
 * @brief Send uniform data to the shaders
 *
 * @param glsl_program      GLSLProgram active
 * @param uniform_prefix    Shaders' uniform variable prefix
 *
 */
void LightDirectional::transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const
{
    // LOAD UNIFORMS
    glsl_program.setUniform((uniform_prefix + DIRECTION_STRING).c_str(), direction_);
    glsl_program.setUniform((uniform_prefix + INTENSITY_STRING).c_str(), intensity_);
}



/**
 * @brief Send uniform data to the shaders
 *
 * @param glsl_program      GLSLProgram active
 * @param uniform_prefix    Shaders' uniform variable prefix
 *
 */
void LightSpotlight::transferToShader(const GLSLProgram& glsl_program, const std::string& uniform_prefix) const
{
    // LOAD UNIFORMS
    glsl_program.setUniform((uniform_prefix + POSITION_STRING).c_str(),  position_);
    glsl_program.setUniform((uniform_prefix + DIRECTION_STRING).c_str(), direction_);
    glsl_program.setUniform((uniform_prefix + INTENSITY_STRING).c_str(), intensity_);
    glsl_program.setUniform((uniform_prefix + SHININESS_STRING).c_str(), shininess_);
    glsl_program.setUniform((uniform_prefix + CUTOFF_STRING).c_str(),    cutoff_);
}



void LightManager::addPositionalLight(const std::string& name, const LightPositional* plight)
{
    mPositional_[name] = plight;
}



void LightManager::deletePositionalLight(const std::string& name)
{
    delete mPositional_[name];
    mPositional_.erase(name);
}



void LightManager::addDirectionalLight(const std::string& name, const LightDirectional* plight)
{
    mDirectional_[name] = plight;
}



void LightManager::deleteDirectionalLight(const std::string& name)
{
    delete mDirectional_[name];
    mDirectional_.erase(name);
}



void LightManager::addSpotlight(const std::string& name, const LightSpotlight* plight)
{
    mSpotlight_[name] = plight;
}



void LightManager::deleteSpotlight(const std::string& name)
{
    delete mSpotlight_[name];
    mSpotlight_.erase(name);
}



void LightManager::transferToShader(const GLSLProgram& glsl_program) const
{
    JU::uint32 counter = 0;

    // POSITIONAL
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mPositional_.size()));

    counter = 0;
    std::string prefix (POSITIONAL_ARRAY_PREFIX_STRING + std::string("["));
    for (LightPositionalMapConstIterator iter = mPositional_.begin(); iter != mPositional_.end(); ++iter)
    {
        iter->second->transferToShader(glsl_program, prefix  + std::to_string(counter) + "]");
        ++counter;
    }

    // DIRECTIONAL
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mPositional_.size()));

    counter = 0;
    prefix = DIRECTIONAL_ARRAY_PREFIX_STRING + std::string("[");
    for (LightDirectionalMapConstIterator iter = mDirectional_.begin(); iter != mDirectional_.end(); ++iter)
    {
        iter->second->transferToShader(glsl_program, prefix  + std::to_string(counter) + "]");
        ++counter;
    }

    // SPOTLIGHT
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mPositional_.size()));

    counter = 0;
    prefix = SPOTLIGHT_ARRAY_PREFIX_STRING + std::string("[");
    for (LightSpotlightMapConstIterator iter = mSpotlight_.begin(); iter != mSpotlight_.end(); ++iter)
    {
        iter->second->transferToShader(glsl_program, prefix  + std::to_string(counter) + "]");
        ++counter;
    }
}



LightManager::~LightManager()
{
    // POSITIONAL
    // ----------
    for (LightPositionalMapIterator iter = mPositional_.begin(); iter != mPositional_.end(); ++iter)
    {
        delete iter->second;
    }
    // DIRECTIONAL
    // ----------
    for (LightDirectionalMapIterator iter = mDirectional_.begin(); iter != mDirectional_.end(); ++iter)
    {
        delete iter->second;
    }
    // SPOTLIGHT
    // ----------
    for (LightSpotlightMapIterator iter = mSpotlight_.begin(); iter != mSpotlight_.end(); ++iter)
    {
        delete iter->second;
    }
}


