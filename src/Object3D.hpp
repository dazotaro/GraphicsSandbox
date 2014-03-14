/*
 * Object3D.hpp
 *
 *  Created on: May 7, 2013
 *      Author: jusabiaga
 */

#ifndef OBJECT3D_HPP_
#define OBJECT3D_HPP_

#include <iostream>
#include <glm/glm.hpp>

/*!
  This class represents an object in 3D: its position and orientation.
*/
class Object3D
{
    public:
        //Object3D();
        Object3D(const glm::vec3 &position = glm::vec3(0.f, 0.f, 0.f),
                 const glm::vec3 &x_axis   = glm::vec3(1.f, 0.f, 0.f),
                 const glm::vec3 &y_axis   = glm::vec3(0.f, 1.f, 0.f),
                 const glm::vec3 &z_axis   = glm::vec3(0.f, 0.f, 1.f));
        //Object3D() {}
        virtual ~Object3D();

        // GETTERS
        inline const glm::vec3& getPosition(void) const
        {
            return position_;
        }
        inline const glm::vec3& getXAxis(void) const
        {
            return x_axis_;
        }
        inline const glm::vec3& getYAxis(void) const
        {
            return y_axis_;
        }
        inline const glm::vec3& getZAxis(void) const
        {
            return z_axis_;
        }

        // SETTERS
        void setPosition(const glm::vec3 &position);
        void setXAxis(const glm::vec3 &x_axis);
        void setYAxis(const glm::vec3 &y_axis);
        void setZAxis(const glm::vec3 &z_axis);

        // OBJECT CONTROL FUNCTIONS
        void translate(const glm::vec3 &translate);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        // Transformation Functions
        glm::mat4 getTransformToParent(void) const;
        glm::mat4 getTransformFromParent(void) const;

        // FRIENDS
        friend std::ostream & operator<<(std::ostream &out, const Object3D &rhs);

    protected:
        glm::vec3 position_;    //!< Position of the object (in the parent's coordinate system)
        glm::vec3 x_axis_;      //!< back-to-front axis (in the parent's coordinate system)
        glm::vec3 y_axis_;      //!< left-to-right
        glm::vec3 z_axis_;      //!< bottom-up
};

#endif /* OBJECT3D_HPP_ */
