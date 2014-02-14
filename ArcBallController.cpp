/*
 * ArcBallController.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: jusabiaga
 */

#include "GL/glew.h"                // Glut
#include "GL/freeglut.h"            // Glut
#include "Defs.hpp"                 // MIDDLE_BUTTON_SCROLL_DOWN/UP
#include <iostream>					// std::cout and std::endl
#include "ArcBallController.hpp"



/**
* @brief Non-Default Constructor
*
* @param width              Width of the viewport
* @param height             Height of the viewport
* @param max_azimuth        Maximum azimuth angle from swiping the whole range of the viewport
* @param max_inclination    Maximum inclination angle from swiping the whole range of the viewport
* @param radius_delta       Increment/decrement of the radius
*/
ArcBallController::ArcBallController(int width, int height, float max_azimuth, float max_inclination, float radius_delta)
    : width_(width),
      height_(height),
      max_azimuth_(max_azimuth),
      max_inclination_(max_inclination),
      active_(false),
      last_x_(0.0f),
      last_y_(0.0f),
      radius_delta_(radius_delta)
{}



/**
* @brief Callback for mouse clicks
*
* @param button             Button id
* @param state              State of the button
* @param x                  x pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
* @param y                  y pixel coordinate in the viewport coordinate system (upper-left = (0, 0))
*/
void ArcBallController::mouseClick(int button, int state, int x, int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                std::cout << "Left button down" << std::endl;
                active_ = true;
                last_x_ = normalize(x, width_);
                last_y_ = normalize(y, height_);
                curr_x_ = last_x_;
                curr_y_ = last_y_;
            }
            else if (state == GLUT_UP)
            {
                std::cout << "Left button up" << std::endl;
                active_ = false;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            std::cout << "Middle button state " << state << std::endl;
            break;

        case MIDDLE_BUTTON_SCROLL_DOWN:
            std::cout << "Middle button scroll DOWN" << std::endl;
            radius_delta_ += 0.1f;
            break;

        case MIDDLE_BUTTON_SCROLL_UP:
            std::cout << "Middle button scroll UP" << std::endl;
            radius_delta_ -= 0.1f;
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
void ArcBallController::mouseMotion(int x, int y)
{
    if (active_)
    {
        curr_x_ = normalize(x, width_);
        curr_y_ = normalize(y, height_);
    }

	std::cout << "Mouse motion " << x << ", " << y << ", " << curr_x_ << ", " << curr_y_ << std::endl;
}



/**
* @brief It updates updates the radius, inclination and azimuth with the delta's since the last update call
*
* @param radius_delta       Increase/decrease in radius since the last read from controller
* @param inclination        Increase/decrease in inclination angle since the last read from controller
* @param azimuth            Increase/decrease in azimuth angle since the last read from controller
*/
void ArcBallController::update(float &radius_delta, float &inclination, float &azimuth)
{
    radius_delta = radius_delta_;
    azimuth = (curr_x_ - last_x_) * max_azimuth_;
    inclination = (curr_y_ - last_y_) * max_inclination_;

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

        std::cout << "Radius delta = " << radius_delta << std::endl;
        std::cout << "Azimuth      = " << azimuth << std::endl;
        std::cout << "Inclination  = " << inclination << std::endl;
    }

    radius_delta_ = 0.0f;
}



/**
* @brief Resize width and height of the viewport
*
* @param width      New width of the viewport
* @param height     New height of the viewport
*/
void ArcBallController::windowResize(int width, int height)
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
float ArcBallController::normalize(int value, int range)
{
    return ((float)value / (range - 1) * 2.0f - 1.0f);

}


