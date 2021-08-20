#include "graphic_helper_functions.h"
#include <QDebug>

namespace graphic_helper_functions
{

    void areaGet(AreaPixels &area
                 , const QPoint &LeftTop
                 , const QSize &RightBottom
                 )
    {
        areaGet(area, LeftTop.x(), LeftTop.y(), RightBottom.width(), RightBottom.height());
    }

    void areaGet(AreaPixels &area
                 , const int &left
                 , const int &top
                 , const int &right
                 , const int &bottom
                 )
    {
        if (area.Left > left) area.Left = left;
        if (area.Top > top) area.Top = top;
        if (area.Right < right) area.Right = right;
        if (area.Bottom < bottom) area.Bottom = bottom;
    }

    void areaCompareAndReduce(AreaPixels &reduced, const AreaPixels &compared)
    {
        if (reduced.Top < compared.Top) reduced.Top = compared.Top;
        if (reduced.Left < compared.Left) reduced.Left = compared.Left;
        if (reduced.Right > compared.Right) reduced.Right = compared.Right;
        if (reduced.Bottom > compared.Bottom) reduced.Bottom = compared.Bottom;
        if (reduced.TopA < compared.TopA) reduced.TopA = compared.TopA;
        if (reduced.LeftA < compared.LeftA) reduced.LeftA = compared.LeftA;
        if (reduced.RightA > compared.RightA) reduced.RightA = compared.RightA;
        if (reduced.BottomA > compared.BottomA) reduced.BottomA = compared.BottomA;

        //qInfo() << "-- areaCompareAndReduce()";
    }

    void areaAdd(AreaPixels &area, const AreaPixels &add)
    {
        if (area.Top > add.Top) area.Top = add.Top;
        if (area.Left > add.Left) area.Left = add.Left;
        if (area.Right < add.Right) area.Right = add.Right;
        if (area.Bottom < add.Bottom) area.Bottom = add.Bottom;

        //qInfo() << "-- areaAdd()";

    }

    void areaAddA(AreaPixels &area, const AreaPixels &add)
    {

        if (area.TopA > add.TopA)
        {
            area.TopA = add.TopA;
            area.Top = add.Top;
            area.Bottom = area.BottomA - area.Top;
            //qInfo() << "-- areaAddA() | if (area.TopA > add.TopA)";
        }
        if (area.LeftA > add.LeftA)
        {
            area.LeftA = add.LeftA;
            area.Left = add.Left;
            area.Right = area.RightA - area.Left;
           // qInfo() << "-- areaAddA() | if (area.LeftA > add.LeftA)";
        }

        if (area.RightA < add.RightA)
        {
            area.RightA = add.RightA;
            area.Right = add.RightA - area.Left;
            //qInfo() << "-- areaAddA() | if (area.RightA < add.RightA)";
        }
        if (area.BottomA < add.BottomA)
        {
            area.BottomA = add.BottomA;
            area.Bottom = add.BottomA - area.Top;
            //qInfo() << "-- areaAddA() | if (area.BottomA < add.BottomA)";
        }

        //qInfo() << "-- areaAddA()";
    }

    void areaSet(AreaPixels &area, const AreaPixels &set)
    {
        area.Top = set.Top;
        area.Left = set.Left;
        area.Right = set.Right;
        area.Bottom = set.Bottom;
        area.TopA = set.TopA;
        area.LeftA = set.LeftA;
        area.RightA = set.RightA;
        area.BottomA= set.BottomA;
        //qInfo() << "-- areaSet()";

    }

    void areaReset(AreaPixels &area)
    {
        area.Top = 0;
        area.Left = 0;
        area.Right = 0;
        area.Bottom = 0;
        area.TopA = 0;
        area.LeftA = 0;
        area.RightA = 0;
        area.BottomA = 0;
        //qInfo() << "-- areaReset()";
    }

    void areaMax(AreaPixels &area, const QSize &max)
    {
        if (area.Top < 0) area.Top = 0;
        if (area.Left < 0) area.Left = 0;
        if (area.Right > max.width()) area.Right = max.width();
        if (area.Bottom > max.height()) area.Bottom = max.height();
        if (area.TopA < 0) area.TopA = 0;
        if (area.LeftA < 0) area.LeftA = 0;
        if (area.RightA > max.width()) area.RightA = max.width();
        if (area.BottomA > max.height()) area.BottomA = max.height();

        //qInfo() << "-- areaMax()";
    }

    bool areaCollide(AreaPixels &area, const AreaPixels &compareTo)
    {
        if (area.Left < compareTo.Left)
        {

        }
        return false;
    }




}
