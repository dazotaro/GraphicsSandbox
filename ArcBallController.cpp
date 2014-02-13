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

void ArcBallController::mouseMotion(int x, int y)
{
    if (active_)
    {
        curr_x_ = normalize(x, width_);
        curr_y_ = normalize(y, height_);
    }

	std::cout << "Mouse motion " << x << ", " << y << ", " << curr_x_ << ", " << curr_y_ << std::endl;
}

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

void ArcBallController::windowResize(int width, int height)
{
    width_ = width;
    height_ = height;
}

float ArcBallController::normalize(int value, int range_pixels)
{
    return ((float)value / (range_pixels - 1) * 2.0f - 1.0f);

}
