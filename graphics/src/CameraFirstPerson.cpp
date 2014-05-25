/*
 * CameraFirstPerson.cpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

#include "CameraFirstPerson.hpp"

CameraFirstPerson::CameraFirstPerson(const CameraIntrinsic &camera_intrinsic, const Object3D &first_person) :
        Object3D(first_person), intrinsic_(camera_intrinsic)
{
}

CameraFirstPerson::~CameraFirstPerson()
{
    // TODO Auto-generated destructor stub
}

void CameraFirstPerson::update(const Object3D &object)
{
    glm::vec3 position_;    //!< Position of the object (in the parent's coordinate system)
    glm::vec3 x_axis_;      //!< back-to-front axis (in the parent's coordinate system)
    glm::vec3 y_axis_;      //!< left-to-right
    glm::vec3 z_axis_;      //!< bottom-up

    position_ = object.getPosition();
    x_axis_   = object.getXAxis();
    y_axis_   = object.getYAxis();
    z_axis_   = object.getZAxis();
}

const glm::mat4& CameraFirstPerson::getPerspectiveMatrix(void) const
{
    return intrinsic_.getPerspectiveMatrix();
}

glm::mat4 CameraFirstPerson::getViewMatrix(void) const
{
    return getTransformFromParent();
}

/**
* @brief Updated the aspect ratio
*
* @param aspect_ratio_ The new aspect ratio (width / height)
*/
void CameraFirstPerson::setAspectRatio(float aspect_ratio)
{
    intrinsic_.setAspectRatio(aspect_ratio);
}
