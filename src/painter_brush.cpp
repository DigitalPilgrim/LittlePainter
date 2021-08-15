#include "painter_brush.h"
#include "painter_brush_circle.h"
#include "painter_brush_texture.h"
#include "painter_brush_debug.h"
#include <QMessageBox>
#include <QDebug>

void painter_brush::set(const painter_brush *pb)
{
    Image = pb->Image;
    Color = pb->Color;
    Width = pb->Width;
    Alpha = pb->Alpha;
    Type = pb->Type;
    //OriginalImageAlpha = pb->OriginalImageAlpha;
    ImageBrushSelected = pb->ImageBrushSelected;
    qInfo() << "-- painter_brush::set() ----------------";
    debug_paint_brush(this);
    qInfo() << "----------------------------------------";
}

PainterBrushHolder::PainterBrushHolder(const PainterBrushHolder &pbh)
{
    set(pbh.pb);
}

PainterBrushHolder::PainterBrushHolder(painter_brush *painterBrush)
{
    if (painterBrush != nullptr)
        set(painterBrush);
}

PainterBrushHolder::~PainterBrushHolder()
{
    if (pb != nullptr)
    {
        delete pb;
    }
}

void PainterBrushHolder::set(painter_brush *painterBrush)
{
    if (painterBrush == nullptr)  { return; }

    if (pb != nullptr)
    {
        if (pb->Type != painterBrush->Type)
        {
            delete pb;
            if (create(painterBrush))
            {
                pb->set(painterBrush);
            }
        }
        else pb->set(painterBrush);
    }
    else
    {
        if (create(painterBrush))
        {
            pb->set(painterBrush);
        }
    }
}

bool PainterBrushHolder::create(painter_brush *painterBrush)
{
    if (painterBrush->Type == e_brushType::circleBrush)
    {
        pb = new painter_brush_circle();
        return true;
    }
    else if (painterBrush->Type == e_brushType::textureBrush)
    {
        pb = new painter_brush_texture();
        return true;
    }
    pb = nullptr;
    return false;
}

void PainterBrushHolder::draw(const DrawManagerArgs &args)
{
    if (pb != nullptr)
        pb->draw(args);
}

void PainterBrushHolder::setImageColor(const QColor &value)
{
    if (pb != nullptr)
        pb->setImageColor(value);
}

void PainterBrushHolder::release()
{
    if (pb != nullptr)
    {
        delete pb;
    }
}

