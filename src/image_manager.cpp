#include "image_manager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "file_manager.h"
// ----------------------------------------------------------------------------------------------------
QString image_manager::m_resPath;

image_manager::image_manager()
{

}

// ----------------------------------------------------------------------------------------------------

void image_manager::ResourcePath(QString path)
{
    m_resPath = path + "/";
    qInfo() << "Resource path = " << m_resPath;
}

// ----------------------------------------------------------------------------------------------------

QString image_manager::ResourcePath()
{
    return m_resPath;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::load(QImage &image)
{
    bool ok = false;
    return ok;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::load(QImage &image, QString path)
{
    bool ok = false;
    return ok;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::loadAsResource(QImage &image, QString pathToImage)
{
    return image.load(pathToImage);
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::loadAsResource(QImage &image, e_imageBrush ib)
{
    bool ok = false;
    QString fileName = m_resPath;
    switch(ib)
    {
        case e_imageBrush::texture1: fileName += "texture1.png"; ok = true; qInfo() << "texture 1 will be selected"; break;
        case e_imageBrush::texture2: fileName += "texture2.png"; ok = true; qInfo() << "texture 2 will be selected"; break;
        case e_imageBrush::custom: break;
    }
    if (ok)
    {
        qInfo() << "texture will be selected: " << fileName;
        //ok = image.load(fileName);
        QImage img;
        ok = img.load(fileName);
        if (ok)
        {
            image = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        }
    }
    else
    {
        if (ib == e_imageBrush::custom)
        {
            QImage img;
            e_file_state fs = file_manager::openImage(img, m_resPath);
            if (fs == e_file_state::opened || fs == e_file_state::closed)
            {
                image = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
                ok = true;
            }
        }
        else
        {
            QMessageBox::warning(nullptr
                                , "Error"
                                , "Loading texture from loadAsResource(QImage &image, e_imageBrush ib) function failed.");
        }
    }
    return ok;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::save(QImage &image)
{
    bool ok = false;
    return ok;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::saveAs(QImage &image)
{
    bool ok = false;
    return ok;
}

// ----------------------------------------------------------------------------------------------------

bool image_manager::loadInternal(QImage &image, QString fileName)
{
    bool ok = false;
    return ok;
}
