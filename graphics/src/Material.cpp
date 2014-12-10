/*
 * Material.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: aitatxo
 */

// Local includes
#include "Material.hpp"

// STATIC MEMBER VARIABLES
// -----------------------
bool MaterialManager::initialized_ = false;
MaterialManager::HashMapMaterial MaterialManager::hpMaterials_;



// STATIC MEMBER FUNCTIONS
// -----------------------

void MaterialManager::init()
{
	if (initialized_)
		return;

	hpMaterials_["emerald"] 	   = Material(0.0215f,  0.1745f, 0.0215f, 0.07568f, 0.61424f, 0.07568f, 0.633f, 0.727811f, 0.633f, 0.6f * 128.0f);
	hpMaterials_["jade"] 		   = Material(0.135f,   0.2225f, 0.1575f, 0.54f, 0.89f, 0.63f, 0.316228f, 0.316228f, 0.316228f, 0.1f * 128.0f);
	hpMaterials_["obsidian"] 	   = Material(0.05375f, 0.05f, 0.06625f, 0.18275f, 0.17f, 0.22525f, 0.332741f, 0.328634f, 0.346435f, 0.3f * 128.0f);
	hpMaterials_["pearl"] 		   = Material(0.25f,    0.20725f, 0.20725f, 1.0f, 0.829f, 0.829f, 0.296648f, 0.296648f, 0.296648f, 0.088f * 128.0f);
	hpMaterials_["ruby"] 		   = Material(0.1745f,  0.01175f, 0.01175f, 0.61424f, 0.04136f, 0.04136f, 0.727811f, 0.626959f, 0.626959f, 0.6f * 128.0f);
	hpMaterials_["turquoise"] 	   = Material(0.1f,     0.18725f, 0.1745f, 0.396f, 0.74151f, 0.69102f, 0.297254f, 0.30829f, 0.306678f, 0.1f * 128.0f);
	hpMaterials_["brass"] 		   = Material(0.329412f,0.223529f, 0.027451f, 0.780392f, 0.568627f, 0.113725f, 0.992157f, 0.941176f, 0.807843f, 0.21794872f * 128.0f);
	hpMaterials_["bronze"] 		   = Material(0.2125f,  0.1275f, 0.054f, 0.714f, 0.4284f, 0.18144f, 0.393548f, 0.271906f, 0.166721f, 0.2f * 128.0f);
	hpMaterials_["chrome"] 		   = Material(0.25f,    0.25f, 0.25f, 0.4f, 0.4f, 0.4f, 0.774597f, 0.774597f, 0.774597f, 0.6f * 128.0f);
	hpMaterials_["copper"] 		   = Material(0.19125f, 0.0735f, 0.0225f, 0.7038f, 0.27048f, 0.0828f, 0.256777f, 0.137622f, 0.086014f, 0.1f * 128.0f);
	hpMaterials_["gold"] 		   = Material(0.24725f, 0.1995f, 0.0745f, 0.75164f, 0.60648f, 0.22648f, 0.628281f, 0.555802f, 0.366065f, 0.4f * 128.0f);
	hpMaterials_["silver"] 		   = Material(0.19225f, 0.19225f, 0.19225f, 0.50754f, 0.50754f, 0.50754f, 0.508273f, 0.508273f, 0.508273f, 0.4f * 128.0f);
	hpMaterials_["black_plastic"]  = Material(0.0f,     0.0f, 0.0f, 0.01f, 0.01f, 0.01f, 0.50f, 0.50f, 0.50f, .25f * 128.0f);
	hpMaterials_["cyan_plastic"]   = Material(0.0f,     0.1f, 0.06f, 0.0f, 0.50980392f, 0.50980392f, 0.50196078f, 0.50196078f, 0.50196078f, .25f * 128.0f);
	hpMaterials_["green_plastic"]  = Material(0.0f,     0.0f, 0.0f, 0.1f, 0.35f, 0.1f, 0.45f, 0.55f, 0.45f, .25f * 128.0f);
	hpMaterials_["red_plastic"]    = Material(0.0f,     0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.7f, 0.6f, 0.6f, .25f * 128.0f);
	hpMaterials_["white_plastic"]  = Material(0.0f,     0.0f, 0.0f, 0.55f, 0.55f, 0.55f, 0.70f, 0.70f, 0.70f, .25f * 128.0f);
	hpMaterials_["yellow_plastic"] = Material(0.0f,     0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.60f, 0.60f, 0.50f, .25f * 128.0f);
	hpMaterials_["black_rubber"]   = Material(0.02f,    0.02f, 0.02f, 0.01f, 0.01f, 0.01f, 0.4f, 0.4f, 0.4f, .078125f * 128.0f);
	hpMaterials_["cyan_rubber"]    = Material(0.0f,     0.05f, 0.05f, 0.4f, 0.5f, 0.5f, 0.04f, 0.7f, 0.7f, .078125f * 128.0f);
	hpMaterials_["green_rubber"]   = Material(0.0f,     0.05f, 0.0f, 0.4f, 0.5f, 0.4f, 0.04f, 0.7f, 0.04f, .078125f * 128.0f);
	hpMaterials_["red_rubber"] 	   = Material(0.05f,    0.0f, 0.0f, 0.5f, 0.4f, 0.4f, 0.7f, 0.04f, 0.04f, .078125f * 128.0f);
	hpMaterials_["white_rubber"]   = Material(0.05f,    0.05f, 0.05f, 0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f, .078125f * 128.0f);
    hpMaterials_["yellow_rubber"]  = Material(0.05f,    0.05f, 0.0f, 0.5f, 0.5f, 0.4f, 0.7f, 0.7f, 0.04f, .078125f * 128.0f);
    hpMaterials_["gray_rubber"]    = Material(0.05f,    0.05f, 0.05f, 0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f, .078125f * 128.0f);

	initialized_ = true;
}



bool MaterialManager::getMaterial(const std::string& material_name, Material& material)
{
	if (!initialized_)
		return false;

	HashMapMaterialConstIter iter = hpMaterials_.find(material_name);
	if (iter == hpMaterials_.end())
		return false;

	material =  iter->second;

	return true;
}



void Material::print() const
{
	std::printf("Ambient  = (%.4f, %.4f, %.4f)\n", ka_.r, ka_.g, ka_.b);
	std::printf("Diffuse  = (%.4f, %.4f, %.4f)\n", kd_.r, kd_.g, kd_.b);
	std::printf("Specular = (%.4f, %.4f, %.4f) -- Shininess (%f)\n", ks_.r, ks_.g, ks_.b, shininess_);
}




