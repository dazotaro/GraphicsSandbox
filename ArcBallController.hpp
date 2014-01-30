/*
 * ArcBallController.hpp
 *
 *  Created on: Nov 6, 2013
 *      Author: jusabiaga
 */

#ifndef ARCBALLCONTROLLER_HPP_
#define ARCBALLCONTROLLER_HPP_

/**
 * @brief   Arc-ball (spherical) camera controller
 *
 */
class ArcBallController
{
    public:
        ArcBallController(int width, int height, float max_azimuth, float max_inclination, float radius_delta = 0.0f);

        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);

        void update(float &radius_delta, float &inclination, float &azimuth);
        void windowResize(int width, int height);

    private:
        float normalize(int value, int range_pixels, int range_angle);

    private:
        int width_;
        int height_;
        float max_azimuth_;
        float max_inclination_;
        bool active_;
        float last_x_;
        float last_y_;
        float curr_x_;
        float curr_y_;
        float radius_delta_;	// Delta in distance to apply to the object controlled by this controller
};

#endif /* ARCBALLCONTROLLER_HPP_ */
