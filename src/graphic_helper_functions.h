#ifndef GRAPHIC_HELPER_FUNCTIONS_H
#define GRAPHIC_HELPER_FUNCTIONS_H

#include "cmath"
#include <QPoint>
#include <QSize>

namespace graphic_helper_functions
{
    // distance between two points
    // d = distance
    // x1, y1 = point 1
    // x2, y2 = point 2
    inline void distanceGet(float & d, const int &x1, const int &x2, const int &y1, const int &y2)
    {
        d = std::sqrtf( static_cast<float>(x1 - x2)  *
                         static_cast<float>(x1 - x2) +
                         static_cast<float>(y1 - y2) *
                         static_cast<float>(y1 - y2));
    }

    struct AreaPixels
    {
        int Top = 0;
        int Left = 0;
        int Right = 0;
        int Bottom = 0;
        int TopA = 0;
        int LeftA = 0;
        int RightA = 0;
        int BottomA = 0;

        AreaPixels() {}
        AreaPixels(int top, int left, int right, int bottom)
            : Top(top), Left(left), Right(right), Bottom(bottom)
        {}

        bool IsZero()
        {
            if (Top == 0
                    && Left == 0
                    && Right == 0
                    && Bottom == 0)
            {
                return true;
            }
            return false;
        }
    };

    /*inline*/ void areaGet(AreaPixels & area
                        , const QPoint & LeftTop
                        , const QSize & RightBottom
                        );

    /*inline*/ void areaGet(AreaPixels & area
                        , const int &left
                        , const int &top
                        , const int &right
                        , const int &bottom
                        );
    /*inline*/  void areaAdd(AreaPixels & area
                        , const AreaPixels & add);

    /*inline*/  void areaAddA(AreaPixels & area
                        , const AreaPixels & add);

    /*inline*/  void areaMax(AreaPixels & area
                        , const QSize & max);

    /*inline*/  void areaSet(AreaPixels & area
                        , const AreaPixels & set);

    /*inline*/ void areaReset(AreaPixels & area);

    // return:
    // true = area collide with compareTo
    // false = area is outside compareTo
    bool areaCollide(AreaPixels & area, const AreaPixels & compareTo);

    // reduced = AreaPixels ktora sa porovnava s compraed a ked jedna zo stran presahuje stranu z compared
    // reduced bude redukovana na compared.
    // pouziva sa na to, aby area nepresiahla velkost platna, nieco ako
    // ap.Left < MIN.Left;
    // ap.Right > MAX.Right;
    inline void areaCompareAndReduce(AreaPixels & reduced, const AreaPixels & compared);
}

namespace ghf = graphic_helper_functions;

#endif // GRAPHIC_HELPER_FUNCTIONS_H
