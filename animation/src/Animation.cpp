/*
 * Animation.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: jusabiaga
 */

// Local include
#include "Animation.hpp"
// Global includes
#include <cstdio>       // std::printf
#include <cmath>        // std::fmod
#include <algorithm>    // std::max, std::min


bool Animation::init(JU::f32 length, JU::uint8 loop)
{
    length_ = length;
    loop_ = loop;

    return true;
}



bool AnimationClip::init(JU::f32 length, JU::uint8 loop, JU::f32 speed)
{
    Animation::init(length, loop);

    speed_ = speed;
    time_start_ = 0.0f;

    return true;
}



void AnimationClip::setTimeStart(JU::f32 time_start)
{
    time_start_ = time_start;
}



JU::f32 AnimationClip::getFrame(JU::f32 time) const
{
    JU::f32 play_time = (time - time_start_) * speed_;

    if (getNumberOfLoops() > 0)
    {
        // Clamp so the time falls within the length of the clip (after adjusting for speed)
        play_time = std::max(0.0f, std::min(play_time, getLength() * getNumberOfLoops()));
    }

    play_time = std::fmod(play_time, getLength());

    return play_time / getLength();
}
