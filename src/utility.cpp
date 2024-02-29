//-----------------------------------------------------------------------------
// File:			utility.cpp
// Original Author:	Ryan Campbell
//-----------------------------------------------------------------------------
#include "utility.h"
namespace Utility
{
// random seed for random function
static void initSeed()
{
    static bool once = true;
    if (once)
    {
        srand((unsigned int)time(NULL));
        once = false;
    }
}

// returns random float inclusive
float randomFloat(float min, float max)
{
    initSeed();
    if (max < min)
    {
        return min;
        puts("max was less than min in utility::randomFloat()");
    }
    float somePercentage = (float)rand() / (float)RAND_MAX;
    return min + (somePercentage * (max - min));
}

// specify points in clockwise order if you where looking at the plane
// in the same direction that you want the normal to point.(i.e for frustum, from the outside)
// points must be non-colinear
glm::vec3 planeNormalFrom3Points(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    return glm::normalize(glm::cross(p1 - p2, p1 - p3));
}

} // namespace Utility