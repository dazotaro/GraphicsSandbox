/*
 * CameraThirdPerson.cpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GraphicsDefs.hpp"                         // MIDDLE_BUTTON_SCROLL_DOWN/UP
#include "CameraThirdPerson.hpp"
#include "CoordinateHelper.hpp"             // spherical2cartesian()
#include "DebugGlm.hpp"                     // debug::print()

// Global includes
#include <JU/core/Defs.hpp>						// JU::f32
#include <glm/gtc/matrix_transform.hpp>     // glm:lookAt
#include <glm/gtx/transform2.hpp>           // glm::rotate, glm::translate
#include <iostream>                         // std::cout, std::endl
#include <cmath>							// std::fabsf



CameraThirdPerson::CameraThirdPerson(const CameraIntrinsic &camera_intrinsic,
                                     const Object3D &target,
                                     JU::f32 distance_to_target,
                                     //JU::f32 height_to_target,
                                     JU::f32 azimuth,
                                     JU::f32 inclination) :
                     intrinsic_         (camera_intrinsic),
                     distance_to_target_(distance_to_target),
                     azimuth_           (azimuth),
                     inclination_       (inclination)
{
    glm::vec3 point_on_sphere;      // given the spherical coordinates compute the cartesian ones for that point on the sphere
    CoordinateHelper::spherical2cartesian(distance_to_target, inclination_, azimuth_, point_on_sphere[0], point_on_sphere[1], point_on_sphere[2]);

    position_ = point_on_sphere + target.getPosition();                   // COP
    z_axis_  = glm::normalize(position_ - target.getPosition());          // VIEW vector
    x_axis_  = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z_axis_));    // U vector
    y_axis_  = glm::normalize(glm::cross(z_axis_, x_axis_));              // UP vector
}

CameraThirdPerson::~CameraThirdPerson()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Update the camera frame
*
* @param target The frame of the target the camera is tracking
*/
void CameraThirdPerson::update(const Object3D &target)
{
    setFrameSpherical(target);
}



/**
* @brief Update the camera frame
*
* @param target The frame of the target the camera is tracking
*/
void CameraThirdPerson::update(const Object3D &target, JU::f32 distance_delta, JU::f32 inclination_delta, JU::f32 azimuth_delta)
{
    distance_to_target_ += distance_delta;
    inclination_        += inclination_delta;
    azimuth_            += azimuth_delta;

    update(target);
}



/**
* @brief Update the camera frame
*
* @param target             The frame of the target the camera is tracking
* @param distance_delta     Increase in distance to the target
* @param angle              Angle to rotate (in radians)
* @param axis               Axis of rotation
*/
void CameraThirdPerson::update(const Object3D &target, JU::f32 distance_delta, JU::f32 angle, const glm::vec3& axis)
{
	static const JU::f32 epsilon = 0.01f;

	if ( (distance_to_target_ + distance_delta) > epsilon)
		distance_to_target_ += distance_delta;

	glm::vec4 p1 (distance_to_target_ * z_axis_, 1.0f);

    glm::mat4 rotation (glm::rotate(angle, axis));

    glm::vec3 op1 (glm::vec3(rotation * p1));

    if ((1.0f - fabs(glm::dot(glm::normalize(op1), glm::vec3(0.0f, 1.0f, 0.0f)))) > 0.005f)
	{
		position_ = target.getPosition() + glm::normalize(op1) * distance_to_target_;
		z_axis_ = glm::normalize(op1);
		x_axis_ = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z_axis_));
		y_axis_ = glm::normalize(glm::cross(z_axis_, x_axis_));
	}

    /*
    distance_to_target_ += distance_delta;

    glm::vec4 p1 (distance_to_target_ * z_axis_, 1.0f);
    glm::vec4 p2 (p1 + 1.0f * glm::vec4(y_axis_, 0.0f));

    glm::mat4 rotation (glm::rotate(glm::degrees(angle), axis));

    glm::vec3 op1 (glm::vec3(rotation * p1));
    glm::vec3 op2 (glm::vec3(rotation * p2));

    position_ = target.getPosition() + glm::normalize(op1) * distance_to_target_;
    z_axis_ = glm::normalize(op1);
    y_axis_ = glm::normalize(op2 - op1);
    x_axis_ = glm::normalize(glm::cross(y_axis_, z_axis_));
    */
}

/**
* @brief Updated the Object3D that the camera is.
*
* @param target The frame of the target the camera is tracking
*/
void CameraThirdPerson::setFrameSpherical(const Object3D &target)
{
    glm::vec3 point_on_sphere;      // given the spherical coordinates compute the cartesian ones for that point on the sphere
    CoordinateHelper::spherical2cartesian(distance_to_target_, inclination_, azimuth_, point_on_sphere[0], point_on_sphere[1], point_on_sphere[2]);

    /*
	static JU::f32 old_inc = 0.0f;
    static JU::f32 old_azi = 0.0f;

    if (fabs(old_inc - inclination_) > 0.0f || fabs(old_azi - azimuth_) > 0.0f)
    {
    	std::printf("theta = %.4f; phi = %.4f; point_sphere = (%.4f, %.4f, %.4f)\n",
    				glm::degrees(azimuth_), glm::degrees(inclination_),
    				point_on_sphere.x, point_on_sphere.y, point_on_sphere.z);

    	old_azi = azimuth_;
    	old_inc = inclination_;
    }
	*/

    // For testing purposes (clarity)
    glm::vec3& cop  = position_;
    glm::vec3& u    = x_axis_;
    glm::vec3& up   = y_axis_;
    glm::vec3& view = z_axis_;

    cop  = point_on_sphere + target.getPosition();
    view = glm::normalize(cop - target.getPosition());
    u    = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), view));
    up   = glm::normalize(glm::cross(view, u));
    //u    = glm::normalize(glm::cross(target.getYAxis(), view));
}

/**
* @brief Return the perspective matrix
*
* @return The perspective matrix
*/
const glm::mat4& CameraThirdPerson::getPerspectiveMatrix(void) const
{
    return intrinsic_.getPerspectiveMatrix();
}

/**
* @brief Return the view matrix
*
* @return Return the view matrix
*/
glm::mat4 CameraThirdPerson::getViewMatrix(void) const
{
    return getTransformFromParent();
}



/**
* @brief Updated the aspect ratio
*
* @param aspect_ratio_ The new aspect ratio (width / height)
*/
void CameraThirdPerson::setAspectRatio(JU::f32 aspect_ratio)
{
    intrinsic_.setAspectRatio(aspect_ratio);
}




