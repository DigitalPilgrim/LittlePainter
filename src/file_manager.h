#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QImage>

enum class e_file_state
{
    opened
    , saved
    , closed
    , error
};

class file_manager
{
    static QString currentFile;
    file_manager();
public:
    static void clearFileName();
    static e_file_state saveImage(QImage & image, QWidget * parent = nullptr);
    static e_file_state saveImage(QImage & image, bool saveAs = false, QWidget * parent = nullptr);
    static e_file_state openImage(QImage &image, QWidget * parent = nullptr);
    static e_file_state openImage(QImage &image, QString path);

};

#endif // FILE_MANAGER_H
