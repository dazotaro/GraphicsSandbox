/*
 * CameraThirdPerson.cpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "GraphicsDefs.hpp"                         // MIDDLE_BUTTON_SCROLL_DOWN/UP
#include "CameraThirdPerson.hpp"
#include "Object3D.hpp"                     // Object3D
#include "CoordinateHelper.hpp"             // spherical2cartesian()

// Global includes
#include <glm/gtc/matrix_transform.hpp>     // glm:lookAt
#include <iostream>                         // std::cout, std::endl



CameraThirdPerson::CameraThirdPerson(const CameraIntrinsic &camera_intrinsic,
                                     const Object3D &target,
                                     float distance_to_target,
                                     //float height_to_target,
                                     float azimuth,
                                     float inclination) :
                     intrinsic_         (camera_intrinsic),
                     distance_to_target_(distance_to_target),
                     azimuth_           (azimuth),
                     inclination_       (inclination)
{
    glm::vec3 point_on_sphere;      // given the spherical coordinates compute the cartesian ones for that point on the sphere
    CoordinateHelper::spherical2cartesian(distance_to_target, inclination_, azimuth_, point_on_sphere[0], point_on_sphere[1], point_on_sphere[2]);

    glm::vec3 cop = point_on_sphere + target.getPosition();
    glm::vec3 view = glm::normalize(cop - target.getPosition());
    glm::vec3 u    = glm::normalize(glm::cross(target.getYAxis(), view));
    glm::vec3 up   = glm::normalize(glm::cross(view, u));

    Object3D(cop, u, up, view);
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
void CameraThirdPerson::update(const Object3D &target, float distance_delta, float inclination_delta, float azimuth_delta)
{
    distance_to_target_ += distance_delta;
    inclination_        += inclination_delta;
    azimuth_            += azimuth_delta;

    update(target);
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

    // For testing purposes (clarity)
    glm::vec3& cop  = position_;
    glm::vec3& u    = x_axis_;
    glm::vec3& up   = y_axis_;
    glm::vec3& view = z_axis_;


    cop  = point_on_sphere + target.getPosition();
    view = glm::normalize(cop - target.getPosition());
    u    = glm::normalize(glm::cross(target.getYAxis(), view));
    up   = glm::normalize(glm::cross(view, u));
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
void CameraThirdPerson::setAspectRatio(float aspect_ratio)
{
    intrinsic_.setAspectRatio(aspect_ratio);
}




