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



void LightManager::transferToShader(const GLSLProgram& glsl_program, const glm::mat4& transform) const
{
	std::string prefix;
	JU::uint32 counter = 0;

    // POSITIONAL
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mPositional_.size()));

    if (mPositional_.size())
    {
		counter = 0;
		prefix = POSITIONAL_ARRAY_PREFIX_STRING + std::string("[");
		for (LightPositionalMapConstIterator iter = mPositional_.begin(); iter != mPositional_.end(); ++iter)
		{
			glm::vec4 new_position = transform * glm::vec4(iter->second->position_, 1.0f);

		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightPositional::POSITION_STRING).c_str(), new_position);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightPositional::INTENSITY_STRING).c_str(), iter->second->intensity_);

			++counter;
		}
    }

    // DIRECTIONAL
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mDirectional_.size()));

    if (mDirectional_.size())
    {
		counter = 0;
		prefix = DIRECTIONAL_ARRAY_PREFIX_STRING + std::string("[");
		for (LightDirectionalMapConstIterator iter = mDirectional_.begin(); iter != mDirectional_.end(); ++iter)
		{
			glm::vec4 new_direction = transform * glm::vec4(iter->second->direction_, 1.0f);

		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightDirectional::DIRECTION_STRING).c_str(), new_direction);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightDirectional::INTENSITY_STRING).c_str(), iter->second->intensity_);

		    ++counter;
		}
	}

    // SPOTLIGHT
    // ----------
    glsl_program.setUniform(NUM_POSITIONAL_LIGHTS_STRING.c_str(), static_cast<int>(mSpotlight_.size()));

    if (mSpotlight_.size())
    {
		counter = 0;
		prefix = SPOTLIGHT_ARRAY_PREFIX_STRING + std::string("[");
		for (LightSpotlightMapConstIterator iter = mSpotlight_.begin(); iter != mSpotlight_.end(); ++iter)
		{
			glm::vec4 new_position = transform * glm::vec4(iter->second->position_, 1.0f);
			glm::vec4 new_direction = transform * glm::vec4(iter->second->direction_, 1.0f);

		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightSpotlight::POSITION_STRING).c_str(),  new_position);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightSpotlight::DIRECTION_STRING).c_str(), new_direction);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightSpotlight::INTENSITY_STRING).c_str(), iter->second->intensity_);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightSpotlight::SHININESS_STRING).c_str(), iter->second->shininess_);
		    glsl_program.setUniform((prefix  + std::to_string(counter) + "]." + LightSpotlight::CUTOFF_STRING).c_str(),    iter->second->cutoff_);

			++counter;
		}
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


