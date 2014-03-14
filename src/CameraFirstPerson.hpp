/*
 * CameraFirstPerson.hpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

#ifndef CAMERAFIRSTPERSON_HPP_
#define CAMERAFIRSTPERSON_HPP_

#include "CameraInterface.hpp"      // CameraInterface parent class
#include "CameraIntrinsic.hpp"      // CameraIntrinsic object contained
#include "Object3D.hpp"             // Object3D

/**
 * @brief   First-person camera type
 *
 * @detail  It implements the CameraInterface interface
 */
class CameraFirstPerson : public CameraInterface, public Object3D
{
    public:
        CameraFirstPerson(const CameraIntrinsic &camera_intrinsic, const Object3D &first_person);
        virtual ~CameraFirstPerson();

        // CameraInterface
        void update(const Object3D &first_person);
        const glm::mat4& getPerspectiveMatrix(void) const;
        glm::mat4 getViewMatrix(void) const;
        void setAspectRatio(float aspect_ratio);

    private:
        CameraIntrinsic intrinsic_;     //!< Intrinsic parameters of this camera
};

#endif /* CameraFirstPerson_HPP_ */
