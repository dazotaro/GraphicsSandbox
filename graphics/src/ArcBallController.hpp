/*
 * ArcBallController.hpp
 *
 *  Created on: Nov 6, 2013
 *      Author: jusabiaga
 */

#ifndef ARCBALLCONTROLLER_HPP_
#define ARCBALLCONTROLLER_HPP_

// GlobalIncludes
#include <JU/Defs.hpp>      // JU::uint32

/**
 * @brief   Arc-ball (spherical) camera controller
 *
 */
class ArcBallController
{
    public:
        ArcBallController(int width, int height, float max_azimuth, float max_inclination, float radius_inc = 0.1f);

        void mouseClick(int button, int state, int x, int y);
        void mouseMotion(int x, int y);

        void update(float &radius_delta, float &inclination, float &azimuth);
        void windowResize(int width, int height);

    private:
        float normalize(int value, int range);

    private:
        int width_;
        int height_;
        float max_azimuth_;
        float max_inclination_;
        bool active_;
        JU::uint32 last_x_;
        JU::uint32 last_y_;
        JU::uint32 curr_x_;
        JU::uint32 curr_y_;
        float radius_inc_;      //!< Increment to apply to the radius for each input signal
        float radius_delta_;    //!< Increase/decrease in radius accumulated since last update
};

#endif /* ARCBALLCONTROLLER_HPP_ */
