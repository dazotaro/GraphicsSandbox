/*
 * NormalMapHelper.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: jusabiaga
 */

#include "NormalMapHelper.hpp"
#include <glm/glm.hpp>          // glm vector and matrix types


/**
* @brief Converts height map to normal map
*
* @param width Width of the image
* @param height Height of the image
* @param channels Number of channels (even if more than one, this function still assumes that they all hold the same value)
* @param image The raw data of the image
*/
void convertHeightMapToNormalMap(int width, int height, int channels, float scale, unsigned char *image)
{
    // Copy the image into a temporary array
    unsigned char *tmp_image = new unsigned char [width * height];
    for (int i = 0; i < width * height; i++)
    {
        tmp_image[i] = image[i * channels];
    }

    for (int y = 0; y < height; y++)
    {
        int y0, y1;
        y0 = (y == 0) ? (height - 1) : (y - 1);
        y1 = (y == (height - 1)) ? 0 : (y + 1);

        for (int x = 0; x < width; x++)
        {
            int x0, x1;
            x0 = (x == 0) ? (width - 1) : (x - 1);
            x1 = (x == (width - 1)) ? 0 : (x + 1);


            glm::vec3 s(1.0f, 0.0f, scale * (tmp_image[y  * width + x1] - tmp_image[y  * width + x0]));
            glm::vec3 t(0.0f, 1.0f, scale * (tmp_image[y1 * width + x ] - tmp_image[y0 * width + x ]));

            glm::vec3 normal = glm::normalize(glm::cross(s, t));
            // Map to range 0 --> 255 and clamp it
            normal = glm::clamp((normal + 1.0f) * 127.0f, 0.0f, 255.0f);

            int offset = (y * width + x) * channels;
            image[offset + 0] = normal.x;
            image[offset + 1] = normal.y;
            image[offset + 2] = normal.z;
        }
    }

    delete [] tmp_image;
}

