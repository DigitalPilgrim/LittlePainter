#ifndef GRAPHIC_HELPER_FUNCTIONS_H
#define GRAPHIC_HELPER_FUNCTIONS_H

#include "cmath"

namespace graphic_helper_functions
{
    // distance between two points
    // d = distance
    // x1, y1 = point 1
    // x2, y2 = point 2
    inline void distanceGet(float & d, const int &x1, const int &x2, const int &y1, const int &y2)
    {
        d = std::sqrtf( static_cast<float>(x1 - x2) *
                         static_cast<float>(x1 - x2) +
                         static_cast<float>(y1 - y2) *
                         static_cast<float>(y1 - y2));
    }
}

namespace ghf = graphic_helper_functions;

#endif // GRAPHIC_HELPER_FUNCTIONS_H
