/*
 * GLSLProgramExt.hpp
 *
 *  Created on: Dec 21, 2014
 *      Author: jusabiaga
 */

#ifndef SRC_GLSLPROGRAMEXT_HPP_
#define SRC_GLSLPROGRAMEXT_HPP_


// Local includes
#include "Lights.hpp"				// LightPositional, LightDirectional, LightSpotlight

// Forward Declarations
class Material;
class GLSLProgram;


/*
 * @brief Static class to help with the uniform data transfers
 *
 */
class GLSLProgramExt
{
	public:
		// Matrices
		static const char* MODEL_MATRIX_STRING;
		static const char* VIEW_MATRIX_STRING;
		static const char* MODELVIEW_MATRIX_STRING;
		static const char* MVP_MATRIX_STRING;
		static const char* PROJECTION_MATRIX_STRING;
		static const char* NORMAL_MATRIX_STRING;
		// Lights
		static const char* NUM_POSITIONAL_LIGHTS_STRING;
		static const char* NUM_DIRECTIONAL_LIGHTS_STRING;
		static const char* NUM_SPOTLIGHT_LIGHTS_STRING;
		static const char* POSITIONAL_ARRAY_PREFIX_STRING;
		static const char* DIRECTIONAL_ARRAY_PREFIX_STRING;
		static const char* SPOTLIGHT_ARRAY_PREFIX_STRING;
		static const char* LIGHT_POSITION_STRING;
		static const char* LIGHT_DIRECTION_STRING;
		static const char* LIGHT_INTENSITY_STRING;
		static const char* LIGHT_SHININESS_STRING;
		static const char* LIGHT_CUTOFF_STRING;
		// Material
		static const char* KA_STRING;
		static const char* KD_STRING;
		static const char* KS_STRING;
		static const char* SHININESS_STRING;

	public:
		static void setUniform(const GLSLProgram& program, const Material& material);
		static void setUniform(const GLSLProgram& program, const LightPositionalVector&  lights);
		static void setUniform(const GLSLProgram& program, const LightDirectionalVector& lights);
		static void setUniform(const GLSLProgram& program, const LightSpotlightVector&   lights);
};

#endif /* SRC_GLSLPROGRAMEXT_HPP_ */
