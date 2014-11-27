/*
 * Material.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: aitatxo
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

// Global includes
#include <glm/glm.hpp>		// glm::vec3
#include <JU/core/Defs.hpp>		// JU::f32
#include <unordered_map>	// std::unordered_map
#include <string>			// std::string
#include <unordered_map>	// std::unordered_map

struct Material
{
	Material(JU::f32 ka_r = 0.0f, JU::f32 ka_g = 0.0f, JU::f32 ka_b = 0.0f,
			 JU::f32 kd_r = 0.0f, JU::f32 kd_g = 0.0f, JU::f32 kd_b = 0.0f,
			 JU::f32 ks_r = 0.0f, JU::f32 ks_g = 0.0f, JU::f32 ks_b = 0.0f, JU::f32 shininess = 0.0f)
		: ka_(glm::vec3(ka_r, ka_g, ka_b)),
		  kd_(glm::vec3(kd_r, kd_g, kd_b)),
		  ks_(glm::vec3(ks_r, ks_g, ks_b)), shininess_(shininess) {}

	Material(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, JU::f32 shininess)
		: ka_(ka), kd_(kd), ks_(ks), shininess_(shininess) {}

	Material(const Material* material)
		: ka_(material->ka_), kd_(material->kd_), ks_(material->ks_), shininess_(material->shininess_) {}

	void print() const;


	glm::vec3 ka_;
	glm::vec3 kd_;
	glm::vec3 ks_;
	JU::f32 shininess_;
};

class MaterialManager
{
	public:
		static void init();
		static bool getMaterial(const std::string& material_name, Material& material);

	private:
		typedef std::unordered_map<std::string, Material> HashMapMaterial;
		typedef HashMapMaterial::const_iterator HashMapMaterialConstIter;

	private:
		static bool initialized_;
		static HashMapMaterial hpMaterials_;
};



#endif /* MATERIAL_HPP_ */
