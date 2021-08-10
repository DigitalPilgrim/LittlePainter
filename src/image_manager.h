#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <QImage>
#include <QString>

enum class e_imageBrush
{
    texture1
    , texture2
    , custom
};

class image_manager
{
    static QString m_resPath;
public:
    image_manager();
    static void ResourcePath(QString path);
    static QString ResourcePath();

    // otvori dialogove okno
    static bool load(QImage& image);

    // otvori dialogove okno a path urci kde presne sa otvori dialogove okno ( na akom umiestneni)
    static bool load(QImage& image, QString path);
    // nacitavanie obrazku / texturi do stetca
    static bool loadAsResource(QImage& image, QString pathToImage);
    static bool loadAsResource(QImage& image, e_imageBrush ib);

    // ukladanie otvoreneho suboru, ak nie je este ulozeny, vyhodi okno na ulozenie (FileDialog)
    static bool save(QImage& image);
    // ukladanie suboru s FileDialogom
    static bool saveAs(QImage& image);

private:
    static bool loadInternal(QImage& image, QString fileName);
};

#endif // IMAGE_MANAGER_H
