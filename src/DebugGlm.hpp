/*
 * DebugGlm.hpp
 *
 *  Created on: May 10, 2013
 *      Author: jusabiaga
 */

#ifndef DEBUGGLM_HPP_
#define DEBUGGLM_HPP_

#include <string>     // std::cout, std::endl

namespace
{
std::ostream& operator<<(std::ostream &out, const glm::vec3 &rhs)
{
    out << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";

    return out;
}

std::ostream& operator<<(std::ostream &out, const glm::vec4 &rhs)
{
    out << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << ", "<< rhs.w << "]";

    return out;
}

std::ostream& operator<<(std::ostream &out, const glm::mat4 &rhs)
{
    out << "[";
    out <<        rhs[0][0] << ", " << rhs[1][0] << ", " << rhs[2][0] << ", "<< rhs[3][0] << std::endl;
    out << " " << rhs[0][1] << ", " << rhs[1][1] << ", " << rhs[2][1] << ", "<< rhs[3][1] << std::endl;
    out << " " << rhs[0][2] << ", " << rhs[1][2] << ", " << rhs[2][2] << ", "<< rhs[3][2] << std::endl;
    out << " " << rhs[0][3] << ", " << rhs[1][3] << ", " << rhs[2][3] << ", "<< rhs[3][3];
    out << "]";

    return out;
}

}

#endif /* DEBUGGLM_HPP_ */
