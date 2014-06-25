/*
 * ArcBallController.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: jusabiaga
 */

// Local Includes
#include "ArcBallController.hpp"
#include "GraphicsDefs.hpp"         // MIDDLE_BUTTON_SCROLL_DOWN/UP

// Global Includes
#include <GL/freeglut.h>            // Glut
#include <iostream>		            // std::cout and std::endl



/**
* @brief Non-Default Constructor
*
* @param width              Width of the viewport
* @param height             Height of the viewport
* @param max_azimuth        Maximum azimuth angle from swiping the whole range of the viewport
* @param max_inclination    Maximum inclination angle from swiping the whole range of the viewport
* @param radius_delta       Increment/decrement of the radius
*/
ArcBallController::ArcBallController(JU::uint32 width, JU::uint32 height, JU::f32 max_azimuth, JU::f32 max_inclination, JU::f32 radius_inc)
    : width_(width),
      height_(height),
      max_azimuth_(max_azimuth),
      max_inclination_(max_inclination),
      active_(false),
      last_x_(0),
      last_y_(0),
      curr_x_(0),
      curr_y_(0),
      radius_inc_(radius_inc),
      radius_delta_(0.0f)
{}



/**
* @brief Callback for mouse clicks
*
* @param button             Button id
* @param state              State of the button
* @param x                  x pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
* @param y                  y pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
*/
void ArcBallController::mouseClick(JU::uint32 button, JU::uint32 state, JU::uint32 x, JU::uint32 y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                active_ = true;
                last_x_ = curr_x_ = x;
                last_y_ = curr_y_ = y;
            }
            else if (state == GLUT_UP)
            {
                active_ = false;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            break;

        case MIDDLE_BUTTON_SCROLL_DOWN:
            radius_delta_ += radius_inc_;
            break;

        case MIDDLE_BUTTON_SCROLL_UP:
            radius_delta_ -= radius_inc_;
            break;

        default:
            break;
    }
}



/**
* @brief Callback for mouse motion with key pressed-down
*
* @param x                  x pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
* @param y                  y pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
*/
void ArcBallController::mouseMotion(JU::uint32 x, JU::uint32 y)
{
    if (active_)
    {
        curr_x_ = x;
        curr_y_ = y;
    }
}



/**
* @brief It updates updates the radius, inclination and azimuth with the delta's since the last update call
*
* @param radius_delta       Increase/decrease in radius since the last read from controller
* @param inclination        Increase/decrease in inclination angle since the last read from controller
* @param azimuth            Increase/decrease in azimuth angle since the last read from controller
*/
void ArcBallController::update(JU::f32 &radius_delta, JU::f32 &inclination, JU::f32 &azimuth)
{
    radius_delta = radius_delta_;
    inclination = azimuth = 0.0f;

    if (last_x_ != curr_x_ || last_y_ != curr_y_)
    {
        glm::vec3 OP1 = getPointOnSphere(last_x_, last_y_);
        glm::vec3 OP2 = getPointOnSphere(curr_x_, curr_y_);
        JU::f32 angle = acos(std::min(1.0f, OP1.x * OP2.x + OP1.y * OP2.y + OP1.z * OP2.z));
        glm::vec3 axis = glm::cross(OP1, OP2);
    }

    if (!active_)
    {
        last_x_ = 0.0f;
        last_y_ = 0.0f;
        curr_x_ = 0.0f;
        curr_y_ = 0.0f;
    }
    else
    {
        last_x_ = curr_x_;
        last_y_ = curr_y_;
    }

    radius_delta_ = 0.0f;
}



/**
* @brief It updates updates the radius, inclination and azimuth with the delta's since the last update call
*
* @param radius_delta    Increase/decrease in radius since the last read from controller
* @param angle           Angle of rotation
* @param axis            Axis of rotation
*/
void ArcBallController::update(JU::f32& radius_delta, JU::f32& angle, glm::vec3& axis)
{
    radius_delta = radius_delta_;
    angle = 0.0f;


    if (last_x_ != curr_x_ || last_y_ != curr_y_)
    {
        glm::vec3 OP1 = getPointOnSphere(last_x_, last_y_);
        glm::vec3 OP2 = getPointOnSphere(curr_x_, curr_y_);
        angle = acos(std::min(1.0f, OP1.x * OP2.x + OP1.y * OP2.y + OP1.z * OP2.z));
        axis = glm::cross(OP2, OP1);
    }
    else
    {
        axis.x = 0.0f;
        axis.y = 1.0f;
        axis.z = 0.0f;
    }


    if (!active_)
    {
        last_x_ = 0.0f;
        last_y_ = 0.0f;
        curr_x_ = 0.0f;
        curr_y_ = 0.0f;
    }
    else
    {
        last_x_ = curr_x_;
        last_y_ = curr_y_;
    }

    radius_delta_ = 0.0f;
}



/**
* @brief Resize width and height of the viewport
*
* @param width      New width of the viewport
* @param height     New height of the viewport
*/
void ArcBallController::windowResize(JU::uint32 width, JU::uint32 height)
{
    width_ = width;
    height_ = height;
}



/**
* @brief Normalize viewport pixel coordinates
*
* @param value      x/y coordinate
* @param range      Range of this axis (either width or height)
*/
JU::f32 ArcBallController::normalize(JU::uint32 value, JU::uint32 range) const
{
    return ((JU::f32)value / (range - 1) * 2.0f - 1.0f);
}



/**
* @brief Get point on unit sphere
*
* @param x      X normalized coordinate
* @param y      Y normalized coordinate
*/
glm::vec3 ArcBallController::getPointOnSphere(JU::uint32 x, JU::uint32 y) const
{
    glm::vec3 point(normalize(x, width_), normalize(y, height_), 0.0f);

    // Flip Y coordinate value
    //point.y = - point.y;

    // Compute distance to the center of the screen of point (x,y)
    JU::f32 dist_sqr = point.x * point.x + point.y * point.y;

    // If this is greater than one then we clicked farther than the sphere
    if (dist_sqr <= 1.0f)
        point.z = sqrt( 1.0f - dist_sqr);
    else
        point = glm::normalize(point);

    return point;
}


