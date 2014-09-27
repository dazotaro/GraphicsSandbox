/*
 * Animation.hpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

// Global includes
#include <JU/core/Defs.hpp>      // Basic types typedefs

class Animation
{
    public:
        Animation() {}
        virtual ~Animation() {}

        bool init(JU::f32 length, JU::uint8 loop);

        JU::f32 getLength() const
        {
            return length_;
        }

        JU::uint8 getNumberOfLoops() const
        {
            return loop_;
        }


    private:
        JU::f32     length_;    //!< Length of the clip in milliseconds
        JU::uint8   loop_;      //!< 0 = infinite loop; anything else means the actual number of loops
};


class AnimationClip : public Animation
{
    public:
        AnimationClip() {}
        ~AnimationClip() {}

        bool init(JU::f32 length, JU::uint8 loop = 0, JU::f32 speed = 1.0f);
        void setTimeStart(JU::f32 time_start);

        JU::f32 getFrame(JU::f32 time) const;

    private:
        JU::f32 time_start_;    //!< Global time of the start of the animation
        JU::f32 speed_;         //!< Playback speed
};


#endif /* ANIMATION_HPP_ */
