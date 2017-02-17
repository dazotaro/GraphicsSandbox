/*
 * MeshImporter.cpp
 *
 *  Created on: Feb 16, 2017
 *      Author: jusabiaga
 */

#include "MeshImporter.hpp"
#include "Mesh2.hpp"                // Mesh2
#include <assimp/Importer.hpp>      //  C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <cstdio>                   // std::printf

namespace JU
{

/**
* @brief Assimp importer. Although Assimp will load a whole scene (meshes, animations, bones...)
* we only read one mesh at this point (to be fixed later)
*
* @param filename Name of the file with the scene to import
* @param mesh     Mesh to store the object loaded
*/
bool MeshImporter::import(const char* filename, Mesh2& mesh)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(filename,
                                             aiProcess_GenNormals             |
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
        /*
        if (scene->mNumMeshes != 1)
        {
            std::printf("Scene in file %s contains more than one mesh (%i)\n", filename, scene->mNumMeshes);
            exit(EXIT_FAILURE);
        }
        */

        std::printf("Scene in file %s contains %i meshes\n", filename, scene->mNumMeshes);

        // Merge all meshes into one
        std::string name(filename);
        Mesh2::VectorPositions        vPositions;
        Mesh2::VectorNormals          vNormals;
        Mesh2::VectorTexCoords        vTexCoords;
        Mesh2::VectorVertexIndices    vVertexIndices;
        Mesh2::VectorTriangleIndices  vTriangleIndices;

        for (uint32 i = 0; i < scene->mNumMeshes; i++)
        {

            const aiMesh* pmesh = scene->mMeshes[i];

            std::printf("Number of color channels = %i\n", pmesh->GetNumColorChannels());
            std::printf("Number of UV channels    = %i\n", pmesh->GetNumUVChannels());
            std::printf("Has positions  = %s\n", pmesh->HasPositions() ? "true" : "false");
            std::printf("Has faces      = %s\n", pmesh->HasFaces() ? "true" : "false");
            std::printf("Has normals    = %s\n", pmesh->HasNormals() ? "true" : "false");
            std::printf("Has tan/bit    = %s\n", pmesh->HasTangentsAndBitangents() ? "true" : "false");
            std::printf("Has bones      = %s\n", pmesh->HasBones() ? "true" : "false");

            std::printf("Number of vertices = %i\n", pmesh->mNumVertices);
            std::printf("Number of faces    = %i\n", pmesh->mNumFaces);
            std::printf("Number of indices per face = %i\n", pmesh->mFaces[0].mNumIndices);

            if (!pmesh->mNumVertices || !pmesh->mNumFaces)
            {
                std::printf("Zero vertices or faces (%i, %i)\n", pmesh->mNumVertices, pmesh->mNumFaces);
                exit(EXIT_FAILURE);
            }

            if (pmesh->mFaces[0].mNumIndices != 3)
            {
                std::printf("Number of vertices (%i) != 3\n", pmesh->mFaces[0].mNumIndices);
                exit(EXIT_FAILURE);
            }

            const uint32& num_vertices = pmesh->mNumVertices;
            const uint32& num_faces    = pmesh->mNumFaces;

            vPositions.resize(vPositions.size() + num_vertices);
            vNormals.resize(vNormals.size() + num_vertices);
            vTexCoords.resize(vTexCoords.size() + num_vertices);
            vVertexIndices.resize(vVertexIndices.size() + num_vertices);
            vTriangleIndices.resize(vTriangleIndices.size() + num_faces);

            // Load vertices into Mesh2 format
            memcpy(&vPositions[0], pmesh->mVertices, sizeof(pmesh->mVertices[0]) *  num_vertices);

            // Load normals into Mesh2 format
            memcpy(&vNormals[0], pmesh->mNormals, sizeof(pmesh->mNormals[0]) * num_vertices);

            // Load texture coordinates into Mesh2 format
            memcpy(&vTexCoords[0], pmesh->mTextureCoords, sizeof(pmesh->mTextureCoords[0]) * num_vertices);

            // Load vertex indices to position array, normal array and texture coordinates array
            for (uint32 vertexid = 0; vertexid < num_vertices; vertexid++)
            {
                vVertexIndices[vertexid].position_ = vertexid;
                vVertexIndices[vertexid].normal_   = vertexid;
                vVertexIndices[vertexid].tex_      = vertexid;
            }

            // Load face info into Mesh2 format
            for (uint32 faceid = 0; faceid < num_faces; faceid++)
            {
                vTriangleIndices[faceid].v0_ = pmesh->mFaces[faceid].mIndices[0];
                vTriangleIndices[faceid].v1_ = pmesh->mFaces[faceid].mIndices[1];
                vTriangleIndices[faceid].v2_ = pmesh->mFaces[faceid].mIndices[2];
            }
        }

        mesh = Mesh2(name, vPositions, vNormals, vTexCoords, vVertexIndices, vTriangleIndices);
    }


    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

} /* namespace JU */
