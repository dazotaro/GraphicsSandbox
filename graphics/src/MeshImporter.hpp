/*
 * MeshImporter.hpp
 *
 *  Created on: Feb 16, 2017
 *      Author: jusabiaga
 */

#ifndef MESHIMPORTER_HPP_
#define MESHIMPORTER_HPP_

// Forward Declarations
class Mesh2;

namespace JU
{


class MeshImporter
{
    public:
        MeshImporter();
        virtual ~MeshImporter();

    public:
        bool import(const char* filename, Mesh2& mesh) const;
};

} /* namespace JU */

#endif /* MESHIMPORTER_HPP_ */
