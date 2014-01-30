/*
 * CameraThirdPerson.hpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

#ifndef CAMERATHIRDPERSON_HPP_
#define CAMERATHIRDPERSON_HPP_

#define _USE_MATH_DEFINES
#include <math.h>
#include "CameraInterface.hpp"      // CameraInterface parent class
#include "CameraIntrinsic.hpp"      // CamneraIntrinsic object contained
#include "Object3D.hpp"             // Object3D

/**
 * @brief   Third-person camera type
 *
 * \todo    Maybe it should not inherit from Object3D but, instead, it should just contain a pointer to a Object3D
 */
class CameraThirdPerson : public CameraInterface, public Object3D
{
    public:
        CameraThirdPerson(const CameraIntrinsic &camera_intrinsic,
                          const Object3D &target,
                          float distance_to_target = 10.0f,
                          float height_to_target = 0.0f,
                          float theta = 0.0f,
                          float phi = M_PI);


        virtual ~CameraThirdPerson();

        // CameraInterface
        void update(const Object3D &object_3d);
        const glm::mat4& getPerspectiveMatrix(void) const;
        glm::mat4 getViewMatrix(void) const;
        void setAspectRatio(float aspect_ratio);

        // Setters
        void addDistanceToTarget(float distance);
        void addHeightToTarget(float distance);
        void mouse(int button, int state, int x, int y);

        // Algorithms
        void setFrameCartesian(const Object3D &target);
        void setFrameSpherical(const Object3D &target);

    private:
        CameraIntrinsic intrinsic_;     //!< Intrinsic parameters of the camera
        float distance_to_target_;
        float height_to_target_;
        float theta_;
        float phi_;
};

#endif /* CameraThirdPerson_HPP_ */
