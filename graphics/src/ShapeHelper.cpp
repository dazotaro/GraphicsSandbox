//#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "ShapeHelper.hpp"  // ShapeHelper definitions
#include "Mesh.hpp"         // Mesh class

namespace Graphics
{

/**
* @brief Builder function
*
* It generates the Mesh for the given shape
*
* @oaram shape_type Shape to build (cube, sphere...)
* @param num_slices To be used only with cylinders, cones and spheres.
* @param num_stacs  To be used with spheres only.
*
* @return The Mesh
*/
Mesh buildMesh(ShapeType shape_type, unsigned int num_slices, unsigned int num_stacks)
{
    std::string        name;
    Mesh::PositionList positions;
    Mesh::NormalList   normals;
    Mesh::TangentList  tangents;
    Mesh::ColorList    colors;
    Mesh::TexCoordList tex_coords;
    Mesh::FaceList     faces;

    switch(shape_type)
    {
    case PLANE:
        buildPlane(name, positions, colors, tex_coords, faces);
        break;

    case CUBE:
        buildCube(name, positions, colors, tex_coords, faces);
        break;

    case CYLINDER:
        buildCylinder(name, positions, colors, tex_coords, faces, num_slices);
        break;

    case CONE:
        buildCone(name, positions, colors, tex_coords, faces, num_slices);
        break;

    case SPHERE:
        buildSphere(name, positions, colors, tex_coords, faces, num_slices, num_stacks);
        break;

    default:
        std::cout << "Error: Shape type " << shape_type << " not handled by 'buildMesh'" << std::endl;
        break;
    }

    computeNormals (positions, faces, normals);
    computeTangents(positions, faces, normals, tex_coords, tangents);

    return Mesh(name, positions, normals, tangents, colors, tex_coords, faces);
}

/**
* @brief Plane builder function
*
* It generates the Mesh for a plane with sides length one
* 
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
void buildPlane(std::string        &name,
                Mesh::PositionList &positions,
                Mesh::ColorList    &colors,
                Mesh::TexCoordList &tex_coords,
                Mesh::FaceList     &faces)
{
    // Clear vectors
    positions.clear();
    colors.clear();
    tex_coords.clear();
    faces.clear();

    // Name
    name = "Plane";

    // Position in model coordinates
    positions.reserve(4);
    positions.push_back(glm::vec3(-0.5f,  0.5f, 0.0f));
    positions.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
    positions.push_back(glm::vec3( 0.5f, -0.5f, 0.0f));
    positions.push_back(glm::vec3( 0.5f,  0.5f, 0.0f));

    // Colors in model coordinates
    colors.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Texture coordinates
    tex_coords.reserve(4);
    tex_coords.push_back(glm::vec2(0.0f, 1.0f));
    tex_coords.push_back(glm::vec2(0.0f, 0.0f));
    tex_coords.push_back(glm::vec2(1.0f, 0.0f));
    tex_coords.push_back(glm::vec2(1.0f, 1.0f));

    // Faces
    faces.push_back(Mesh::TriangleFace(0, 1, 2,   // Positions
                                       0, 0, 0,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(0, 2, 3,   // Positions
                                       0, 0, 0,   // Colors
                                       0, 2, 3)); // Texture Coordinates
}

/**
* @brief Cube builder function
*
* It generates the Mesh for a cube with sides length one
*
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
void buildCube(std::string        &name,
               Mesh::PositionList &positions,
               Mesh::ColorList    &colors,
               Mesh::TexCoordList &tex_coords,
               Mesh::FaceList     &faces)
{
    // Normals in model coordinates
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);

    const unsigned int PURPLE_INDEX  = 0;
    const unsigned int BLUE_INDEX    = 1;
    const unsigned int RED_INDEX     = 2;
    const unsigned int GREEN_INDEX   = 3;
    const unsigned int GRAY_INDEX    = 4;
    const unsigned int YELLOW_INDEX  = 5;

    // Clear vectors
    positions.clear();
    colors.clear();
    tex_coords.clear();
    faces.clear();

    // Name
    name = "Cube";

    // Position in model coordinates
    positions.reserve(8);
    positions.push_back(glm::vec3(-0.5f, -0.5f, +0.5f));
    positions.push_back(glm::vec3(+0.5f, -0.5f, +0.5f));
    positions.push_back(glm::vec3(+0.5f, +0.5f, +0.5f));
    positions.push_back(glm::vec3(-0.5f, +0.5f, +0.5f));
    positions.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
    positions.push_back(glm::vec3(+0.5f, -0.5f, -0.5f));
    positions.push_back(glm::vec3(+0.5f, +0.5f, -0.5f));
    positions.push_back(glm::vec3(-0.5f, +0.5f, -0.5f));

    colors.reserve(6);
    colors.push_back(PURPLE);
    colors.push_back(BLUE);
    colors.push_back(RED);
    colors.push_back(GREEN);
    colors.push_back(GRAY);
    colors.push_back(YELLOW);

    tex_coords.reserve(4);
    tex_coords.push_back(glm::vec2(0.0f, 1.0f));
    tex_coords.push_back(glm::vec2(0.0f, 0.0f));
    tex_coords.push_back(glm::vec2(1.0f, 0.0f));
    tex_coords.push_back(glm::vec2(1.0f, 1.0f));

    // Faces
    faces.reserve(12);
    faces.push_back(Mesh::TriangleFace(0, 1, 2,   // Positions
                                       PURPLE_INDEX, PURPLE_INDEX, PURPLE_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(0, 2, 3,   // Positions
                                       PURPLE_INDEX, PURPLE_INDEX, PURPLE_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(1, 5, 6,   // Positions
                                       BLUE_INDEX, BLUE_INDEX, BLUE_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(1, 6, 2,   // Positions
                                       BLUE_INDEX, BLUE_INDEX, BLUE_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(5, 4, 7,   // Positions
                                       RED_INDEX, RED_INDEX, RED_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(5, 7, 6,   // Positions
                                       RED_INDEX, RED_INDEX, RED_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(4, 0, 3,   // Positions
                                       GREEN_INDEX, GREEN_INDEX, GREEN_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(4, 3, 7,   // Positions
                                       GREEN_INDEX, GREEN_INDEX, GREEN_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(3, 2, 6,   // Positions
                                       GRAY_INDEX, GRAY_INDEX, GRAY_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(3, 6, 7,   // Positions
                                       GRAY_INDEX, GRAY_INDEX, GRAY_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(4, 5, 1,   // Positions
                                       YELLOW_INDEX, YELLOW_INDEX, YELLOW_INDEX,   // Colors
                                       0, 1, 2)); // Texture Coordinates
    faces.push_back(Mesh::TriangleFace(4, 1, 0,   // Positions
                                       YELLOW_INDEX, YELLOW_INDEX, YELLOW_INDEX,   // Colors
                                       0, 2, 3)); // Texture Coordinates
}

/**
* @brief Cylinder builder function
*
* It generates the Mesh for a cylinder which fits in a cube of size 1.0.
*
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
void buildCylinder(std::string        &name,
                   Mesh::PositionList &positions,
                   Mesh::ColorList    &colors,
                   Mesh::TexCoordList &tex_coords,
                   Mesh::FaceList     &faces,
                   unsigned int        num_slices)
{
    // CONSTANTS
    const glm::vec3 ORIGIN (0.0f, 0.0f, 0.0f);          // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice

    // CLEAN-UP
    positions.clear();
    colors.clear();
    tex_coords.clear();
    faces.clear();

    // SET NAME
    name = "Cylinder_" + num_slices;

    // COLORS
    colors.push_back(GRAY);    // GREEN

    // CENTER VERTICES (FOR TOP AND BOTTOM DISKS)
    //-------------------------------------------
    const glm::vec3 center_top   (  0.0f, 0.0f,  Z_OFFSET);
    const glm::vec3 center_bottom(  0.0f, 0.0f, -Z_OFFSET);
    positions.push_back(center_top);
    positions.push_back(center_bottom);

    const unsigned int top_disk_offset = positions.size();  // This is the offset to start accessing the top-disk vertices
    const unsigned int top_center_index = 0;                // Index to the position and normal of the center of the top disk
    const unsigned int bottom_center_index = 1;             // Index to the position and normal of the center of the bottom disk

    // TOP-DISK VERTICES
    //-------------------------------------------
    float theta = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        // Generate four triangles per slice:
        //    - One pie-slice on the top (top_center, top_RADIUS_left, top_RADIUS_right)
        //    - Two on the side (which form a rectangle on the along the side of cylinder)
        //      (top_RADIUS_left, bottom_RADIUS_left, bottom_RADIUS_right)
        //      (top_RADIUS_left, bottom_RADIUS_right, top_RADIUS_right)
        //    - One pie-sice on the bottom (bottom_RADIUS_left, bottom_center, bottom_RADIUS_right)
        glm::vec3 top = glm::vec3(RADIUS * cos(theta), RADIUS * sin(theta),  Z_OFFSET);

        positions.push_back(top);

        theta += DELTA_THETA;
    }

    const unsigned int bottom_disk_offset = positions.size();   // This is the offset to start accessing the top-disk vertices

    // BOTTOM-DISk VERTICES
    //-------------------------------------------
    theta = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        // Generate four triangles per slice:
        //    - One pie-slice on the top (top_center, top_RADIUS_left, top_RADIUS_right)
        //    - Two on the side (which form a rectangle on the along the side of cylinder)
        //      (top_RADIUS_left, bottom_RADIUS_left, bottom_RADIUS_right)
        //      (top_RADIUS_left, bottom_RADIUS_right, top_RADIUS_right)
        //    - One pie-sice on the bottom (bottom_RADIUS_left, bottom_center, bottom_RADIUS_right)
        glm::vec3 bottom = glm::vec3(RADIUS * cos(theta), RADIUS * sin(theta), -Z_OFFSET);

        positions.push_back(bottom);

        theta += DELTA_THETA;
    }

    // TEXTURE COORDINATES
    //-------------------------------------------
    const float delta_U = 1.0f / num_slices;    // Divide U into as many intervals as slices
    const float half_delta_U = delta_U * 0.5f;  // The top and bottom's middle vertex takes the average U of the other two vertices on the rim

    // TOP DISK TEXTURE COORDINATES
    const unsigned int top_disk_tex_coords_offset = 0;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 1.0f));  // TOP
    }

    // MIDDLE DISKS TEXTURE COORDINATES
    const unsigned int side_top_disk_tex_coords_offset = tex_coords.size();
    for (unsigned int slice = 0; slice <= num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U, 1.0f));  // TOP
    }
    const unsigned int side_bot_disk_tex_coords_offset = tex_coords.size();
    for (unsigned int slice = 0; slice <= num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U, 0.0f));  // TOP
    }

    // BOTTOM DISK TEXTURE COORDINATES
    const unsigned int bottom_disk_tex_coords_offset = tex_coords.size();
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 0.0f));  // BOTTOM
    }

    // FACES: six faces per slice
    //-------------------------------------------
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        // The four vertices that make the side of the cylinder are
        unsigned int top_left     = top_disk_offset + slice;
        unsigned int top_right    = top_disk_offset + slice + 1;
        unsigned int bottom_left  = bottom_disk_offset + slice;
        unsigned int bottom_right = bottom_disk_offset + slice + 1;
        unsigned int tex_coord_top_center   = top_disk_tex_coords_offset + slice;
        unsigned int tex_coord_top_left     = side_top_disk_tex_coords_offset + slice;
        unsigned int tex_coord_top_right    = side_top_disk_tex_coords_offset + slice + 1;
        unsigned int tex_coord_bot_center   = bottom_disk_tex_coords_offset + slice;
        unsigned int tex_coord_bottom_left  = side_bot_disk_tex_coords_offset + slice;
        unsigned int tex_coord_bottom_right = side_bot_disk_tex_coords_offset + slice + 1;

        // Wrap it around for the last slice
        if (slice == (num_slices - 1))
        {
            top_right    = top_disk_offset;
            bottom_right = bottom_disk_offset;
            //tex_coord_top_right    = top_disk_tex_coords_offset;
            //tex_coord_bottom_right = bottom_disk_tex_coords_offset;
        }

        // Triangle on the top disk
        faces.push_back(Mesh::TriangleFace(top_center_index, top_left, top_right,                             // Positions
                                           0, 0, 0,                                                           // Colors
                                           tex_coord_top_center, tex_coord_top_left, tex_coord_top_right));   // Texture Coordinates

        // Two Triangles on the side of the cylinder
        faces.push_back(Mesh::TriangleFace(top_left, bottom_left, bottom_right,                                 // Positions
                                           0, 0, 0,                                                             // Colors
                                           tex_coord_top_left, tex_coord_bottom_left, tex_coord_bottom_right)); // Texture Coordinates

        faces.push_back(Mesh::TriangleFace(top_left, bottom_right, top_right,                                   // Positions
                                           0, 0, 0,                                                             // Colors
                                           tex_coord_top_left, tex_coord_bottom_right, tex_coord_top_right));   // Texture Coordinates

        // Triangle on the bottom disk
        faces.push_back(Mesh::TriangleFace(bottom_center_index, bottom_right, bottom_left,                           // Positions
                                           0, 0, 0,                                                                  // Colors
                                           tex_coord_bot_center, tex_coord_bottom_right, tex_coord_bottom_left));  // Texture Coordinates
    }
}

/**
* @brief Cone builder function
*
* It generates the Mesh for a cone which fits in a cube of size 1.0.
*
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
void buildCone(std::string        &name,
               Mesh::PositionList &positions,
               Mesh::ColorList    &colors,
               Mesh::TexCoordList &tex_coords,
               Mesh::FaceList     &faces,
               unsigned int        num_slices)
{
    // CONSTANTS
    const glm::vec3 ORIGIN(0.0f, 0.0f, 0.0f);           // ORIGIN of the Mesh in Model Coordinates
    const float Z_OFFSET (0.5f);                        // Distance from the ORIGIN to the top (or bottom)
    const float RADIUS   (0.5f);                        // RADIUS of the cylinder
    const float DELTA_THETA (2 * M_PI / num_slices);    // Increment of the angle from slice to slice

    // CLEAN-UP
    positions.clear();
    colors.clear();
    tex_coords.clear();
    faces.clear();

    // SET NAME
    name = "Cone_" + num_slices;

    // COLORS
    colors.push_back(GRAY);

    // CENTER VERTICES (FOR TOP AND BOTTOM DISKS)
    //-------------------------------------------
    const glm::vec3 apex       (0.0f, 0.0f,  Z_OFFSET);
    const glm::vec3 disk_center(0.0f, 0.0f, -Z_OFFSET);
    positions.push_back(apex);
    positions.push_back(disk_center);

    const unsigned int disk_offset = positions.size();  // This is the offset to start accessing the base-disk vertices
    const unsigned int apex_index = 0;                  // Index to the position and normal of the apex vertex
    const unsigned int disk_center_index = 1;           // Index to the position and normal of the center of the base-disk

    // DISK VERTICES
    //-------------------------------------------
    float theta = 0.0f;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        // Generate four triangles per slice:
        //    - One pie-slice on the top (top_center, top_RADIUS_left, top_RADIUS_right)
        //    - Two on the side (which form a rectangle on the along the side of cylinder)
        //      (top_RADIUS_left, bottom_RADIUS_left, bottom_RADIUS_right)
        //      (top_RADIUS_left, bottom_RADIUS_right, top_RADIUS_right)
        //    - One pie-sice on the bottom (bottom_RADIUS_left, bottom_center, bottom_RADIUS_right)
        glm::vec3 vertex = glm::vec3(RADIUS * cos(theta), RADIUS * sin(theta),  Z_OFFSET);

        positions.push_back(vertex);

        theta += DELTA_THETA;
    }

    // TEXTURE COORDINATES
    //-------------------------------------------
    const float delta_U = 1.0f / num_slices;    // Divide U into as many intervals as slices
    const float half_delta_U = delta_U * 0.5f;  // The top triangles take the average U coordinate of its other two vertices

    // TOP DISK TEXTURE COORDINATES
    const unsigned int tex_coords_apex_offset = 0;
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 1.0f));  // TOP
    }

    // BOTTOM DISK TEXTURE COORDINATES
    const unsigned int tex_coords_disk_offset = tex_coords.size();
    for (unsigned int slice = 0; slice <= num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U, 0.0f));  // BOTTOM
    }

    // BOTTOM DISK CENTER TEXTURE COORDINATES
    const unsigned int tex_coords_center_offset = tex_coords.size();
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 0.0f));  // BOTTOM
    }

    // FACES: Two triangles per slice
    //-------------------------------------------
    for (unsigned int slice = 0; slice < num_slices; slice++)
    {
        // The four vertices that make the side of the cylinder are
        unsigned int disk_left     = disk_offset + slice;
        unsigned int disk_right    = disk_offset + slice + 1;
        unsigned int tex_coord_appex       = tex_coords_apex_offset + slice;
        unsigned int tex_coord_disk_left   = tex_coords_disk_offset + slice;
        unsigned int tex_coord_disk_right  = tex_coords_disk_offset + slice + 1;
        unsigned int tex_coord_center      = tex_coords_center_offset + slice;

        // Wrap it around for the last slice
        if (slice == (num_slices - 1))
        {
            disk_right = disk_offset;
            //tex_coord_disk_right  = tex_coords_disk_offset;
        }

        // Triangle from the Apex to the base disk
        faces.push_back(Mesh::TriangleFace(apex_index, disk_left, disk_right,                            // Positions
                                           0, 0, 0,                                                      // Colors
                                           tex_coord_appex, tex_coord_disk_left, tex_coord_disk_right)); // Texture Coordinates

        // Triangle on the base disk
        faces.push_back(Mesh::TriangleFace(disk_center_index, disk_right, disk_left,                         // Positions
                                           0, 0, 0,                                                          // Colors
                                           tex_coord_center, tex_coord_disk_right, tex_coord_disk_left)); // Texture Coordinates
    }
}

/**
* @brief Sphere builder function
*
* It generates the Mesh for a Sphere which fits in a cube of size 1.0
*
* @param name Id of the Mesh
* @param positions Vector with all vertex positions
* @param colors Vector with all vertex colors
* @param tex_coords Vector with all vertex colors
* @param faces Vector with all info about the faces of the Mesh
*/
void buildSphere(std::string        &name,
                 Mesh::PositionList &positions,
                 Mesh::ColorList    &colors,
                 Mesh::TexCoordList &tex_coords,
                 Mesh::FaceList     &faces,
                 unsigned int        num_slices,
                 unsigned int        num_stacks)
{
    const glm::vec3 ORIGIN (glm::vec3(0.0f, 0.0f, 0.0f));   // ORIGIN of the Mesh
    const float RADIUS (0.5f);                              // RADIUS of the sphere
    const float DELTA_THETA (2 * M_PI / num_slices);        // Angle increments for each slice
    const float DELTA_PHI (M_PI / num_stacks);              // Angle increments for each stack

    // CLEAN-UP
    positions.clear();
    colors.clear();
    tex_coords.clear();
    faces.clear();

    // SET NAME
    char tmp [100];
    sprintf(tmp, "Sphere_%i_%i", num_slices, num_stacks);
    name = tmp;

    // COLORS
    colors.push_back(GRAY);

    // VERTICES/NORMALS
    //-------------------------------------------
    std::vector<unsigned int> stack_offsets (num_stacks + 1);

    // TOP
    const glm::vec3 top (0.0f, 0.0f, RADIUS);
    positions.push_back(top);
    stack_offsets[0] = 0; // Index to the position and normal of the center of the top disk

    // VERTICES/NORMALS PER STACK (excluding top and bottom)
    //-------------------------------------------
    float phi = 0.0f;
    for (unsigned int stack = 1; stack < num_stacks; ++stack)
    {
        float theta = 0.0f;
        stack_offsets[stack] = positions.size();        // To keep track of where the first position/normal of this stack are
        // VERTICES/NORMALS PER SLICE
        phi += DELTA_PHI;
        for (unsigned int slice = 0; slice < num_slices; ++slice)
        {
            glm::vec3 vertex  = glm::vec3(RADIUS * cos(theta) * sin(phi), RADIUS * sin(theta) * sin(phi), RADIUS * cos(phi));

            positions.push_back(vertex);

            theta += DELTA_THETA;
        }
    }

    // BOTTOM
    stack_offsets[num_stacks] = positions.size();             // Index to the position and normal of the center of the bottom disk
    const glm::vec3 bottom(  0.0f, 0.0f, -RADIUS);
    positions.push_back(bottom);

    // TEXTURE COORDINATES (spherical mapping)
    //-------------------------------------------
    float delta_V = 1.0f / num_stacks;      // Increments for the texture coordinate U
    float delta_U = 1.0f / num_slices;      // Increments for the texture coordinate V
    float half_delta_U = delta_U * 0.5f;    // Top and bottom vertices take their U coord at half the other two vertices
    std::vector<unsigned int> tex_coord_stack_offsets (num_stacks + 1);     // It stores the offsets to the tex_coords vector for the beginning of a stack

    // The top and bottom vertices have different texture coordinates per face they appear.
    // TOP: the zeroth stack
    tex_coord_stack_offsets[0] = 0;
    for (unsigned int slice = 0; slice < num_slices; ++slice)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 1.0f));
    }

    // MIDDLE: all the other stacks
    for (unsigned int stack = 1; stack < num_stacks; ++stack)
    {
        tex_coord_stack_offsets[stack] = tex_coords.size();
        for (unsigned int slice = 0; slice <= num_slices; ++slice)
        {
            tex_coords.push_back(glm::vec2(slice * delta_U, 1.0f - stack * delta_V));
        }
    }

    // BOTTOM: last stack + 1
    tex_coord_stack_offsets[num_stacks] = tex_coords.size();
    for (unsigned int slice = 0; slice < num_slices; ++slice)
    {
        tex_coords.push_back(glm::vec2(slice * delta_U + half_delta_U, 0.0f));
    }

    // FACES:
    //-------------------------------------------
    // TOP stack (the pope-hat that contains the top vertex)
    for (unsigned int slice = 0; slice < num_slices; ++slice)
    {
        unsigned int top   = stack_offsets[0];
        unsigned int left  = stack_offsets[1] + slice;
        unsigned int right = stack_offsets[1] + slice + 1;
        unsigned int tex_top   = tex_coord_stack_offsets[0] + slice;
        unsigned int tex_left  = tex_coord_stack_offsets[1] + slice;
        unsigned int tex_right = tex_coord_stack_offsets[1] + slice + 1;
        // Wrap it around
        if (slice == (num_slices - 1))
        {
            right = stack_offsets[1];
            //tex_right = tex_coord_stack_offsets[0];
        }

        // Triangle from the Apex to the base disk
        faces.push_back(Mesh::TriangleFace(top, left, right,                // Positions
                                           0, 0, 0,                         // Colors
                                           tex_top, tex_left, tex_right));  // Texture Coordinates
    }

    // MIDDLE SECTION OF THE SPHERE
    for (unsigned int stack = 1; stack < (num_stacks - 1); ++stack)
    {
        for (unsigned int slice = 0; slice < num_slices; ++slice)
        {
            unsigned int top_left     = stack_offsets[stack] + slice;
            unsigned int top_right    = stack_offsets[stack] + slice + 1;
            unsigned int bottom_left  = stack_offsets[stack + 1] + slice;
            unsigned int bottom_right = stack_offsets[stack + 1] + slice + 1;
            unsigned int tex_top_left     = tex_coord_stack_offsets[stack] + slice;
            unsigned int tex_top_right    = tex_coord_stack_offsets[stack] + slice + 1;
            unsigned int tex_bottom_left  = tex_coord_stack_offsets[stack + 1] + slice;
            unsigned int tex_bottom_right = tex_coord_stack_offsets[stack + 1] + slice + 1;
            // Wrap it around
            if (slice == (num_slices - 1))
            {
                top_right    = stack_offsets[stack];
                bottom_right = stack_offsets[stack + 1];
                //tex_top_right    = tex_coord_stack_offsets[stack];
                //tex_bottom_right = tex_coord_stack_offsets[stack + 1];
            }

            // Triangle from the Apex to the base disk
            faces.push_back(Mesh::TriangleFace(top_left, bottom_left, bottom_right,                 // Positions
                                               0, 0, 0,                                             // Colors
                                               tex_top_left, tex_bottom_left, tex_bottom_right));   // Texture Coordinates
            // Triangle from the Apex to the base disk
            faces.push_back(Mesh::TriangleFace(top_left, bottom_right, top_right,                // Positions
                                               0, 0, 0,                                          // Colors
                                               tex_top_left, tex_bottom_right, tex_top_right));  // Texture Coordinates

        }
    }


    // BOTTOM stack (the pope hat that contains the bottom vertex)
    for (unsigned int slice = 0; slice < num_slices; ++slice)
    {
        unsigned int left   = stack_offsets[num_stacks - 1] + slice;
        unsigned int right  = stack_offsets[num_stacks - 1] + slice + 1;
        unsigned int bottom = stack_offsets[num_stacks];
        unsigned int tex_left   = tex_coord_stack_offsets[num_stacks - 1] + slice;
        unsigned int tex_right  = tex_coord_stack_offsets[num_stacks - 1] + slice + 1;
        unsigned int tex_bottom = tex_coord_stack_offsets[num_stacks] + slice;
        // Wrap it around
        if (slice == (num_slices - 1))
        {
            right = stack_offsets[num_stacks - 1];
            //tex_right = tex_coord_stack_offsets[num_stacks - 1];
        }

        // Triangle from the Apex to the base disk
        faces.push_back(Mesh::TriangleFace(bottom, right, left,                 // Positions
                                           0, 0, 0,                             // Colors
                                           tex_bottom, tex_right, tex_left));   // Texture Coordinates
    }
}


/**
* @brief Compute Normals
*
* It compute the per-vertex normals for the given mesh
*
* @param positions Vector with all vertex positions
* @param faces Vector with all info about the faces of the Mesh
* @param normals Vector with all vertex normals
*/
void computeNormals(const Mesh::PositionList &positions,
                    const Mesh::FaceList     &faces,
                    Mesh::NormalList         &normals)
{
    // Same number of normals as vertices
    normals.clear();
    normals.resize(positions.size(), glm::vec3(0.0f, 0.0f, 0.0f));

    for (Mesh::FaceListConstIterator iter = faces.begin(); iter != faces.end(); ++iter)
    {
        // Retrieve indices to the positions of the vertices of this face
        Mesh::IndexList vertex_indices;
        iter->getIndices(Mesh::Face::VERTEX_INDEX, vertex_indices);
        // Compute the normal of the face
        glm::vec3 vec1 = positions[vertex_indices[1]] - positions[vertex_indices[0]];
        glm::vec3 vec2 = positions[vertex_indices[2]] - positions[vertex_indices[0]];
        glm::vec3 norm = glm::normalize(glm::cross(vec1, vec2));
        // Accumulate the normal for each of the face's vertices
        normals[vertex_indices[0]] += norm;
        normals[vertex_indices[1]] += norm;
        normals[vertex_indices[2]] += norm;
    }

    for(Mesh::NormalListIterator iter = normals.begin(); iter != normals.end(); ++iter)
    {
        *iter = glm::normalize(*iter);
    }
}


/**
* @brief Compute Tangents
*
* It compute the per-vertex tangents for the given mesh
*
* @param positions Vector with all vertex positions
* @param faces Vector with all info about the faces of the Mesh
* @param normals Vector with all vertex normals
* @param tangents Vector with all vertex tangents
*/
void computeTangents(const Mesh::PositionList &positions,
                     const Mesh::FaceList     &faces,
                     const Mesh::NormalList   &normals,
                     const Mesh::TexCoordList &tex_coords,
                     Mesh::TangentList        &tangents)
{
    // Same number of tangents as vertices
    unsigned int num_vertices = positions.size();
    tangents.clear();
    tangents.resize(num_vertices, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    // Temporary vectors
    std::vector<glm::vec3> tan (num_vertices, glm::vec3(0.0f, 0.0f, 0.0f));
    std::vector<glm::vec3> bit (num_vertices, glm::vec3(0.0f, 0.0f, 0.0f));

    for (Mesh::FaceListConstIterator iter = faces.begin(); iter != faces.end(); ++iter)
    {
        // Retrieve indices to the positions of the vertices of this face
        Mesh::IndexList vertex_indices;
        iter->getIndices(Mesh::Face::VERTEX_INDEX, vertex_indices);

        // Compute the normal of the face
        glm::vec3 vec1 = glm::normalize(positions[vertex_indices[1]] - positions[vertex_indices[0]]);
        glm::vec3 vec2 = glm::normalize(positions[vertex_indices[2]] - positions[vertex_indices[0]]);

        Mesh::IndexList tex_coord_indices;
        iter->getIndices(Mesh::Face::TEXTURE_INDEX, tex_coord_indices);

        static unsigned face_id = 0;
        std::printf("Face %i/%i\n", face_id++, faces.size());
        std::printf("%f, %f)\n", tex_coords[tex_coord_indices[0]].x, tex_coords[tex_coord_indices[0]].y);
        std::printf("%f, %f)\n", tex_coords[tex_coord_indices[1]].x, tex_coords[tex_coord_indices[1]].y);
        std::printf("%f, %f)\n", tex_coords[tex_coord_indices[2]].x, tex_coords[tex_coord_indices[2]].y);

        /*
        glm::vec2 tex1 = glm::normalize(tex_coords[tex_coord_indices[1]] -tex_coords[tex_coord_indices[0]]);
        glm::vec2 tex2 = glm::normalize(tex_coords[tex_coord_indices[2]] -tex_coords[tex_coord_indices[0]]);
        */
        glm::vec2 tex1 = tex_coords[tex_coord_indices[1]] -tex_coords[tex_coord_indices[0]];
        glm::vec2 tex2 = tex_coords[tex_coord_indices[2]] -tex_coords[tex_coord_indices[0]];

        float factor = 1 / (tex1.s*tex2.t - tex2.s*tex1.t);

        /* REDO these next two lines. The program is crashing when the factor is inf/nan */
        if (std::isinf(factor) || std::isnan(factor))
        	continue;

        glm::vec3 tmp_tan (factor * ( tex2.t * vec1.x - tex1.t * vec2.x),
                           factor * ( tex2.t * vec1.y - tex1.t * vec2.y),
                           factor * ( tex2.t * vec1.z - tex1.t * vec2.z));
        glm::vec3 tmp_bit (factor * (-tex2.s * vec1.x + tex1.s * vec2.x),
                           factor * (-tex2.s * vec1.y + tex1.s * vec2.y),
                           factor * (-tex2.s * vec1.z + tex1.s * vec2.z));

		std::printf("Tex1 %i   = (%f, %f)\n", face_id, tex1.x, tex1.y);
		std::printf("Tex2 %i   = (%f, %f)\n", face_id, tex2.x, tex2.y);
        std::printf("Factor %i %f\n", face_id, factor);
		std::printf("Tan %i    = (%f, %f, %f)\n", face_id, tmp_tan.x, tmp_tan.y, tmp_tan.z);
		std::printf("Bit %i    = (%f, %f, %f)\n", face_id++, tmp_bit.x, tmp_bit.y, tmp_bit.z);

		tan[vertex_indices[0]] += tmp_tan;
        tan[vertex_indices[1]] += tmp_tan;
        tan[vertex_indices[2]] += tmp_tan;
        bit[vertex_indices[0]] += tmp_bit;
        bit[vertex_indices[1]] += tmp_bit;
        bit[vertex_indices[2]] += tmp_bit;
    }

    for(unsigned int index = 0; index < num_vertices; ++index)
    {
    	// ??? Compute the per-vertex tangent and bitangent. Not sure about these next two lines
    	//tan[index] = glm::normalize(tan[index]);
    	//bit[index] = glm::normalize(bit[index]);

		std::printf("Vertex %i = (%f, %f, %f)\n", index, positions[index].x, positions[index].y, positions[index].z);
		std::printf("Tan %i    = (%f, %f, %f)\n", index, tan[index].x, tan[index].y, tan[index].z);
		std::printf("Bit %i    = (%f, %f, %f)\n", index, bit[index].x, bit[index].y, bit[index].z);
		std::printf("Normal %i = (%f, %f, %f)\n", index, normals[index].x, normals[index].y, normals[index].z);
		/*
		// Gram-Schmidt orthogonalize
        glm::vec3 tangent (glm::normalize(tan[index] - (glm::dot(normals[index], tan[index]) * normals[index])));

        // Calculate handedness
        float w = (glm::dot(glm::cross(normals[index], tan[index]), bit[index])) < 0.0f ? -1.0f : 1.0f;

        tangents[index] = glm::vec4(tangent, w);
        */
    }
}


/*
void textureMap(Mesh & mesh, TexMapMode mode)
{
    // Position data
    std::vector<Mesh::Face> faces;
    mesh.getFaceData(faces);
    // Faces data (indices into the position vector, indices into texture vector and normals
    std::vector<glm::vec3> positions;
    mesh.getVertexPositionData(positions);
    // NEW texture vector
    std::vector<glm::vec2> tex_coords;

    switch (mode)
    {
        case PLANAR:
            planarMapping(positions, faces, tex_coords);
            break;

        case CYLINDRICAL:
            cylindricalMapping(positions, faces, tex_coords);
            break;

        case SPHERICAL:
            sphericalMapping(positions, faces, tex_coords);
            break;

        default:
            printf("Unknown mapping mode\n");
            exit(1);
            break;
    }

    mesh.setFaceData(faces);
    mesh.setTextureCoordData(tex_coords);
}
*/

/**
* @brief Planar Mapping
*
* Given a set of vertices, it generates texture coordinates using
* planar mapping. It assumes the vertices fit within a cube of size
* 1 centered at the ORIGIN
*
* @param positions Vector with the 3D positions of all the vertices in the mesh
* @param faces Face information for the whole mesh
* @param tex_coords Vector with 2D texture coordinate points (referred to by the faces)
*/
/*
static void planarMapping(const std::vector<glm::vec3> & positions,
                          std::vector<Mesh::Face> & faces,
                          std::vector<glm::vec2> & tex_coords)
{
    glm::vec3 vtx;
    glm::vec2 tex;
    unsigned int num_tex_points = 0;
    
    std::vector<Mesh::Face>::iterator iter;
    for (iter = faces.begin(); iter != faces.end(); iter++)
    {
        for (int i = 0; i < 3; i++)
        {
            vtx = positions[iter->pos_indices[i]];
            tex.x = vtx.x + 0.5f;
            tex.y = vtx.y + 0.5f;
            tex_coords.push_back(tex);
            iter->tex_indices[i] = num_tex_points++;
        }
    }
}
*/

/**
* @brief Cylindrical Mapping
*
* Given a set of vertices, it generates texture coordinates using
* cylindrical mapping. It assumes the vertices fit within a cube of size
* 1 centered at the ORIGIN
*
* @param positions Vector with the 3D positions of all the vertices in the mesh
* @param faces Face information for the whole mesh
* @param tex_coords Vector with 2D texture coordinate points (referred to by the faces)
*/
/*
static void cylindricalMapping(const std::vector<glm::vec3> & positions,
                               std::vector<Mesh::Face> & faces,
                               std::vector<glm::vec2> & tex_coords)
{
    glm::vec3 vtx;
    glm::vec2 tex;
    unsigned int num_tex_points = 0;

    std::vector<Mesh::Face>::iterator iter;
    for (iter = faces.begin(); iter != faces.end(); iter++)
    {
        for (int i = 0; i < 3; i++)
        {
            vtx = positions[iter->pos_indices[i]];
            float theta = atan(vtx.y/vtx.x);
            // map theta from 0 to 2*PI to (0, 1)
            tex.x = theta / (2 * M_PI);;
            tex.y = vtx.z + 0.5f;
            tex_coords.push_back(tex);
            iter->tex_indices[i] = num_tex_points++;
        }
    }
}
*/

/**
* @brief Planar Mapping
*
* Given a set of vertices, it generates texture coordinates using
* planar mapping. It assumes the vertices fit within a cube of size
* 1 centered at the ORIGIN
*
* @param positions Vector with the 3D positions of all the vertices in the mesh
* @param faces Face information for the whole mesh
* @param tex_coords Vector with 2D texture coordinate points (referred to by the faces)
*/
/*
static void sphericalMapping(const std::vector<glm::vec3> & positions,
                             std::vector<Mesh::Face> & faces,
                             std::vector<glm::vec2> & tex_coords)
{
    glm::vec3 vtx;
    glm::vec2 tex;
    unsigned int num_tex_points = 0;

    std::vector<Mesh::Face>::iterator iter;
    for (iter = faces.begin(); iter != faces.end(); iter++)
    {
        for (int i = 0; i < 3; i++)
        {
            vtx = positions[iter->pos_indices[i]];
            float theta = atan(vtx.y/vtx.x);
            float phi = atan(vtx.z/sqrt(vtx.x*vtx.x + vtx.y*vtx.y + vtx.z*vtx.z));
            // map THETA from (0,2*PI) to (0, 1)
            // map PHI from (0,2*PI) to (0, 1)
            tex.x = theta / (2 * M_PI);
            tex.y = phi / M_PI;
            tex_coords.push_back(tex);
            iter->tex_indices[i] = num_tex_points++;
        }
    }
}
*/

} // End Graphics namespace
