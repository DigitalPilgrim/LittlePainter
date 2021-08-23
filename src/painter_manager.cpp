#include "painter_manager.h"
#include <QMessageBox>
#include "graphic_helper_functions.h"
#include <QDebug>

#include "painter_brush_circle.h"
#include "painter_brush_texture.h"

// -------------------------------------------------------------------------------------------------------------------------

painter_manager::painter_manager()
{
    m_brushes.push_back(new painter_brush_circle());
    m_brushes.push_back(new painter_brush_texture());

    m_brushes[BRUSH_CIRCLE]->Width = 25;
    m_brushes[BRUSH_TEXTURE]->Width = 50;
}

// -------------------------------------------------------------------------------------------------------------------------

painter_manager::~painter_manager()
{
    for (painter_brush * pb : m_brushes)
    {
        delete pb;
    }
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::draw(const DrawManagerArgs &args)
{
    m_brushes[BRUSH_SELECTED]->draw(args);
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::BrushType(e_brushType value)
{
    m_brushType = value;
    if (value == e_brushType::circleBrush) BRUSH_SELECTED = BRUSH_CIRCLE;
    else if (value == e_brushType::textureBrush) BRUSH_SELECTED = BRUSH_TEXTURE;
}

void painter_manager::ImageBrushSelect(e_imageBrush value)
{
    m_imageBrush = value;
    m_brushes[BRUSH_TEXTURE]->ImageBrushSelected = value;
    loadTexture();
}

// -------------------------------------------------------------------------------------------------------------------------

e_imageBrush painter_manager::ImageBrushSelected() const
{
    return m_imageBrush;
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::Color(QColor value)
{
    if (m_brushType != e_brushType::not_set)
    {
        m_brushes[BRUSH_SELECTED]->Color = value;
        if (m_brushType == e_brushType::textureBrush)
        {
            m_brushes[BRUSH_TEXTURE]->setImageColor(value);
        }

    }
}

// -------------------------------------------------------------------------------------------------------------------------

QColor painter_manager::Color() const
{
    return m_brushes[BRUSH_SELECTED]->Color;
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::BrushTransparency(const float &value)
{
    m_brushes[BRUSH_SELECTED]->Alpha = value;
    m_brushes[BRUSH_SELECTED]->setImageAlpha(value);
}

// -------------------------------------------------------------------------------------------------------------------------

float painter_manager::BrushTransparency() const
{
    return m_brushes[BRUSH_SELECTED]->Alpha;
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::Width(int value)
{
    m_brushes[BRUSH_SELECTED]->Width = value;
    m_brushes[BRUSH_SELECTED]->resize();
}

// -------------------------------------------------------------------------------------------------------------------------

int painter_manager::Width() const
{
    return m_brushes[BRUSH_SELECTED]->Width;
}


// =========================================================================================================================

void painter_manager::Initialize()
{
    m_brushes[BRUSH_TEXTURE]->Width = 50;
    loadTexture();
}

painter_brush *painter_manager::GetBrush()
{
    return m_brushes[BRUSH_SELECTED];
}

// =========================================================================================================================

void painter_manager::loadTexture()
{
    /*if (image_manager::loadAsResource(m_brush[static_cast<int>(e_brushType::textureBrush)].Image
                                      , m_brush[static_cast<int>(e_brushType::textureBrush)].ImageBrushSelected))*/

    if (image_manager::loadAsResource(m_brushes[BRUSH_TEXTURE]->OriginalImageTexture
                                      , m_brushes[BRUSH_TEXTURE]->ImageBrushSelected))
    {
        qInfo() << "image loaded";
        //m_brushes[BRUSH_TEXTURE]->Image = m_brushes[BRUSH_TEXTURE]->OriginalImageTexture;
        m_brushes[BRUSH_TEXTURE]->reinitialize();
        /*m_brushes[BRUSH_TEXTURE]->setImageColor(m_brushes[BRUSH_TEXTURE]->Color);
        m_brushes[BRUSH_TEXTURE]->setImageAlpha(m_brushes[BRUSH_TEXTURE]->Alpha, true);*/
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "Could not load texture.");
    }
}

// =========================================================================================================================

