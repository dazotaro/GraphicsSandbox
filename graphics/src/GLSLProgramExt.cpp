/*
 * GLSLProgramExt.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: jusabiaga
 */

// Local includes
#include "GLSLProgramExt.hpp"
#include "GLSLProgram.hpp"			// GLSLProgram
#include "Material.hpp"				// Material

// STATIC CONST DECLARATIONS
// -------------------------
// Matrices
const char* GLSLProgramExt::MODEL_MATRIX_STRING 			= "ModelMatrix";
const char* GLSLProgramExt::VIEW_MATRIX_STRING  			= "ViewMatrix";
const char* GLSLProgramExt::MODELVIEW_MATRIX_STRING  		= "ModelViewMatrix";
const char* GLSLProgramExt::MVP_MATRIX_STRING				= "MVP";
const char* GLSLProgramExt::PROJECTION_MATRIX_STRING		= "ProjectionMatrix";
const char* GLSLProgramExt::NORMAL_MATRIX_STRING			= "NormalMatrix";
// Lights
const char* GLSLProgramExt::NUM_POSITIONAL_LIGHTS_STRING    = "num_pos_lights";
const char* GLSLProgramExt::NUM_DIRECTIONAL_LIGHTS_STRING   = "num_dir_lights";
const char* GLSLProgramExt::NUM_SPOTLIGHT_LIGHTS_STRING     = "num_spot_lights";
const char* GLSLProgramExt::POSITIONAL_ARRAY_PREFIX_STRING  = "light_pos";
const char* GLSLProgramExt::DIRECTIONAL_ARRAY_PREFIX_STRING = "light_dir";
const char* GLSLProgramExt::SPOTLIGHT_ARRAY_PREFIX_STRING   = "light_spot";
const char* GLSLProgramExt::LIGHT_POSITION_STRING			= "position";
const char* GLSLProgramExt::LIGHT_DIRECTION_STRING			= "direction";
const char* GLSLProgramExt::LIGHT_INTENSITY_STRING			= "intensity";
const char* GLSLProgramExt::LIGHT_SHININESS_STRING			= "shininess";
const char* GLSLProgramExt::LIGHT_CUTOFF_STRING		 		= "cutoff";
// Material
const char* GLSLProgramExt::KA_STRING						= "material.Ka";
const char* GLSLProgramExt::KD_STRING						= "material.Kd";
const char* GLSLProgramExt::KS_STRING						= "material.Ks";
const char* GLSLProgramExt::SHININESS_STRING				= "material.shininess";



void GLSLProgramExt::setUniform(const GLSLProgram& program, const Material& material)
{
	program.setUniform(KA_STRING, material.ka_);
	program.setUniform(KD_STRING, material.kd_);
	program.setUniform(KS_STRING, material.ks_);
	program.setUniform(SHININESS_STRING, material.shininess_);
}



void GLSLProgramExt::setUniform(const GLSLProgram& program, const LightPositionalVector& lights)
{
    program.setUniform(NUM_POSITIONAL_LIGHTS_STRING, static_cast<int>(lights.size()));

    if (lights.size())
    {
    	JU::uint32 counter = 0;
		std::string prefix = std::string(POSITIONAL_ARRAY_PREFIX_STRING) + std::string("[");
		for (LightPositionalVector::const_iterator iter = lights.begin(); iter != lights.end(); ++iter)
		{
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_POSITION_STRING).c_str(), iter->position_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_INTENSITY_STRING).c_str(), iter->intensity_);

			++counter;
		}
    }
}



void GLSLProgramExt::setUniform(const GLSLProgram& program, const LightDirectionalVector& lights)
{
    program.setUniform(NUM_DIRECTIONAL_LIGHTS_STRING, static_cast<int>(lights.size()));

    if (lights.size())
    {
    	JU::uint32 counter = 0;
		std::string prefix = std::string(DIRECTIONAL_ARRAY_PREFIX_STRING) + std::string("[");
		for (LightDirectionalVector::const_iterator iter = lights.begin(); iter != lights.end(); ++iter)
		{
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_DIRECTION_STRING).c_str(), iter->direction_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_INTENSITY_STRING).c_str(), iter->intensity_);

			++counter;
		}
    }
}



void GLSLProgramExt::setUniform(const GLSLProgram& program, const LightSpotlightVector& lights)
{
    program.setUniform(NUM_SPOTLIGHT_LIGHTS_STRING, static_cast<int>(lights.size()));

    if (lights.size())
    {
    	JU::uint32 counter = 0;
		std::string prefix = std::string(SPOTLIGHT_ARRAY_PREFIX_STRING) + std::string("[");
		for (LightSpotlightVector::const_iterator iter = lights.begin(); iter != lights.end(); ++iter)
		{
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_POSITION_STRING).c_str(),  iter->position_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_DIRECTION_STRING).c_str(), iter->direction_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_INTENSITY_STRING).c_str(), iter->intensity_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_SHININESS_STRING).c_str(), iter->shininess_);
		    program.setUniform((prefix + std::to_string(counter) + "]." + LIGHT_CUTOFF_STRING).c_str(),    iter->cutoff_);

			++counter;
		}
    }
}



