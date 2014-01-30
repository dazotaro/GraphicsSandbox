/*
 * Camera.cpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#include "CameraIntrinsic.hpp"
#include <glm/gtc/matrix_transform.hpp>     // gluPerspective

/**
* @brief Non-Default Constructor
*
* @param object         Object3D containing the position and orientation of this node in the parent coordinate system
* @param fovy           Vertical field-of-view
* @param aspect_ration  Width divided by height
* @param zNear          Distance from the COP to the Near Plane
* @param zFar           Distance from the COP to the Far Plane
*/
CameraIntrinsic::CameraIntrinsic(float fovy, float aspect_ratio, float zNear, float zFar) :
    fovy_(fovy), aspect_ratio_(aspect_ratio), zNear_(zNear), zFar_(zFar)
{
    perspective_ = glm::perspective(fovy_, aspect_ratio_, zNear_, zFar_);
}

/**
* @brief Destructor
*/
CameraIntrinsic::~CameraIntrinsic()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Returns the Perspective Projection matrix
*
* @return 4x4 homogeneous matrix with the transformation
*/
const glm::mat4& CameraIntrinsic::getPerspectiveMatrix(void) const
{
    return perspective_;
}

/**
* @brief Updated the aspect ratio
*
* @param aspect_ratio_ The new aspect ratio (width / height)
*/
void CameraIntrinsic::setAspectRatio(float aspect_ratio)
{
    aspect_ratio_ = aspect_ratio;
    perspective_  = glm::perspective(fovy_, aspect_ratio_, zNear_, zFar_);
}

