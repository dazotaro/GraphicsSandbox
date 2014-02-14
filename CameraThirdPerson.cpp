/*
 * CameraThirdPerson.cpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "Defs.hpp"                         // MIDDLE_BUTTON_SCROLL_DOWN/UP
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
                     //height_to_target_(height_to_target),
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



/*
void CameraThirdPerson::setFrameCartesian(const Object3D &target)
{
    glm::vec3 cop  (target.getPosition() - (distance_to_target_ * target.getXAxis()) + (height_to_target_ * target.getYAxis()));
    glm::vec3 view (glm::normalize(cop - target.getPosition()));
    glm::vec3 u    (glm::normalize(glm::cross(target.getYAxis(), view)));
    glm::vec3 up   (glm::normalize(glm::cross(view, u)));

    position_ = cop;
    x_axis_   = u;
    y_axis_   = up;
    z_axis_   = view;
}
*/



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
    glm::vec3 cop  = position_;
    glm::vec3 u    = x_axis_;
    glm::vec3 up   = y_axis_;
    glm::vec3 view = z_axis_;


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
/*
void CameraThirdPerson::addDistanceToTarget(float distance)
{
    distance_to_target_ += distance;
}
*/



/**
* @brief Updated the aspect ratio
*
* @param aspect_ratio_ The new aspect ratio (width / height)
*/
/*
void CameraThirdPerson::addHeightToTarget(float height)
{
    height_to_target_ += height;
}
*/



/**
* @brief Updated the aspect ratio
*
* @param aspect_ratio_ The new aspect ratio (width / height)
*/
void CameraThirdPerson::setAspectRatio(float aspect_ratio)
{
    intrinsic_.setAspectRatio(aspect_ratio);
}



/*
void CameraThirdPerson::mouse(int button, int state, int x, int y)
{
    static int x_start = 0;
    static int y_start = 0;

    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {    std::cout << "Left button down" << std::endl;
                x_start = x;
                y_start = y;
                azimuth_ = 0.f;
                inclination_   = 0.f;
            }

            else if (state == GLUT_UP)
            {
                std::cout << "Left button up" << std::endl;
                int x_diff = x - x_start;
                int y_diff = y - y_start;

                // From PIXELS to DEGREES
                azimuth_ += x_diff / 100.f * M_PI_4;
                inclination_   += y_diff / 100.f * M_PI_4;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            std::cout << "Middle button state " << state << std::endl;
            break;

        case MIDDLE_BUTTON_SCROLL_DOWN:
            std::cout << "Middle button scroll DOWN" << std::endl;
            addDistanceToTarget(0.1f);
            break;

        case MIDDLE_BUTTON_SCROLL_UP:
            std::cout << "Middle button scroll UP" << std::endl;
            addDistanceToTarget(-0.1f);
            break;

        default:
            break;
    }
}
*/



