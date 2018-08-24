/*
 * Object3D.cpp
 *
 *  Created on: May 7, 2013
 *      Author: jusabiaga
 */

// Local includes
#include "Object3D.hpp"

// Global includes
#define GLM_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>     // glm::rotate, glm::translate
#include <glm/gtx/transform2.hpp>           // glm::rotate, glm::translate

/**
* @brief Default Constructor
*
* It positions the object at the parent's coordinate system origin.
* It aligns the object's axis with the parents:
*   + X axis = parent's X axis
*   + Y axis     = parent's Y axis
*   + Z axis      = parent's Z axis
*/
/*
Object3D::Object3D() :
    position_           (glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    x_axis_  (glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)),
    y_axis_      (glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)),
    z_axis_       (glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)),
    roll_angle_         (1.0f),
    yaw_angle_          (1.0f),
    pitch_angle_        (1.0f)
{
}
*/

/**
* @brief Non-Default Constructor
*
* It is responsability of the caller to provide three axis that form an Orthonormal coordinate system
*
* @param position   Position of the object (int the parent's coordinate system)
* @param x_axis     Object's X axis
* @param y_axis     Object's Y axis
* @param z_axis     Object's Z axis
*/
Object3D::Object3D(const glm::vec3 &position,
                   const glm::vec3 &x_axis,
                   const glm::vec3 &y_axis,
                   const glm::vec3 &z_axis) :
             position_      (position),
             x_axis_        (x_axis),
             y_axis_        (y_axis),
             z_axis_        (z_axis)
{
}

/**
* @brief Destructor
*/
Object3D::~Object3D()
{
    // TODO Auto-generated destructor stub
}

/**
* @brief Setter function
*/
void Object3D::setPosition(const glm::vec3 &position)
{
    position_ = position;
}

/**
* @brief Setter function
*/
void Object3D::setXAxis(const glm::vec3 &x_axis)
{
    x_axis_ = x_axis;
}

/**
* @brief Setter function
*/
void Object3D::setYAxis(const glm::vec3 &y_axis)
{
    y_axis_ = y_axis;
}

/**
* @brief Setter function
*/
void Object3D::setZAxis(const glm::vec3 &z_axis)
{
    z_axis_ = z_axis;
}

/**
* @brief Translate the object
*
* @param translate Displacement
*/
void Object3D::translate(const glm::vec3 &translate)
{
    position_ += translate;
}

/**
* @brief Rotate the object around its own X axis
*
* @param angle Angle to rotate (in degrees)
*/
void Object3D::rotateX(JU::f32 angle)
{
    glm::mat4 rotate = glm::rotate(angle, x_axis_);


    y_axis_ = glm::vec3(rotate * glm::vec4(y_axis_, 0.0f));
    z_axis_ = glm::vec3(rotate * glm::vec4(z_axis_, 0.0f));
}

/**
* @brief Rotate the object around its own Y axis
*
* @param angle Angle to rotate (in degrees)
*/
void Object3D::rotateY(JU::f32 angle)
{
    glm::mat4 rotate = glm::rotate(angle, y_axis_);

    x_axis_ = glm::vec3(rotate * glm::vec4(x_axis_, 0.0f));
    z_axis_ = glm::vec3(rotate * glm::vec4(z_axis_, 0.0f));
}

/**
* @brief Rotate the object around its own Z axis
*
* @param angle Angle to rotate (in degrees)
*/
void Object3D::rotateZ(JU::f32 angle)
{
    glm::mat4 rotate = glm::rotate(angle, z_axis_);

    x_axis_ = glm::vec3(rotate * glm::vec4(x_axis_, 0.0f));
    y_axis_ = glm::vec3(rotate * glm::vec4(y_axis_, 0.0f));
}



/**
* @brief Rotate the object around axis
*
* @param angle Angle to rotate (in degrees)
* @param axis  Axis of rotation
*/
void Object3D::rotate(JU::f32 angle, const glm::vec3& axis)
{
    glm::mat4 rotation = glm::rotate(angle, axis);

    x_axis_ = glm::vec3(rotation * glm::vec4(x_axis_, 0.0f));
    y_axis_ = glm::vec3(rotation * glm::vec4(y_axis_, 0.0f));
    z_axis_ = glm::vec3(rotation * glm::vec4(z_axis_, 0.0f));
}


/**
* @brief Get the transformation to the parent's coordinates system (e.g. Model matrix if the parent is the World C.S.)
*
* @return 4x4 Homogeneous transformation matrix
*/
glm::mat4 Object3D::getTransformToParent(void) const
{
    return glm::mat4(  x_axis_.x,   x_axis_.y,   x_axis_.z, 0.0f,
                       y_axis_.x,   y_axis_.y,   y_axis_.z, 0.0f,
                       z_axis_.x,   z_axis_.y,   z_axis_.z, 0.0f,
                     position_.x, position_.y, position_.z, 1.0f);
}

/**
* @brief Get the transformation from the parent's coordinates system (e.g. inverse of the Model matrix if the parent is the World C.S.)
*
* @return 4x4 Homogeneous transformation matrix
*/
glm::mat4 Object3D::getTransformFromParent(void) const
{
    JU::f32 x_dot = glm::dot(x_axis_, position_);
    JU::f32 y_dot = glm::dot(y_axis_, position_);
    JU::f32 z_dot = glm::dot(z_axis_, position_);

    return glm::mat4(x_axis_.x, y_axis_.x, z_axis_.x, 0.0,
                     x_axis_.y, y_axis_.y, z_axis_.y, 0.0,
                     x_axis_.z, y_axis_.z, z_axis_.z, 0.0,
                        -x_dot,    -y_dot,    -z_dot, 1.0f);
}


/**
* @brief Overloaded output operator
*
* @param out            Output stream
* @param rhs            Instance of Object3D to output
*
* @return std::ostream object
*/
std::ostream & operator<<(std::ostream &out, const Object3D &rhs)
{
    out << "Position = (" << rhs.position_.x << ", " << rhs.position_.y << ", " << rhs.position_.z << ")" << std::endl;
    out << "X Axis   = (" <<   rhs.x_axis_.x << ", " <<   rhs.x_axis_.y << ", " <<   rhs.x_axis_.z << ")" << std::endl;
    out << "Y Axis   = (" <<   rhs.y_axis_.x << ", " <<   rhs.y_axis_.y << ", " <<   rhs.y_axis_.z << ")" << std::endl;
    out << "Z Axis   = (" <<   rhs.z_axis_.x << ", " <<   rhs.z_axis_.y << ", " <<   rhs.z_axis_.z << ")" << std::endl;

    return out;
}

