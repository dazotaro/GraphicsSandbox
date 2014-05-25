/*
 * Camera.hpp
 *
 *  Created on: May 8, 2013
 *      Author: jusabiaga
 */

#ifndef CAMERAINTRINSIC_HPP_
#define CAMERAINTRINSIC_HPP_

#include <glm/glm.hpp>      // glm::mat4

/**
 * @brief      Camera intrinsic parameters
 *
 * @details
 *
 */
class CameraIntrinsic
{
    public:
        CameraIntrinsic(float fovy, float aspect_ratio, float zNear, float zFar);
        virtual ~CameraIntrinsic();

        // Getters
        const glm::mat4& getPerspectiveMatrix(void) const;

        // Setters
        void setAspectRatio(float aspect_ratio);

    private:
        float fovy_;            //!< Field of View in the Y axis (in degrees)
        float aspect_ratio_;    //!< Aspect ratio
        float zNear_;           //!< Near plane
        float zFar_;            //!< Far plane
        glm::mat4 perspective_; //!< Perspective matrix
};

#endif /* CAMERAINTRINSIC_HPP_ */
