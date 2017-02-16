/*
 * MeshImporter.cpp
 *
 *  Created on: Feb 16, 2017
 *      Author: jusabiaga
 */

#include "MeshImporter.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <cstdio>                   // std::printf

namespace JU
{

MeshImporter::MeshImporter()
{
    // TODO Auto-generated constructor stub

}

MeshImporter::~MeshImporter()
{
    // TODO Auto-generated destructor stub
}


/**
* @brief Assimp importer. Although Assimp will load a whole scene (meshes, animations, bones...)
* we only read one mesh at this point (to be fixed later)
*
* @param filename Name of the file with the scene to import
* @param mesh     Mesh to store the object loaded
*/
bool import(const char* filename, Mesh2& mesh) const
{
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(filename,
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        //DoTheErrorLogging( importer.GetErrorString());
        std::printf("Could not import file");
        return false;
    }

    // Load the mesh in the scene into a Mesh2 object
    std::printf("Has Meshes:     %s\n", scene->HasMeshes() ? "true" : "false");
    std::printf("Has Textures:   %s\n", scene->HasTextures() ? "true" : "false");
    std::printf("Has Materials:  %s\n", scene->HasMaterials() ? "true" : "false");
    std::printf("Has Lights:     %s\n", scene->HasLights() ? "true" : "false");
    std::printf("Has Cameras:    %s\n", scene->HasCameras() ? "true" : "false");
    std::printf("Has Animations: %s\n", scene->HasAnimations() ? "true" : "false");

    if (scene->HasMeshes())
    {
        // NOTE: Although Assimp can load more than one mesh per scene (depending on the assets format),
        // at this point, only one mesh is processed per asset file
        if (scene->mNumMeshes != 1)
        {
            std::printf("Scene in file %s contains more than one mesh\n", filename);
            exit(EXIT_FAILURE);
        }

        std::printf("Number of color channels = %i\n", scene->mMeshes[0]->GetNumColorChannels());
        std::printf("Number of UV channels    = %i\n", scene->mMeshes[0]->GetNumUVChannels());
        std::printf("Has positions  = %s\n", scene->mMeshes[0]->HasPositions() ? "true" : "false");
        std::printf("Has faces      = %s\n", scene->mMeshes[0]->HasFaces() ? "true" : "false");
        std::printf("Has normals    = %s\n", scene->mMeshes[0]->HasNormals() ? "true" : "false");
        std::printf("Has tan/bit    = %s\n", scene->mMeshes[0]->HasTangentsAndBitangents() ? "true" : "false");
        std::printf("Has bones      = %s\n", scene->mMeshes[0]->HasBones() ? "true" : "false");

        std::printf("Number of vertices = %i\n", scene->mMeshes[0]->mNumVertices);
        std::printf("Number of faces    = %i\n", scene->mMeshes[0]->mNumFaces);
        std::printf("Number of indices per face = %i\n", scene->mMeshes[0]->mFaces[0].mNumIndices);

        /*
        std::printf("Vertex 0 = (%f, %f, %f)\n", scene->mMeshes[0]->mVertices[0].x,
                                                 scene->mMeshes[0]->mVertices[0].y,
                                                 scene->mMeshes[0]->mVertices[0].z);
        std::printf("Normal 0 = (%f, %f, %f)\n", scene->mMeshes[0]->mNormals[0].x,
                                                 scene->mMeshes[0]->mNormals[0].y,
                                                 scene->mMeshes[0]->mNormals[0].z);
        std::printf("Face 0 = (%i, %i, %i)\n", scene->mMeshes[0]->mFaces[0].mIndices[0],
                                               scene->mMeshes[0]->mFaces[0].mIndices[1],
                                               scene->mMeshes[0]->mFaces[0].mIndices[2]);
       */
    }


    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

} /* namespace JU */
