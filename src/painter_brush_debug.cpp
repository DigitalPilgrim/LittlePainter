#include "painter_brush_debug.h"

void debug_paint_brush(painter_brush *pb)
{
    if (pb != nullptr)
    {
        QString type = "unknown";
        if (pb->Type == e_brushType::circleBrush) type = "CIRCLE";
        else if (pb->Type == e_brushType::textureBrush) type = "TEXTURE";
        else if (pb->Type == e_brushType::pen) type = "PEN";
        //else if (pb->Type == e_brushType::not_set) type = "NOT SET";
        qInfo() << "- BRUSH | "
                << "Alpha = " << pb->Alpha
                << ", Color(" << pb->Color.red() << ", " << pb->Color.green() << ", " << pb->Color.blue() << ", " << pb->Color.alpha() <<")"
                << ", Width = " << pb->Width
                << ", Type" << type;
    }
    else qInfo() << "- BRUSH | IS NULL";
}

