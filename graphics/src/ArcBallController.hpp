/*
 * ArcBallController.hpp
 *
 *  Created on: Nov 6, 2013
 *      Author: jusabiaga
 */

#ifndef ARCBALLCONTROLLER_HPP_
#define ARCBALLCONTROLLER_HPP_

// GlobalIncludes
#include "common/Defs.hpp"      // JU::uint32
#include <glm/glm.hpp>      // glm::vec3

/**
 * @brief   Arc-ball (spherical) camera controller
 *
 */
class ArcBallController
{
    public:
        ArcBallController(JU::uint32 width, JU::uint32 height, JU::f32 radius_inc = 0.1f);

        // Mouse Controllers
        void mouseClick(JU::uint32 button, JU::uint32 state, JU::uint32 x, JU::uint32 y);
        void mouseMotion(JU::uint32 x, JU::uint32 y);

        void update(JU::f32& radius_delta, JU::f32& angle, glm::vec3& axis);
        void windowResize(JU::uint32 width, JU::uint32 height);

    private:
        // Utility Functions
        // -----------------
        JU::f32 normalize(JU::uint32 value, JU::uint32 range) const;
        glm::vec3 getPointOnSphere(JU::uint32 x, JU::uint32 y) const;

    private:
        JU::uint32 width_;
        JU::uint32 height_;
        bool active_;
        JU::uint32 last_x_;
        JU::uint32 last_y_;
        JU::uint32 curr_x_;
        JU::uint32 curr_y_;
        JU::f32 radius_inc_;      //!< Increment to apply to the radius for each input signal
        JU::f32 radius_delta_;    //!< Increase/decrease in radius accumulated since last update
};

#endif /* ARCBALLCONTROLLER_HPP_ */
